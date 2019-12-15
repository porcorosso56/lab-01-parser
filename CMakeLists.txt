cmake_minimum_required(VERSION 3.10.2:)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_COVERAGE "Build coverage" OFF)

set(
 HUNTER_CACHE_SERVERS
 "https://github.com/bmstu-iu8-cpp-sem-3/hunter-cache"
 CACHE STRING "Default cache server"
)

include("cmake/HunterGate.cmake")

huntergate(
 URL "https://github.com/ruslo/hunter/archive/v0.23.34.tar.gz"
 SHA1 "70287b1ffa810ee4e952052a9adff9b4856d0d54"
)

project(Json)

string(APPEND CMAKE_CXX_FLAGS " -pedantic -Werror -Wall -Wextra")
string(APPEND CMAKE_CXX_FLAGS " -Wno-unused-command-line-argument")
string(APPEND CMAKE_CXX_FLAGS " -Wshadow -Wnon-virtual-dtor")

hunter_add_package(GTest)
hunter_add_package(nlohmann_json)

find_package(GTest CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_library(Json STATIC
  ${CMAKE_CURRENT_SOURCE_DIR}/sources/Json.cpp )

add_executable(tests
  ${CMAKE_CURRENT_SOURCE_DIR}/tests/test.cpp)

add_executable(parser
  ${CMAKE_CURRENT_SOURCE_DIR}/sources/source.cpp)

add_executable(main 
  ${CMAKE_CURRENT_SOURCE_DIR}/sources/main.cpp)

target_include_directories(Json PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
 )


target_link_libraries(parser Json nlohmann_json::nlohmann_json) 
target_link_libraries(tests Json GTest::main)
target_link_libraries(main nlohmann_json::nlohmann_json)

enable_testing()
add_test(NAME unit_tests COMMAND tests) 

install(TARGETS Json
    EXPORT Json-config
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION include)
install(EXPORT Json-config DESTINATION cmake)

if(BUILD_COVERAGE)
  set(ENABLE_TESTS ON CACHE BOOL "Enable coverage build." FORCE)
  list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
  find_package(codecov)
  add_coverage(parser)
  add_coverage(tests)
  list(APPEND LCOV_REMOVE_PATTERNS "'${PROJECT_SOURCE_DIR}/tests/*'")
  coverage_evaluate()
endif()
