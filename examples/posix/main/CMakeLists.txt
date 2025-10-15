file(GLOB_RECURSE SRC *.cpp)
idf_component_register(SRCS ${SRC} INCLUDE_DIRS .)

idf_build_set_property(COMPILE_OPTIONS "-Wno-error=switch" APPEND)
