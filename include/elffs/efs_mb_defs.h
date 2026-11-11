/*
 * Copyright (c) 2026. Tetex7
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

//
// Created by tete on 9/13/26.
//

#ifndef ELFFS_EFS_MB_DEFS_H
#define ELFFS_EFS_MB_DEFS_H

#include <elffs/efs_types.h>

ELFFS_BEGIN_CPP_COMPAT

typedef struct elffs_mb_blob_s
{
    const efs_size_t offset_in_cluster;
    const efs_size_t size;
} elffs_mb_blob_t;

ELFFS_END_CPP_COMPAT

#endif //ELFFS_EFS_MB_DEFS_H
