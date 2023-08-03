# CMakeLists.txt

# Copyright (c) Mateusz Jandura. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.21)
project(fshred
    VERSION 1.0.0
    DESCRIPTION "File Shredder"
    LANGUAGES CXX
)

set(CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED ON)

if(NOT(CMAKE_GENERATOR_PLATFORM STREQUAL x64 OR CMAKE_GENERATOR_PLATFORM STREQUAL Win32))
    message(FATAL_ERROR "Requires either x64 or Win32 platform architecture.")
endif()

set(CMAKE_SUPPRESS_REGENERATION TRUE)
if(MSVC)
    set(VS_SOURCE_GROUPS src)
endif()

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../src")
set(FSHRED_SOURCES
    "${SRC_DIR}/fshred/dialog.cpp"
    "${SRC_DIR}/fshred/dialog.hpp"
    "${SRC_DIR}/fshred/file.cpp"
    "${SRC_DIR}/fshred/file.hpp"
    "${SRC_DIR}/fshred/main.cpp"
    "${SRC_DIR}/fshred/program.cpp"
    "${SRC_DIR}/fshred/program.hpp"
    "${SRC_DIR}/fshred/random.cpp"
    "${SRC_DIR}/fshred/random.hpp"
    "${SRC_DIR}/fshred/shredder.cpp"
    "${SRC_DIR}/fshred/shredder.hpp"
    "${SRC_DIR}/fshred/utils.hpp"
)

# put all source files in "src" directory
source_group("src" FILES ${FSHRED_SOURCES})

# put the compiled executable in either the "bin\Debug" or "bin\Release" directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}")

add_executable(fshred WIN32
    ${FSHRED_SOURCES}
)

target_compile_features(fshred PRIVATE cxx_std_17)
target_include_directories(fshred PRIVATE "${SRC_DIR}")