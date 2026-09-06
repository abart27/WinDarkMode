#[===[
Copyright (c) 2026, WinDarkMode contributors.

SPDX-License-Identifier: GPL-3.0-or-later
]===]

# Toolchain file for MinGW cross-compilation from Linux to Windows.
#
# The compiler can be selected by a CMake preset or supplied manually:
#
#   cmake -S demo -B demo/build/mingw64 \
#         -G Ninja \
#         -DCMAKE_TOOLCHAIN_FILE=demo/toolchains/mingw-cross.cmake \
#         -DMINGW_TARGET=x86_64 \
#         -DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc \
#         -DCMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++
#
# Install the MinGW toolchain provided by your distribution. On Debian/Ubuntu:
#
#   sudo apt install g++-mingw-w64-x86-64  # 64-bit
#   sudo apt install g++-mingw-w64-i686   # 32-bit

set(CMAKE_SYSTEM_NAME Windows)

# ---- architecture selection --------------------------------------------------
if(NOT DEFINED MINGW_TARGET)
    set(MINGW_TARGET "x86_64" CACHE STRING "MinGW target (x86_64 or i686)")
endif()

if(MINGW_TARGET STREQUAL "x86_64")
    set(CMAKE_SYSTEM_PROCESSOR x86_64 CACHE STRING "" FORCE)
    set(_mingw_prefix "x86_64-w64-mingw32")
elseif(MINGW_TARGET STREQUAL "i686")
    set(CMAKE_SYSTEM_PROCESSOR x86 CACHE STRING "" FORCE)
    set(_mingw_prefix "i686-w64-mingw32")
else()
    message(FATAL_ERROR "Invalid MINGW_TARGET '${MINGW_TARGET}'. Use x86_64 or i686.")
endif()

# ---- resource compiler -------------------------------------------------------
find_program(CMAKE_RC_COMPILER
    NAMES "${_mingw_prefix}-windres"
    REQUIRED
    DOC "MinGW resource compiler"
)

# ---- sysroot / find-root-path ------------------------------------------------
# Try to locate the MinGW sysroot automatically.
execute_process(
    COMMAND "${CMAKE_C_COMPILER}" -print-sysroot
    OUTPUT_VARIABLE _mingw_sysroot
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
)
if(_mingw_sysroot AND EXISTS "${_mingw_sysroot}")
    set(CMAKE_SYSROOT "${_mingw_sysroot}" CACHE INTERNAL "")
    set(ENV{PKG_CONFIG_SYSROOT_DIR} "${_mingw_sysroot}")
endif()

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
