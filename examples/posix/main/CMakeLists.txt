set(COMMON_INC ${CMAKE_CURRENT_LIST_DIR}/../../common)
file(GLOB_RECURSE COMMON_SRC ${COMMON_INC}/*.cpp)
file(GLOB_RECURSE SRC *.cpp)
idf_component_register(SRCS ${COMMON_SRC} ${SRC} INCLUDE_DIRS ${COMMON_INC} .)

idf_build_set_property(COMPILE_OPTIONS "-Wno-error=switch" APPEND)
