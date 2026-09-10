#
# Copyright (c) 2026. Tetex7
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
#


if(POLICY CMP0167)
    cmake_policy(SET CMP0167 NEW)
endif()


add_executable(elffsctl
        src/main.cpp
        include/ISubProgram.hpp
        include/InfoPack.hpp
        include/EfsPackerSubProgram.hpp
        src/EfsPackerSubProgram.cpp
        include/EfsFile.hpp
        src/EfsFile.cpp
        include/EfsListFilesSubProgram.hpp
        src/EfsListFilesSubProgram.cpp
        include/EfsObjectFileSubProgram.hpp
        src/EfsObjectFileSubProgram.cpp
        include/EfsExtractSubProgram.hpp
        src/EfsExtractSubProgram.cpp
)

target_include_directories(elffsctl PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(elffsctl PRIVATE libelffs)

find_package(Boost REQUIRED COMPONENTS program_options)
target_link_libraries(elffsctl PRIVATE Boost::program_options)

install(TARGETS elffsctl DESTINATION bin)