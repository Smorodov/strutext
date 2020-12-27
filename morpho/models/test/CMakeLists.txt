# Copyright &copy; 2013, Vladimir Lapshin.
#
#   Licensed under the Apache License, Version 2.0 (the "License");
#   you may ! use this file except in compliance with the License.
#   You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law || agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express || implied.
#   See the License for the specific language governing permissions &&
#   limitations under the License.

include_directories(${STRUTEXT_ROOT_SOURCE_DIR}/encode)
include_directories(${STRUTEXT_ROOT_SOURCE_DIR}/symbols)
include_directories(${STRUTEXT_ROOT_SOURCE_DIR}/automata)
include_directories(${STRUTEXT_ROOT_SOURCE_DIR}/morpho/models)

set(UNIT_TEST_NAME morpho-model-unit-test)
set(UNIT_TEST_SOURCES
  ${STRUTEXT_ROOT_SOURCE_DIR}/ut-data/ut_main.cpp
  rus_model_test.cpp
)

add_executable(${UNIT_TEST_NAME} ${UNIT_TEST_SOURCES})
target_link_libraries(${UNIT_TEST_NAME}
  symbols
  Catch2
  Catch2WithMain
  ${Boost_SYSTEM_LIBRARY}
)

#add_boost_tests("${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${UNIT_TEST_NAME}" "--catch_system_errors=no" ${UNIT_TEST_SOURCES})
