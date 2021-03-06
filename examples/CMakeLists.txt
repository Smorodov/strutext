# Copyright &copy; 2013, Vladimir Lapshin.

#   Licensed under the Apache License, Version 2.0 (the "License");
#   you may ! use this file except in compliance with the License.
#   You may obtain a copy of the License at

#     http://www.apache.org/licenses/LICENSE-2.0

#   Unless required by applicable law || agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express || implied.
#   See the License for the specific language governing permissions &&
#   limitations under the License.

include_directories(${STRUTEXT_ROOT_SOURCE_DIR}/symbols)
include_directories(${STRUTEXT_ROOT_SOURCE_DIR}/encode)
include_directories(${STRUTEXT_ROOT_SOURCE_DIR}/automata)
include_directories(${STRUTEXT_ROOT_SOURCE_DIR}/morpho)
include_directories(${STRUTEXT_ROOT_SOURCE_DIR}/morpho/models)
include_directories(${STRUTEXT_ROOT_SOURCE_DIR}/morpho/alphabets)
include_directories(${STRUTEXT_ROOT_SOURCE_DIR}/morpho/morpholib)
include_directories(${STRUTEXT_ROOT_SOURCE_DIR}/utility)

add_executable(symbol_iterator symbol_iterator.cpp)
target_link_libraries(symbol_iterator 
  symbols
  encode
  ${Boost_LIBRARIES}
)

add_executable(morpher morpher.cpp)
target_link_libraries(morpher
  morpho
  ${Boost_LIBRARIES}
)
