file(GLOB_RECURSE SRC *.c *.cpp)
add_executable(Z21Examples ${SRC})

sanitize("address,undefined")

target_common_warnings(Z21Examples PRIVATE)

cpmaddpackage("gh:daniele77/cli@2.1.0")

target_link_libraries(Z21Examples PRIVATE cli::cli Z21::Z21)
