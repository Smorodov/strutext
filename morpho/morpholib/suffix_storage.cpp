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
 * \brief  Word suffices storage implementation.
 * \author Vladimir Lapshin.
 */

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/vector.hpp>

#include "suffix_storage.h"

namespace strutext { namespace morpho {

void SuffixStorage::Serialize(std::ostream& os) const {
  boost::archive::text_oarchive oa(os);
  oa << *this;
}

void SuffixStorage::Deserialize(std::istream& is) {
  boost::archive::text_iarchive ia(is);
  ia >> *this;
}

}} // namespace strutext, morpho.
