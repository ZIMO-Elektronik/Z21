if(ESP_PLATFORM)
  # ESP32 example gets automatically included by component manager
  # https://docs.espressif.com/projects/idf-component-manager/en/latest/reference/manifest_file.html#examples
elseif(CMAKE_SYSTEM_NAME STREQUAL Linux OR CMAKE_SYSTEM_NAME STREQUAL Windows)
  set(COMMON_INC ${CMAKE_CURRENT_LIST_DIR}/common)
  file(GLOB_RECURSE COMMON_SRC ${COMMON_INC}/*.cpp)
  add_subdirectory(posix)
  add_subdirectory(sim)
endif()
