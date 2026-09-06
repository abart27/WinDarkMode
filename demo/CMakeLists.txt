cmake_minimum_required(VERSION 3.25)
project(WinDarkModeDemo LANGUAGES CXX RC)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> <DEFINES> <INCLUDES> <FLAGS> -O coff -o <OBJECT> <SOURCE>")
endif()

add_executable(WinDarkModeDemo WIN32
    main.cpp
    dialog.rc
)

target_include_directories(WinDarkModeDemo PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_compile_definitions(WinDarkModeDemo PRIVATE
    WIN32_LEAN_AND_MEAN
    NOMINMAX
    UNICODE
    _UNICODE
    _WIN32_WINNT=0x0A00
)


target_link_libraries(WinDarkModeDemo PRIVATE
    uxtheme
    comdlg32
    dwmapi
    comctl32
    gdi32
)

target_link_options(WinDarkModeDemo PRIVATE
    $<$<CXX_COMPILER_ID:GNU>:-municode>
)

target_compile_options(WinDarkModeDemo PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<CXX_COMPILER_ID:MSVC>:/WX>
    $<$<CXX_COMPILER_ID:MSVC>:/permissive->
    $<$<COMPILE_LANG_AND_ID:CXX,GNU,Clang,AppleClang>:-Wall;-Wextra;-Wpedantic>
)
