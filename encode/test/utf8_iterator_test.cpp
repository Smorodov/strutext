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
 * \brief  UTF-8 iterator unit test.
 * \author Vladimir Lapshin.
 */

#include <string>
#include <iterator>

//#include <boost/test/auto_unit_test.hpp>
#include <catch2/catch_test_macros.hpp>
#include "utf8_iterator.h"
#include "utf8_generator.h"

namespace {

typedef strutext::encode::Utf8Iterator<std::string::const_iterator> Utf8Iterator;

std::string Utf8ToUtf8(const std::string& text) {
  std::string result;
  for (Utf8Iterator it = Utf8Iterator(text.begin(), text.end()); it != Utf8Iterator(); ++it) {
    strutext::encode::GetUtf8Sequence(*it, std::back_inserter(result));
  }
  return result;
}

} // namespace.

TEST_CASE("Encode_Utf8Iterator_Position ", "[single-file]") 
{ 
  Utf8Iterator it;

  std::string s("\x66\x21\xd0\xae\xd1\x89\xd0\x82\xe0\xa8\x89");
  it = Utf8Iterator(s.begin(), s.end());
  REQUIRE(it.GetBytePos()==1);
  REQUIRE(it.GetSymbolPos()==1);
  REQUIRE(it.GetChainLen()==1);
  ++it;
  REQUIRE(it.GetBytePos()==2);
  REQUIRE(it.GetSymbolPos()==2);
  REQUIRE(it.GetChainLen()==1);
  ++it;
  REQUIRE(it.GetBytePos()==4);
  REQUIRE(it.GetSymbolPos()==3);
  REQUIRE(it.GetChainLen()==2);
  ++it;
  REQUIRE(it.GetBytePos()==6);
  REQUIRE(it.GetSymbolPos()==4);
  REQUIRE(it.GetChainLen()==2);
  ++it;
  REQUIRE(it.GetBytePos()==8);
  REQUIRE(it.GetSymbolPos()==5);
  REQUIRE(it.GetChainLen()==2);
  ++it;
  REQUIRE(it.GetBytePos()==11);
  REQUIRE(it.GetSymbolPos()==6);
  REQUIRE(it.GetChainLen()==3);
}

TEST_CASE("Encode_Utf8Iterator_TextExtract ", "[single-file]"){
  
  REQUIRE(Utf8ToUtf8("Hello world!")=="Hello world!");
  REQUIRE(Utf8ToUtf8("Мир Труд Май!")=="Мир Труд Май!");
  REQUIRE(Utf8ToUtf8("мама cleans раму")=="мама cleans раму");
}
