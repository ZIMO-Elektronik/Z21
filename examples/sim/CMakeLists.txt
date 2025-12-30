option(Z21_SIM_HIDE_CONFIDENTIAL_SETTINGS
       "Hide settings not available in the Z21 app or maintenance tool" ON)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 23)
set(QT_VERSION 6.6.3 CACHE STRING "Preferred Qt6 version")
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION OFF)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTORCC_OPTIONS "-compress-algo;zlib" "--compress;9")

file(GLOB_RECURSE SRC src/*.cpp)
add_executable(Z21Sim WIN32 ${COMMON_SRC} ${SRC})

target_compile_definitions(
  Z21Sim
  PRIVATE
    Z21_SIM_VERSION="${PROJECT_VERSION}"
    "$<$<BOOL:${Z21_SIM_HIDE_CONFIDENTIAL_SETTINGS}>:Z21_SIM_HIDE_CONFIDENTIAL_SETTINGS>"
)

target_include_directories(Z21Sim PRIVATE ${COMMON_INC})

if(CMAKE_SYSTEM_NAME STREQUAL Linux AND CMAKE_BUILD_TYPE STREQUAL Debug)
  sanitize(address,undefined)
endif()

target_common_warnings(Z21Sim PRIVATE)
target_common_errors(Z21Sim PRIVATE -Wno-shadow)

if(CMAKE_BUILD_TYPE STREQUAL Release)
  if(CMAKE_SYSTEM_NAME STREQUAL Linux)
    build_qt(
      ${QT_VERSION}
      MODULES
      qtbase
      qtsvg
      CMAKE_OPTIONS
      -DCMAKE_BUILD_TYPE=Release
      -DINPUT_optimize_size=ON
      -DBUILD_SHARED_LIBS=ON
      -DBUILD_WITH_PCH=ON
      -DCMAKE_INTERPROCEDURAL_OPTIMIZATION=${CMAKE_INTERPROCEDURAL_OPTIMIZATION}
      # Set C_STANDARD and CXX_STANDARD
      -DFEATURE_c11=ON
      -DFEATURE_cxx20=ON
      -DFEATURE_cxx2b=ON
      # Disable features that are not needed
      -DFEATURE_system_proxies=OFF
      -DFEATURE_system_doubleconversion=OFF
      -DFEATURE_system_freetype=OFF
      -DFEATURE_system_harfbuzz=OFF
      -DFEATURE_system_libb2=OFF
      -DFEATURE_system_pcre2=OFF
      # Enable freetype to get some sort of font support
      -DFEATURE_freetype=ON
      # Default to xcb and disable all other platform plugins
      -DQT_QPA_DEFAULT_PLATFORM=xcb
      -DFEATURE_linuxfb=OFF
      -DFEATURE_vkkhrdisplay=OFF
      -DFEATURE_vnc=OFF
      -DFEATURE_zstd=OFF
      # No FEATURE flags yet
      -DINPUT_libjpeg=no
      -DINPUT_libmd4c=no)
  elseif(CMAKE_SYSTEM_NAME STREQUAL Windows)
    # Install Qt6 for host using https://github.com/miurahr/aqtinstall
    find_package(Python3 REQUIRED COMPONENTS Interpreter)
    execute_process(
      COMMAND
        ${Python3_EXECUTABLE} -m aqt install-qt --outputdir
        ${CMAKE_BINARY_DIR}/qt6-host-build linux desktop ${QT_VERSION} gcc_64
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR} COMMAND_ERROR_IS_FATAL ANY)

    # Cross-compile Qt6 with mingw-w64
    build_qt(
      ${QT_VERSION}
      MODULES
      qtbase
      qtsvg
      CMAKE_OPTIONS
      -DQT_HOST_PATH=${CMAKE_BINARY_DIR}/qt6-host-build/${QT_VERSION}/gcc_64
      -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
      -DCMAKE_BUILD_TYPE=Release
      -DINPUT_optimize_size=ON
      -DBUILD_SHARED_LIBS=ON
      -DCMAKE_INTERPROCEDURAL_OPTIMIZATION=${CMAKE_INTERPROCEDURAL_OPTIMIZATION}
      # Set C_STANDARD and CXX_STANDARD
      -DFEATURE_c11=ON
      -DFEATURE_cxx20=ON
      -DFEATURE_cxx2b=ON
      # Disable features that are not needed
      -DFEATURE_system_proxies=OFF
      -DFEATURE_system_doubleconversion=OFF
      -DFEATURE_system_freetype=OFF
      -DFEATURE_system_harfbuzz=OFF
      -DFEATURE_system_libb2=OFF
      -DFEATURE_system_pcre2=OFF
      # Enable freetype to get some sort of font support
      -DFEATURE_freetype=ON
      # No FEATURE flags yet
      -DINPUT_libjpeg=no
      -DINPUT_libmd4c=no)
  endif()
endif()

find_qt(REQUIRED COMPONENTS Network Widgets)

qt_add_resources(
  Z21Sim
  z21sim_fonts
  PREFIX
  fonts
  BASE
  ${CMAKE_SOURCE_DIR}/data/fonts
  FILES
  ${CMAKE_SOURCE_DIR}/data/fonts/DejaVuSansMono.ttf)

qt_add_resources(
  Z21Sim
  z21sim_icons
  PREFIX
  icons
  BASE
  ${CMAKE_SOURCE_DIR}/data/icons
  FILES
  ${CMAKE_SOURCE_DIR}/data/icons/accessory.svg
  ${CMAKE_SOURCE_DIR}/data/icons/loco.svg
  ${CMAKE_SOURCE_DIR}/data/icons/turnout.svg)

qt_add_resources(
  Z21Sim
  z21sim_images
  PREFIX
  images
  BASE
  ${CMAKE_SOURCE_DIR}/data/images
  FILES
  ${CMAKE_SOURCE_DIR}/data/images/logo.png)

if(NOT TARGET Microsoft.GSL::GSL)
  cpmaddpackage("gh:microsoft/GSL@4.2.0")
endif()

if(NOT TARGET Qt::BreezeIcons)
  cpmaddpackage("gh:ZIMO-Elektronik/QtBreeze@5.115.0")
endif()

target_link_libraries(Z21Sim PRIVATE Microsoft.GSL::GSL Qt::BreezeStyleSheets
                                     Qt::Core Qt::Network Qt::Widgets Z21::Z21)

# Deploy release builds with CQtDeployer
if(CMAKE_BUILD_TYPE STREQUAL Release)
  # Output directory of .zip files
  set(TARGET_DIR ${CMAKE_BINARY_DIR}/DistributionKit)

  find_package(CQtDeployer 1.6.2351 COMPONENTS Linux)

  if(CMAKE_SYSTEM_NAME STREQUAL Linux)
    file(GLOB_RECURSE QMAKE_EXECUTABLE ${QT_BINARY_DIR}/*/qmake6)

    # Qt libs path
    execute_process(
      COMMAND ${QMAKE_EXECUTABLE} -query QT_INSTALL_LIBS
      OUTPUT_VARIABLE LIB_DIR
      OUTPUT_STRIP_TRAILING_WHITESPACE)

    # Qt plugins path
    execute_process(
      COMMAND ${QMAKE_EXECUTABLE} -query QT_INSTALL_PLUGINS
      OUTPUT_VARIABLE QT_INSTALL_PLUGINS
      OUTPUT_STRIP_TRAILING_WHITESPACE)
    file(GLOB EXTRA_PLUGIN ${QT_INSTALL_PLUGINS}/*)
    list(JOIN EXTRA_PLUGIN "," EXTRA_PLUGIN)

    add_custom_command(
      TARGET Z21Sim
      POST_BUILD
      COMMAND
        ${CQTDEPLOYER_EXECUTABLE} #
        force-clear #
        noTranslations #
        -bin $<TARGET_FILE:Z21Sim> #
        -qmake ${QMAKE_EXECUTABLE} #
        -libDir ${LIB_DIR} #
        -extraLibs xcb-cursor #
        -extraPlugin ${EXTRA_PLUGIN} #
        -recursiveDepth 2 #
        -targetDir ${TARGET_DIR} #
        -verbose 3 #
        -platform linux_x86_64 #
    )

  elseif(CMAKE_SYSTEM_NAME STREQUAL Windows)
    file(GLOB_RECURSE QMAKE_EXECUTABLE ${QT_BINARY_DIR}/*/qmake)

    # For Windows we need Qt and system libs, so use CMAKE_FIND_ROOT_PATH
    list(JOIN CMAKE_FIND_ROOT_PATH "," LIB_DIR)

    add_custom_command(
      TARGET Z21Sim
      POST_BUILD
      COMMAND
        ${CQTDEPLOYER_EXECUTABLE} #
        force-clear #
        noTranslations #
        -bin $<TARGET_FILE:Z21Sim> #
        -qmake ${QMAKE_EXECUTABLE} #
        -libDir ${LIB_DIR} #
        -recursiveDepth 2 #
        -targetDir ${TARGET_DIR} #
        -verbose 3 #
        -platform win_x86_64 #
    )
  endif()

  add_custom_command(
    TARGET Z21Sim
    POST_BUILD
    COMMAND
      ${CMAKE_COMMAND} -E tar "cf"
      Z21Sim-${PROJECT_VERSION}-${CMAKE_SYSTEM_NAME}-x64.zip --format=zip *
    WORKING_DIRECTORY ${TARGET_DIR})
endif()
