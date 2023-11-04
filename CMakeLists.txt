cmake_minimum_required(VERSION 3.25 FATAL_ERROR)
include(FetchContent)

FetchContent_Declare(
  CMakeModules
  GIT_REPOSITORY https://github.com/ZIMO-Elektronik/CMakeModules
  GIT_TAG v0.2.2
  SOURCE_DIR ${CMAKE_BINARY_DIR}/CMakeModules)
FetchContent_MakeAvailable(CMakeModules)

# version_from_git()
project(
  Z21
  VERSION ${VERSION_FROM_GIT}
  LANGUAGES C CXX)

file(GLOB_RECURSE SRC src/*.cpp)
add_library(Z21 STATIC ${SRC})
add_library(Z21::Z21 ALIAS Z21)

target_compile_features(Z21 PUBLIC cxx_std_23)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  target_include_directories(
    Z21
    INTERFACE include
    PRIVATE include/z21)
else()
  target_include_directories(
    Z21 SYSTEM
    INTERFACE include
    PRIVATE include/z21)
endif()

target_common_warnings(Z21 PRIVATE)

if(NOT TARGET ZTL::ZTL)
  cpmaddpackage("gh:ZIMO-Elektronik/ZTL@0.18.0")
endif()

target_link_libraries(Z21 PUBLIC ZTL::ZTL)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  include(CTest)
endif()

if(BUILD_TESTING
   AND CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME
   AND CMAKE_SYSTEM_NAME STREQUAL CMAKE_HOST_SYSTEM_NAME)
  add_subdirectory(examples)
  add_subdirectory(tests)
endif()
