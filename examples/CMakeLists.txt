if(ESP_PLATFORM)
  # ESP32 example gets automatically included by component manager
  # https://docs.espressif.com/projects/idf-component-manager/en/latest/reference/manifest_file.html#examples
elseif(CMAKE_SYSTEM_NAME STREQUAL Linux OR CMAKE_SYSTEM_NAME STREQUAL Windows)
  # get_socket is common to all examples (but must be located in /posix/main for
  # ESP_PLATFORM)
  set(COMMON_INC ${CMAKE_CURRENT_LIST_DIR}/posix/main)
  file(GLOB_RECURSE COMMON_SRC ${COMMON_INC}/get_socket.cpp)
  add_subdirectory(posix)
  add_subdirectory(sim)
endif()
