#
# Copyright (c) 2026. Tetex7
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
#

cmake_minimum_required(VERSION 4.3.4)
project(elffs C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 26)

#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

set(MASTR_INC ${CMAKE_CURRENT_SOURCE_DIR}/include)

if (NOT WIN32)
    set(CMAKE_SHARED_LIBRARY_PREFIX "")
endif ()


if (NOT MSVC)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)

    set(CMAKE_C_STANDARD_REQUIRED ON)
    set(CMAKE_C_EXTENSIONS OFF)
endif ()

function(enable_warnings target)
    if(MSVC)
        #target_compile_options(${target} PRIVATE /W4 /WX)
    else()
        target_compile_options(${target} PRIVATE
                -Wall
                -Wextra
                -Wpedantic
                -Werror
        )
    endif()
endfunction()

function(enable_api_ctl target)
    if (NOT WIN32)
        target_compile_options(${target} PRIVATE -fvisibility=hidden)
    endif ()
endfunction()

add_library(libelffs SHARED
        include/elffs/export_defs.h
        include/elffs/efs_types.h
        src/efs_uid.c
        include/elffs/efs_uid.h
        src/efs_types.c
        include/elffs/efs_int_types.h
        include/elffs/elffs_manager.h
        src/elffs_manager.c
        include/elffs/efs_path_utils.h
        src/efs_path_utils.c
)
enable_api_ctl(libelffs)
target_compile_definitions(libelffs PRIVATE ELFFS_EXPORTS)
target_include_directories(libelffs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(libelffs PUBLIC c_std_99)

install(TARGETS libelffs DESTINATION lib)

add_subdirectory(elffsctl)
#target_include_directories(elffsctl PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
add_subdirectory(rt)
add_subdirectory(tester)


# --- CPack Configuration ---
set(CPACK_PACKAGE_NAME "elffs")
set(CPACK_PACKAGE_VENDOR "trs")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A brief description of my application")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})

# Specify default generators (optional, defaults based on OS if omitted)
set(CPACK_GENERATOR "TGZ;ZIP;DEB")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Name <email@example.com>")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.27)")

# Always include CPack at the very end of the file
include(CPack)