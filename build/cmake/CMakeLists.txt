# CMakeLists.txt

# Copyright (c) Mateusz Jandura. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.21)
project(fshred
    VERSION 1.0.1
    DESCRIPTION "File Shredder"
    LANGUAGES CXX
)

set(CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED ON)

# translate x64/Win32 into x64/x86
if(CMAKE_GENERATOR_PLATFORM STREQUAL x64)
    set(FSHRED_PLATFORM_ARCH x64)
elseif(CMAKE_GENERATOR_PLATFORM STREQUAL Win32)
    set(FSHRED_PLATFORM_ARCH x86)
else()
    set(FSHRED_PLATFORM_ARCH Invalid)
    message(FATAL_ERROR "Requires either x64 or Win32 platform architecture.")
endif()

set(CMAKE_SUPPRESS_REGENERATION TRUE)
if(MSVC)
    set(VS_SOURCE_GROUPS src)
endif()

set(FSHRED_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../src")
set(FSHRED_SOURCES
    "${FSHRED_SRC_DIR}/fshred/dialog.cpp"
    "${FSHRED_SRC_DIR}/fshred/dialog.hpp"
    "${FSHRED_SRC_DIR}/fshred/main.cpp"
    "${FSHRED_SRC_DIR}/fshred/program.cpp"
    "${FSHRED_SRC_DIR}/fshred/program.hpp"
    "${FSHRED_SRC_DIR}/fshred/random.cpp"
    "${FSHRED_SRC_DIR}/fshred/random.hpp"
    "${FSHRED_SRC_DIR}/fshred/shredder.cpp"
    "${FSHRED_SRC_DIR}/fshred/shredder.hpp"
    "${FSHRED_SRC_DIR}/fshred/tinywin.hpp"
    "${FSHRED_SRC_DIR}/fshred/utils.hpp"
)

# put all source files in "src" directory
source_group("src" FILES ${FSHRED_SOURCES})

# put the compiled executable in either "bin\Debug" or "bin\Release" directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}")

add_executable(fshred WIN32 ${FSHRED_SOURCES})

target_compile_features(fshred PRIVATE cxx_std_17)
target_include_directories(fshred PRIVATE
    "${FSHRED_SRC_DIR}"
    "${FSHRED_SRC_DIR}/thirdparty/MJFS/inc"
    "${FSHRED_SRC_DIR}/thirdparty/MJMEM/inc"
    "${FSHRED_SRC_DIR}/thirdparty/MJSTR/inc"
)
target_link_libraries(fshred PRIVATE
    # link MJFS module
    $<$<CONFIG:Debug>:${FSHRED_SRC_DIR}/thirdparty/MJFS/bin/${FSHRED_PLATFORM_ARCH}/Debug/mjfs.lib>
    $<$<CONFIG:Release>:${FSHRED_SRC_DIR}/thirdparty/MJFS/bin/${FSHRED_PLATFORM_ARCH}/Release/mjfs.lib>

    # link MJMEM module
    $<$<CONFIG:Debug>:${FSHRED_SRC_DIR}/thirdparty/MJMEM/bin/${FSHRED_PLATFORM_ARCH}/Debug/mjmem.lib>
    $<$<CONFIG:Release>:${FSHRED_SRC_DIR}/thirdparty/MJMEM/bin/${FSHRED_PLATFORM_ARCH}/Release/mjmem.lib>

    # link MJSTR module
    $<$<CONFIG:Debug>:${FSHRED_SRC_DIR}/thirdparty/MJSTR/bin/${FSHRED_PLATFORM_ARCH}/Debug/mjstr.lib>
    $<$<CONFIG:Release>:${FSHRED_SRC_DIR}/thirdparty/MJSTR/bin/${FSHRED_PLATFORM_ARCH}/Release/mjstr.lib>
)