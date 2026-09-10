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

#ifndef ELFFS_EFS_PATH_UTILS_H
#define ELFFS_EFS_PATH_UTILS_H

#include <elffs/export_defs.h>
#include <elffs/efs_types.h>

ELFFS_BEGIN_CPP_COMPAT

typedef struct efs_path_lookup_pair_s
{
    char blob[EFS_BLOB_NAME_MAX];
    char path[EFS_ENTRY_PATH_MAX];
} *efs_path_lookup_pair_o, efs_path_lookup_pair_t;

ELFFS_API efs_bool_t efs_mb_path_split(efs_path_lookup_pair_o out, const char* efs_mb_path);

ELFFS_END_CPP_COMPAT


#endif //ELFFS_EFS_PATH_UTILS_H
