cmake_minimum_required(VERSION 3.25)
project(WinDarkModeDemo LANGUAGES CXX RC)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#[=======================[
Copies MinGW runtime DLLs (libgcc, libstdc++, libwinpthread) to the target's
output directory. Only active when cross-compiling with MinGW.
]=======================]
function(copy_mingw_runtime target)
    if(NOT MINGW)
        return()
    endif()

    # Derive the MinGW prefix from the compiler name.
    # e.g. /bin/x86_64-w64-mingw32-g++ -> x86_64-w64-mingw32
    get_filename_component(_compiler_name "${CMAKE_CXX_COMPILER}" NAME)
    string(REGEX REPLACE "-g\\+\\+.*$" "" _mingw_prefix "${_compiler_name}")
    set(_mingw_bindir "/usr/${_mingw_prefix}/bin")

    add_custom_command(
        TARGET "${target}" POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different
        "${_mingw_bindir}/libgcc_s_seh-1.dll"
        "${_mingw_bindir}/libstdc++-6.dll"
        "${_mingw_bindir}/libwinpthread-1.dll"
        "$<TARGET_FILE_DIR:${target}>"
        VERBATIM
    )
endfunction()

#[=======================[
Copies MSVC runtime DLLs (msvcp140, msvcp140_atomic_wait, vcruntime140) to the
target's output directory. Needed on Windows 10 where these aren't pre-installed.
]=======================]
function(copy_msvc_runtime target)
    if(NOT (WIN32 AND NOT MINGW))
        return()
    endif()

    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(_msvc_arch "x64")
    else()
        set(_msvc_arch "x86")
    endif()

    get_filename_component(_compiler_dir "${CMAKE_CXX_COMPILER}" DIRECTORY)
    set(_candidates "${_compiler_dir}")

    if(DEFINED ENV{VCToolsInstallDir} AND NOT "$ENV{VCToolsInstallDir}" STREQUAL "")
        list(APPEND _candidates "$ENV{VCToolsInstallDir}/bin/Hostx64/${_msvc_arch}")
    endif()

    get_filename_component(_tools_dir "${_compiler_dir}/../../.." ABSOLUTE)
    file(GLOB _msvc_versions "${_tools_dir}/MSVC/*")
    foreach(_ver IN LISTS _msvc_versions)
        list(APPEND _candidates "${_ver}/bin/Hostx64/${_msvc_arch}")
    endforeach()

    set(_msvc_bindir "")
    foreach(_dir IN LISTS _candidates)
        if(EXISTS "${_dir}/msvcp140.dll")
            set(_msvc_bindir "${_dir}")
            break()
        endif()
    endforeach()

    if(NOT _msvc_bindir)
        message(WARNING "copy_msvc_runtime: could not locate the MSVC runtime DLLs (msvcp140.dll, msvcp140_atomic_wait.dll, vcruntime140.dll). Skipping the copy.")
        return()
    endif()

    add_custom_command(
        TARGET "${target}" POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different
        "${_msvc_bindir}/msvcp140.dll"
        "${_msvc_bindir}/msvcp140_atomic_wait.dll"
        "${_msvc_bindir}/vcruntime140.dll"
        "$<TARGET_FILE_DIR:${target}>"
        VERBATIM
    )
endfunction()

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

copy_mingw_runtime(WinDarkModeDemo)
copy_msvc_runtime(WinDarkModeDemo)
