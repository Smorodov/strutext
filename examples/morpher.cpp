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
 * \brief  Russian morpho analyser using example.
 * \author Vladimir Lapshin.
 */

#include <fstream>
#include <stdexcept>
#include <string>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/trim.hpp>

#include "rus_model.h"
#include "rus_alphabet.h"
#include "rus_model_description.h"
#include "morpho.h"

int main(int argc, char* argv[]) {
  setlocale(LC_ALL, "ru_RU.UTF8");
  std::locale::global(std::locale("ru_RU.UTF8"));

  try {
    using Morpher = strutext::morpho::Morphologist<strutext::morpho::RussianAlphabet>;
    using RussianPos = strutext::morpho::RussianPos;
    using PosSerializer = strutext::morpho::russian::PosSerializer;
    using RussianPosDecription = strutext::morpho::RussianPosDecription;

    std::string dict_path="./data/rus_dict.bin";

    // проверяем входные данные
    if (! boost::filesystem::exists(dict_path)) {
      throw std::invalid_argument("Dictionary file does not exist");
    }

    // инициализируем морфологический анализатор
    std::ifstream dict(dict_path.c_str(), std::ios::binary);
    if (! dict.is_open()) {
      std::stringstream err;
      err << "Can not open russian dictionary: " << dict_path;
      throw std::invalid_argument(err.str().c_str());
    }
    Morpher morpher;
    morpher.Deserialize(dict);
    strutext::morpho::MorphologistBase::LemList lemm_list;
    std::string word = "слушающий";
    morpher.Analize(word, lemm_list);
    std::cout << lemm_list.size();
    std::cout << word << ": {";
    unsigned idx = 0;
    for (const auto& lemma : lemm_list) {
      RussianPos::Ptr pos = PosSerializer::Deserialize(lemma.attr_);
      std::cout << RussianPosDecription::GetDecription(pos);
      if (++idx < lemm_list.size()) {
        std::cout << " | ";
      }
    }
    std::cout << "}\n";

  } catch (std::exception &ex) {
    std::cerr << "Error: " << ex.what() << std::endl;
    return 1;
    
  }
  getchar();
  return 0;
}

