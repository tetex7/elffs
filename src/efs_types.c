/*
 * Copyright (c) 2026. Tetex7
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

//
// Created by tete on 8/23/26.
//
#include <elffs/efs_types.h>

static const char* name_tp[] = {
    [EFS_ENTRY_TYPE_NONE] = "EFS_ENTRY_TYPE_NONE",
    [EFS_ENTRY_TYPE_BLOB] = "EFS_ENTRY_TYPE_BLOB",
    [EFS_ENTRY_TYPE_CLUSTER] = "EFS_ENTRY_TYPE_CLUSTER",
};

ELFFS_API const char* efs_entry_type_to_string(efs_entry_type_t type)
{
    const char* result = name_tp[type];

    return result;
}