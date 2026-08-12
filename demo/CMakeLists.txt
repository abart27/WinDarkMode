cmake_minimum_required(VERSION 3.25)
project(WinDarkModeDemo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT MSVC)
    message(FATAL_ERROR "WinDarkModeDemo requires MSVC.")
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
    _WIN32_WINNT=0x0A00
)

target_link_options(WinDarkModeDemo PRIVATE /MANIFEST:NO)
set_target_properties(WinDarkModeDemo PROPERTIES
    VS_GLOBAL_GenerateManifest "false"
)

target_link_libraries(WinDarkModeDemo PRIVATE
    UxTheme
    Comdlg32
    Dwmapi
    Comctl32
)

target_compile_options(WinDarkModeDemo PRIVATE
    /W4
    /WX
    /permissive-
)
