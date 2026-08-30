#
# Copyright (c) 2026. Tetex7
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
#

add_library(libelffs_rt STATIC ${MASTR_INC}/elffs/rt/elffs_manager.h elffs_manager.c)
target_link_libraries(libelffs_rt PUBLIC libelffs)