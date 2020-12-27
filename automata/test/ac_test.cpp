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
 * \brief  Aho-Corasick trie unit test.
 * \author Vladimir Lapshin.
 */

#include <stdint.h>

#include <string>
#include <sstream>

// #include <boost/test/unit_test.hpp>

#include "aho_corasick.h"
#include "ac_iterator.h"
#include "flex_transitions.h"
#include "serializer.h"
#include <catch2/catch_test_macros.hpp>
namespace {

// Type definitions.
typedef strutext::automata::FlexTransitions<char>                                 Trans;
typedef strutext::automata::AhoCorasickTrie<Trans, uint64_t>                      AcTrie;
typedef strutext::automata::FailMoveGenerator<AcTrie>                             AcTrieGenerator;
typedef strutext::automata::AcProcessor<AcTrie>                                   AcTrieProcessor;
typedef strutext::automata::AcSerializer<AcTrie>                                  AcSerializer;
typedef strutext::automata::AcSymbolIterator<AcTrie, std::string::const_iterator> AcIterator;
typedef strutext::automata::AcChainIterator<AcTrie, std::string::const_iterator>  ChainIterator;

// Utilities.
struct TrieUtils {
  // Addin a chain to the trie.
  static void AddChainToTrie(const std::string& chain, AcTrie::Attribute attr, AcTrie& trie) {
    trie.AddChain(chain.begin(), chain.end(), attr);
  }
};

}  // namespace.

bool Automata_AcTrie_Search(void) {
  bool ok=true;
  // Create the trie && add chains there.
  AcTrie trie;
  TrieUtils::AddChainToTrie("abcde", 1, trie);
  TrieUtils::AddChainToTrie("ab", 2, trie);
  TrieUtils::AddChainToTrie("abc", 3, trie);
  TrieUtils::AddChainToTrie("abcd", 4, trie);
  TrieUtils::AddChainToTrie("cde", 5, trie);

  // Generate fail moves.
  AcTrieGenerator::Generate(trie);

  // Find chains.
  std::string text = "cdeabcde";
  AcTrieProcessor searcher(trie);
  strutext::automata::StateId state = strutext::automata::kStartState;
  for (size_t i = 0; i < text.length(); ++i) {
    state = searcher.Move(state, text[i]);
    const AcTrie::AttributeList& st_attrs = trie.GetStateAttributes(state);
    switch (i) {
      case 0: case 1: case 3: // There must be found nothing.
        CHECK(st_attrs.size() == 0);
        if(st_attrs.size() != 0) {
          ok = false;
        }
        break;
      case 2: // "cde" must be found.
        CHECK(st_attrs.size() == 1);
        if (st_attrs.size() != 1) {
          ok = false;
        }
        for (size_t j = 0; j < st_attrs.size(); ++j) {
        CHECK(st_attrs[j] == 5);
          if (st_attrs[j] != 5) {
            ok = false;
          }
        }
        break;
      case 4: // "ab" must be found.
        CHECK(st_attrs.size() == 1);
        if (st_attrs.size() != 1) {
          ok = false;
        }
        for (size_t j = 0; j < st_attrs.size(); ++j) {
        CHECK(st_attrs[j] == 2);
          if (st_attrs[j] != 2) {
            ok = false;
          }
        }
        break;
      case 5: // "abc" must be found.
        CHECK(st_attrs.size() == 1);
        if (st_attrs.size() != 1) {
          ok = false;
        }
        for (size_t j = 0; j < st_attrs.size(); ++j) {
        CHECK(st_attrs[j] == 3);
          if (st_attrs[j] != 3) {
            ok = false;
          }
        }
        break;
      case 6: // "abcd" must be found.
        CHECK(st_attrs.size() == 1);
        if (st_attrs.size() != 1) {
          ok = false;
        }
        for (size_t j = 0; j < st_attrs.size(); ++j) {
        CHECK(st_attrs[j] == 4);
          if (st_attrs[j] != 4) {
            ok = false;
          }
        }
        break;
      case 7: // "cde" && "abcde" must be found.
        CHECK(st_attrs.size() == 2);
        if (st_attrs.size() != 2) {
          ok = false;
        }
        for (size_t j = 0; j < st_attrs.size(); ++j) {
        CHECK( (st_attrs[j] == 1 || st_attrs[j] == 5) );
          if (st_attrs[j] != 1 && st_attrs[j]!=5) {
            ok = false;
          }
        }
        break;
      default:
        CHECK(false);
        ok = false;
        break;
    }
  }
  return ok;
}

