cmake_minimum_required(VERSION 3.25 FATAL_ERROR)
include(FetchContent)

if(ESP_PLATFORM)
  idf_component_register(INCLUDE_DIRS include REQUIRES lwip)
  target_link_libraries(${COMPONENT_LIB} INTERFACE Z21)
endif()

FetchContent_Declare(
  CMakeModules
  GIT_REPOSITORY "https://github.com/ZIMO-Elektronik/CMakeModules"
  GIT_TAG v0.9.5
  SOURCE_DIR ${CMAKE_BINARY_DIR}/CMakeModules)
FetchContent_MakeAvailable(CMakeModules)

version_from_git()
project(
  Z21
  VERSION ${VERSION_FROM_GIT}
  LANGUAGES C CXX)

set(Z21_FIRMWARE_VERSION_MAJOR
    0x01u
    CACHE STRING "Firmware major version in BCD format")
set(Z21_FIRMWARE_VERSION_MINOR
    0x43u
    CACHE STRING "Firmware minor version in BCD format")
set(Z21_XBUS_PROTOCOL_VERSION
    0x40u
    CACHE STRING
          "X-Bus protocol version (0x30 = V3.0, 0x36 = V3.6, 0x40 = V4.0, … )")
set(Z21_COMMAND_STATION_ID
    0x12u
    CACHE STRING "Command station ID (0x12 = Z21 device family)")
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
    Z21_FIRMWARE_VERSION_MAJOR=${Z21_FIRMWARE_VERSION_MAJOR}
    Z21_FIRMWARE_VERSION_MINOR=${Z21_FIRMWARE_VERSION_MINOR}
    Z21_XBUS_PROTOCOL_VERSION=${Z21_XBUS_PROTOCOL_VERSION}
    Z21_COMMAND_STATION_ID=${Z21_COMMAND_STATION_ID}
    Z21_MAX_PAYLOAD_SIZE=${Z21_MAX_PAYLOAD_SIZE}
    Z21_MAX_DATASET_SIZE=${Z21_MAX_DATASET_SIZE}
    Z21_SERVER_DEQUE_SIZE=${Z21_SERVER_DEQUE_SIZE}
    Z21_SERVER_MAX_LOCO_ADDRESSES=${Z21_SERVER_MAX_LOCO_ADDRESSES}
    Z21_SERVER_MAX_ACCESSORY_ADDRESSES=${Z21_SERVER_MAX_ACCESSORY_ADDRESSES}
    Z21_SERVER_MAX_LOCO_ADDRESSES_PER_CLIENT=${Z21_SERVER_MAX_LOCO_ADDRESSES_PER_CLIENT}
)

# https://github.com/espressif/esp-idf/issues/17773
if(PROJECT_IS_TOP_LEVEL AND NOT ESP_PLATFORM)
  target_include_directories(Z21 INTERFACE include)
  target_common_warnings(Z21 INTERFACE)
else()
  target_include_directories(Z21 SYSTEM INTERFACE include)
endif()

if(NOT TARGET ZTL::ZTL)
  cpmaddpackage("gh:ZIMO-Elektronik/ZTL@0.23.1")
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

if(BUILD_TESTING
   AND PROJECT_IS_TOP_LEVEL
   AND CMAKE_SYSTEM_NAME STREQUAL CMAKE_HOST_SYSTEM_NAME)
  add_subdirectory(tests)
endif()
