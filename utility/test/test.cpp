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
 * \file   test.cpp
 * \brief  Ngram iterator unit test.
 * \author Vladimir Lapshin.
 * \date   23.11.2013
 */

#include <stdint.h>

#include <string>
#include <vector>

#include "spdlog/cfg/env.h"  // support for loading levels from the environment variable
#include "spdlog/spdlog.h"
#include <catch2/catch_test_macros.hpp>

//#include <boost/test/unit_test.hpp>

#include "utf8_iterator.h"
#include "utf8_generator.h"
#include "ngram_iterator.h"
#include "symbol_iterator.h"
#include "word_iterator.h"

namespace strutext { namespace utility {

TEST_CASE("Utility_NgrammIterator_Symbols") {
  std::string text = "abcde";
  typedef NgramIterator<char, std::string::const_iterator> NgramIteratorImpl;
  std::vector<std::string> result;
  for (NgramIteratorImpl it(text.begin(), text.end(), 1, 5), end; it != end; ++it) {
    NgramIteratorImpl::Ngram ngram = *it;
    result.push_back(std::string(ngram.begin(), ngram.end()));
  }
  REQUIRE(result.size() == 15);
  REQUIRE(result[0] == "a");
  REQUIRE(result[1] == "ab");
  REQUIRE(result[2] == "abc");
  REQUIRE(result[3] == "abcd");
  REQUIRE(result[4] == "abcde");
  REQUIRE(result[5] == "b");
  REQUIRE(result[6] == "bc");
  REQUIRE(result[7] == "bcd");
  REQUIRE(result[8] == "bcde");
  REQUIRE(result[9] == "c");
  REQUIRE(result[10] == "cd");
  REQUIRE(result[11] == "cde");
  REQUIRE(result[12] == "d");
  REQUIRE(result[13] == "de");
  REQUIRE(result[14] == "e");
}

TEST_CASE("Utility_NgrammIterator_Utf8RussianText") {
  std::string text = "абвгдеж";
  typedef encode::Utf8Iterator<std::string::const_iterator> Utf8IteratorImpl;
  typedef NgramIterator<symbols::SymbolCode, Utf8IteratorImpl> NgramIteratorImpl;
  std::vector<std::string> result;
  Utf8IteratorImpl utf8_begin(text.begin(), text.end()), utf8_end;
  for (NgramIteratorImpl it(utf8_begin, utf8_end, 2, 3), end; it != end; ++it) {
    NgramIteratorImpl::Ngram ngram = *it;
    std::string text;
    encode::GetUtf8Sequence(ngram.begin(), ngram.end(), std::back_inserter(text));
    result.push_back(text);
  }
  REQUIRE(result.size() == 11);
  REQUIRE(result[0] == "аб");
  REQUIRE(result[1] == "абв");
  REQUIRE(result[2] == "бв");
  REQUIRE(result[3] == "бвг");
  REQUIRE(result[4] == "вг");
  REQUIRE(result[5] == "вгд");
  REQUIRE(result[6] == "гд");
  REQUIRE(result[7] == "где");
  REQUIRE(result[8] == "де");
  REQUIRE(result[9] == "деж");
  REQUIRE(result[10] == "еж");
}

TEST_CASE("Utility_SymbolAlphaIterator_EnglishGeneral") {
  typedef FilterIterator<symbols::SymbolCode, std::string::const_iterator, AlphaFilter, SymTransform> IteratorImpl;
  std::string text = "   !:,213213Hello,]]][[[[    World  !   ";
  AlphaFilter filter;
  std::string result;
  for (IteratorImpl it(text.begin(), text.end(), filter, LowerAndSpace), end; it != end; ++it) {
    result += static_cast<char>(*it);
  }
  REQUIRE(result == " hello world ");
}

TEST_CASE("Utility_SymbolAlphaIterator_RussianGeneral") {
  typedef encode::Utf8Iterator<std::string::const_iterator> Utf8IteratorImpl;
  typedef FilterIterator<symbols::SymbolCode, Utf8IteratorImpl, AlphaFilter, SymTransform> IteratorImpl;
  std::string text = "    12321321  ,,, \\///!!! Здравствуй, [[[ Мир  ]]]  !!!!";
  AlphaFilter filter;
  std::string result;
  Utf8IteratorImpl utf8_begin(text.begin(), text.end()), utf8_end;
  for (IteratorImpl it(utf8_begin, utf8_end, filter, LowerAndSpace), end; it != end; ++it) {
    encode::GetUtf8Sequence(*it, std::back_inserter(result));
  }
  REQUIRE(result == " здравствуй мир ");
}

TEST_CASE("Utility_WordIterator_EnglishRussianWords") {
  typedef encode::Utf8Iterator<std::string::const_iterator> Utf8IteratorImpl;
  typedef WordIterator<symbols::SymbolCode, Utf8IteratorImpl> WordIteratorImpl;
  std::string text = "Добро пожаловать, dear friend";
  Utf8IteratorImpl utf8_begin(text.begin(), text.end()), utf8_end;
  size_t counter = 0;
  WordIteratorImpl it(utf8_begin, utf8_end);
  for (; it != WordIteratorImpl(); ++it) {
    ++counter;
    std::string result;
    encode::GetUtf8Sequence((*it).begin(), (*it).end(), std::back_inserter(result));
    switch (counter) {
      case 1:
        REQUIRE(result == "Добро");
        break;
      case 2:
        REQUIRE(result == "пожаловать");
        break;
      case 3:
        REQUIRE(result == "dear");
        break;
      case 4:
        REQUIRE(result == "friend");
        break;
      default:
        REQUIRE("Incorrect number of words extracted");
        break;
    }
  }
  REQUIRE(counter == 4);
}

}} // namespace strutext, utility.