bool Automata_AcTrie_Serialize(void) {
  bool ok = true;
  // Create the trie && add chains there.
  AcTrie trie;
  TrieUtils::AddChainToTrie("he", 1, trie);
  TrieUtils::AddChainToTrie("hers", 2, trie);
  TrieUtils::AddChainToTrie("his", 3, trie);
  TrieUtils::AddChainToTrie("she", 4, trie);

  // Generate fail transitions.
  AcTrieGenerator::Generate(trie);

  // Stream to serialization.
  std::stringstream ss;

  // Serialize the trie.
  AcSerializer::Serialize(trie, ss);

  // Deserialize the trie.
  AcTrie trie1;
  AcSerializer::Deserialize(trie1, ss);

  // Search chains in the text.
  std::string text = "ushers";
  AcTrieProcessor searcher(trie1);
  strutext::automata::StateId state = strutext::automata::kStartState;
  for (size_t i = 0; i < text.length(); ++i) {
    state = searcher.Move(state, text[i]);
    const AcTrie::AttributeList& st_attrs = trie.GetStateAttributes(state);
    switch (i) {
      case 0: case 1: case 2: case 4: // Nothing should be found.
        CHECK(st_attrs.size() == 0);
        if (st_attrs.size() != 0) {
          ok = false;
        }
        break;
      case 3: // "she" && "he" must be found.
        CHECK(st_attrs.size() == 2);
        if (st_attrs.size() != 2) {
          ok = false;
        }
        for (size_t j = 0; j < st_attrs.size(); ++j) {
        CHECK( (st_attrs[j] == 1 || st_attrs[j] == 4) );
          if (st_attrs[j] != 1 && st_attrs[j]!=4) {
            ok = false;
          }
        }
        break;
      case 5: // "hers" must be found.
        CHECK(st_attrs.size() == 1);
        if (st_attrs.size() != 1) {
          ok = false;
        }
        for (size_t j = 0; j < st_attrs.size(); ++j) {
        CHECK(st_attrs[j] == 2);
          if (st_attrs[j] != 2) {
            ok = false;
          }
        }
        break;
      default:
        CHECK(false);
        ok = false;
        break;
    }
  }
  return ok;
}

// AC iterator simple search test.
bool Automata_AcIterator_Search(void) {
  bool ok = true;
  // Create the trie && add chains there.
  AcTrie trie;
  TrieUtils::AddChainToTrie("he", 1, trie);
  TrieUtils::AddChainToTrie("hers", 2, trie);
  TrieUtils::AddChainToTrie("his", 3, trie);
  TrieUtils::AddChainToTrie("she", 4, trie);

  // Generate fail transitions.
  AcTrieGenerator::Generate(trie);

  // Find chains.
  std::string text = "ushers";
  size_t pos = 0;
  for (AcIterator it(text.begin(), text.end(), trie); it != AcIterator(); ++it, ++pos) {
    switch (pos) {
      case 0: case 1: case 2: case 4: // Nothing should be found.
        CHECK((*it).size() == 0);
        if ((*it).size() != 0) {
          ok = false;
        }
        break;
      case 3: // "she" && "he" should be found.
        CHECK((*it).size() == 2);
        if ((*it).size() != 2) {
          ok = false;
        }
        for (size_t i = 0; i < (*it).size(); ++i) {
          CHECK( ((*it)[i] == 1 || (*it)[i] == 4) );
          if ((*it)[i] != 1 && (*it)[i]!=4) {
            ok = false;
          }
        }
        break;
      case 5: // "hers" should be found.
        CHECK((*it).size() == 1);
        if ((*it).size() != 1) {
          ok = false;
        }
        for (size_t i = 0; i < (*it).size(); ++i) {
          CHECK((*it)[i] == 2);
          if ((*it)[i] != 2) {
            ok = false;
          }
        }
        break;
      default:
        CHECK(false);
        ok = false;
        break;
    }
  }
  return ok;
}

// AC chain iterator simple search test.
bool Automata_ChainIterator_Search(void) {
  bool ok = true;
  // Create the trie && add chains there.
  AcTrie trie;
  TrieUtils::AddChainToTrie("he", 1, trie);
  TrieUtils::AddChainToTrie("hers", 2, trie);
  TrieUtils::AddChainToTrie("his", 3, trie);
  TrieUtils::AddChainToTrie("she", 4, trie);

  // Generate fail transitions.
  AcTrieGenerator::Generate(trie);

  // Chains find flags.
  bool he_found = false, she_found = false, hers_found = false;

  // Find chains.
  std::string text = "ushers";
  for (ChainIterator it(text.begin(), text.end(), trie); it != ChainIterator(); ++it) {
    switch (*it) {
      case 1: case 4: // "she" && "he" found, it must be position 4.
        CHECK(it.GetPos() == 4);
        if (it.GetPos() != 4) {
          ok = false;
        }
        if (*it == 1) {
          he_found = true;
        } else if (*it == 4) {
          she_found = true;
        }
        break;
      case 2: // "hers" found, it must be position 6.
        CHECK(it.GetPos() == 6);
        if (it.GetPos() != 6) {
          ok = false;
        }

        hers_found = true;
        break;
      default:
        CHECK(false);
        ok = false;
        break;
    }
  }
  CHECK( (he_found && she_found && hers_found) );
  if (!(he_found && she_found && hers_found)) {
    ok = false;
  }
  return ok;
}

TEST_CASE("Automata_AcTrie_Search ", "[single-file]") { REQUIRE(Automata_AcTrie_Search()); }
TEST_CASE("Automata_AcTrie_Serialize ", "[single-file]") { REQUIRE(Automata_AcTrie_Serialize()); }
TEST_CASE("Automata_AcIterator_Search ", "[single-file]") { REQUIRE(Automata_AcIterator_Search()); }
TEST_CASE("Automata_ChainIterator_Search ", "[single-file]") { REQUIRE(Automata_ChainIterator_Search()); }
