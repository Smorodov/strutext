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

set(UNIT_TEST_MODULE encode-unit-test)
set(UNIT_TESTS_SOURCES
  ${STRUTEXT_ROOT_SOURCE_DIR}/ut-data/ut_main.cpp
  utf8_iterator_test.cpp
  char_iterator_test.cpp
)

add_executable(${UNIT_TEST_MODULE} ${UNIT_TESTS_SOURCES})
target_link_libraries(${UNIT_TEST_MODULE}
  encode
  Catch2
  Catch2WithMain
)

#add_boost_tests("${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${UNIT_TEST_MODULE}" "--catch_system_errors=no" ${UNIT_TESTS_SOURCES})
