include(GoogleTest)

file(GLOB_RECURSE SRC *.cpp)
add_executable(Z21Tests ${SRC})

sanitize("address,undefined")

target_common_warnings(Z21Tests PRIVATE)

cpmaddpackage("gh:google/googletest#main")

target_link_libraries(Z21Tests PRIVATE Z21::Z21 GTest::gtest_main GTest::gmock)

gtest_discover_tests(Z21Tests)
