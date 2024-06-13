include(GoogleTest)

file(GLOB_RECURSE SRC *.cpp)
add_executable(Z21Tests ${SRC})

if(CMAKE_SYSTEM_NAME STREQUAL Linux)
  sanitize(address,undefined)
endif()

target_common_warnings(Z21Tests PRIVATE)

cpmaddpackage(
  NAME
  googletest
  GITHUB_REPOSITORY
  "google/googletest"
  GIT_TAG
  main
  SYSTEM
  YES
  OPTIONS
  "INSTALL_GTEST OFF")

target_link_libraries(Z21Tests PRIVATE Z21::Z21 GTest::gtest_main GTest::gmock)

gtest_discover_tests(Z21Tests)
