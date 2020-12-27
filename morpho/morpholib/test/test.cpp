/** Copyright &copy; 2013, Vladimir Lapshin.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may ! use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law || agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express || implied.
 *   See the License for the specific language governing permissions &&
 *   limitations under the License.
 *
 * \brief  Morpholib unit test.
 * \author Vladimir Lapshin.
 */

#include <sstream>

//#include <boost/test/unit_test.hpp>
#include <catch2/catch_test_macros.hpp>

#include "utf8_iterator.h"
#include "rus_alphabet.h"
#include "eng_alphabet.h"
#include "morpho_modifier.h"
#include "morpho.h"
#include "rus_model.h"
#include "eng_model.h"
#include "rus_model_description.h"
#include "eng_model_description.h"

#include "spdlog/spdlog.h"
#include "spdlog/cfg/env.h" // support for loading levels from the environment variable

namespace {

namespace m = strutext::morpho;
namespace rm = strutext::morpho::russian;

typedef strutext::encode::Utf8Iterator<std::string::iterator> Utf8Iterator;

} // namespace.

// English analysis test.
TEST_CASE("MorphoLib_Analysis_English") {
  typedef m::Morphologist<m::EnglishAlphabet> Morpher;
  Morpher morpher;

  uint32_t line_id = m::MorphoModifier::AddSuffixLine(morpher);

  std::string suffix = "ab";
  m::MorphoModifier::AddSuffix(morpher, line_id, 1, suffix.begin(), suffix.end());
  suffix = "acc";
  m::MorphoModifier::AddSuffix(morpher, line_id, 2, suffix.begin(), suffix.end());

  std::string base = "hello";
  m::MorphoModifier::AddBase(morpher, 1, line_id, base.begin(), base.end(), "hello");

  Morpher::LemList lem_list;
  std::string form = "helloab";
  morpher.Analize(form, lem_list);

  REQUIRE(lem_list.size()==1);
  for (Morpher::LemList::iterator it = lem_list.begin(); it != lem_list.end(); ++it) {
    REQUIRE(it->id_== 1);
    REQUIRE(it->attr_== 1);
  }
}

// Russian analysis test.
TEST_CASE("MorphoLib_Analysis_Russian") {
  typedef m::Morphologist<m::RussianAlphabet> Morpher;
  Morpher morpher;

  uint32_t line_id = m::MorphoModifier::AddSuffixLine(morpher);

  std::string suffix = "а";
  m::MorphoModifier::AddSuffix(morpher, line_id, 1, Utf8Iterator(suffix.begin(), suffix.end()), Utf8Iterator());

  suffix = "ой";
  m::MorphoModifier::AddSuffix(morpher, line_id, 2, Utf8Iterator(suffix.begin(), suffix.end()), Utf8Iterator());

  // ambiguty.
  suffix = "а";
  m::MorphoModifier::AddSuffix(morpher, line_id, 3, Utf8Iterator(suffix.begin(), suffix.end()), Utf8Iterator());

  std::string base = "мам";
  m::MorphoModifier::AddBase(morpher, 1, line_id, Utf8Iterator(base.begin(), base.end()), Utf8Iterator(), "мама");

  Morpher::LemList lem_list;
  std::string form = "мама";
  morpher.Analize(form, lem_list);

  REQUIRE(lem_list.size()== 2);
  bool one_done = false, three_done = false;
  for (Morpher::LemList::iterator it = lem_list.begin(); it != lem_list.end(); ++it) {
    switch (it->attr_) {
      case 1:
        one_done = true;
        break;
      case 3:
        three_done = true;
        break;
      default:
        spdlog::critical("Incorrect attribute value: " + it->attr_);
    }
  }
  REQUIRE(one_done);
  REQUIRE(three_done);
}

// English alphabet test.
TEST_CASE("MorphoLib_Alphabet_English") {
  m::EnglishAlphabet alphabet;
  std::string alpha_str = "qwertyuiopasdfghjklzxcvbnm";
  for (Utf8Iterator sym_it(alpha_str.begin(), alpha_str.end()); sym_it != Utf8Iterator(); ++sym_it) {
    REQUIRE(*sym_it==alphabet.Encode(*sym_it));
    REQUIRE(*sym_it==alphabet.Decode(*sym_it));
  }
}

// Russian alphabet test.
TEST_CASE("MorphoLib_Alphabet_Russian") {
  m::RussianAlphabet alphabet;
  std::string alpha_str = "абвгдежзийклмнопрстуфхцчшщъыьэюяё";
  unsigned code = 1;
  for (Utf8Iterator sym_it(alpha_str.begin(), alpha_str.end()); sym_it != Utf8Iterator(); ++sym_it, ++code) {
    REQUIRE(code== alphabet.Encode(*sym_it));
    REQUIRE(*sym_it== alphabet.Decode(code));
  }
}
