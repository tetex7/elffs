/*
 * Copyright (c) 2026. Tetex7
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

//
// Created by tete on 9/10/26.
//

#ifndef ELFFS_EFS_STREAMING_MANAGER_H
#define ELFFS_EFS_STREAMING_MANAGER_H

#include <elffs/efs_types.h>
#include <stdio.h>

#include <elffs/efsFile.h>

ELFFS_BEGIN_CPP_COMPAT

typedef struct efs_streaming_manager_s
{
    FILE* bin;
    efs_header_t header;
    efs_entry_t* entries;
} *efs_streaming_manager_o, efs_streaming_manager_t;

ELFFS_API efs_bool_t efs_streaming_manager_init(efs_streaming_manager_o self, const char* file_path);
ELFFS_API void efs_streaming_manager_uninit(efs_streaming_manager_o self);

ELFFS_API efs_streaming_manager_o efs_streaming_manager_new(const char* file_path);
ELFFS_API void efs_streaming_manager_destroy(efs_streaming_manager_o self);

ELFFS_API efsFile* efs_streaming_manager_fopen(efs_streaming_manager_o self, const char* file_path);

ELFFS_END_CPP_COMPAT

#endif //ELFFS_EFS_STREAMING_MANAGER_H
