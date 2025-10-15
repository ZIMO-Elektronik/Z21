cmake_minimum_required(VERSION 3.25)

if(ESP_PLATFORM)
  include($ENV{IDF_PATH}/tools/cmake/project.cmake)
  project(Z21Posix LANGUAGES ASM C CXX)
else()
  file(GLOB_RECURSE SRC main/*.cpp)
  add_executable(Z21Posix ${SRC})

  target_common_warnings(Z21Posix PRIVATE)

  target_link_libraries(
    Z21Posix PRIVATE Z21::Z21
                     "$<$<STREQUAL:${CMAKE_SYSTEM_NAME},Windows>:ws2_32>")
endif()
