cmake_minimum_required(VERSION 3.25 FATAL_ERROR)
include(FetchContent)

if(ESP_PLATFORM)
  idf_component_register(INCLUDE_DIRS include REQUIRES lwip)
  target_link_libraries(${COMPONENT_LIB} INTERFACE Z21)
endif()

FetchContent_Declare(
  CMakeModules
  GIT_REPOSITORY "https://github.com/ZIMO-Elektronik/CMakeModules"
  GIT_TAG v0.9.2
  SOURCE_DIR ${CMAKE_BINARY_DIR}/CMakeModules)
FetchContent_MakeAvailable(CMakeModules)

version_from_git()
project(
  Z21
  VERSION ${VERSION_FROM_GIT}
  LANGUAGES C CXX)

set(Z21_MAX_PAYLOAD_SIZE
    1472uz
    CACHE STRING "Maximum size of a payload in bytes")
set(Z21_MAX_DATASET_SIZE
    63uz
    CACHE STRING "Maximum size of a dataset in bytes")
set(Z21_SERVER_DEQUE_SIZE
    31uz
    CACHE STRING "Size of the receiver deque of server")
set(Z21_SERVER_MAX_LOCO_ADDRESSES
    256uz
    CACHE STRING "Maximum number of locomotive addresses of server")
set(Z21_SERVER_MAX_ACCESSORY_ADDRESSES
    256uz
    CACHE STRING "Maximum number of accessory addresses of server")
set(Z21_SERVER_MAX_LOCO_ADDRESSES_PER_CLIENT
    16uz
    CACHE STRING "Maximum number of subscribed locomotive addresses per client")

add_library(Z21 INTERFACE)
add_library(Z21::Z21 ALIAS Z21)

target_compile_features(Z21 INTERFACE cxx_std_23)

target_compile_definitions(
  Z21
  INTERFACE
    Z21_VERSION_MAJOR=0x01
    Z21_VERSION_MINOR=0x43
    Z21_XBUS_VERSION=0x40
    Z21_COMMAND_STATION_ID=0x12
    Z21_MAX_PAYLOAD_SIZE=${Z21_MAX_PAYLOAD_SIZE}
    Z21_MAX_DATASET_SIZE=${Z21_MAX_DATASET_SIZE}
    Z21_SERVER_DEQUE_SIZE=${Z21_SERVER_DEQUE_SIZE}
    Z21_SERVER_MAX_LOCO_ADDRESSES=${Z21_SERVER_MAX_LOCO_ADDRESSES}
    Z21_SERVER_MAX_ACCESSORY_ADDRESSES=${Z21_SERVER_MAX_ACCESSORY_ADDRESSES}
    Z21_SERVER_MAX_LOCO_ADDRESSES_PER_CLIENT=${Z21_SERVER_MAX_LOCO_ADDRESSES_PER_CLIENT}
)

if(PROJECT_IS_TOP_LEVEL)
  target_include_directories(Z21 INTERFACE include)
else()
  target_include_directories(Z21 SYSTEM INTERFACE include)
endif()

target_common_warnings(Z21 INTERFACE)

if(NOT TARGET ZTL::ZTL)
  cpmaddpackage("gh:ZIMO-Elektronik/ZTL@0.19.0")
endif()

target_link_libraries(Z21 INTERFACE ZTL::ZTL)

if(PROJECT_IS_TOP_LEVEL)
  include(CTest)
  add_subdirectory(examples)
  file(
    DOWNLOAD
    "https://github.com/ZIMO-Elektronik/.github/raw/master/data/.clang-format"
    ${CMAKE_CURRENT_LIST_DIR}/.clang-format)
  file(GLOB_RECURSE SRC examples/*.[ch]pp include/*.[ch]pp src/*.[ch]pp
       tests/*.[ch]pp)
  add_clang_format_target(Z21Format OPTIONS -i FILES ${SRC})
endif()

if(BUILD_TESTING AND CMAKE_SYSTEM_NAME STREQUAL CMAKE_HOST_SYSTEM_NAME)
  add_subdirectory(tests)
endif()
