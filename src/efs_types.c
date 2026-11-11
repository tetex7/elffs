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
#include <string.h>
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

ELFFS_API efs_bool_t efs_header_magic_check(const efs_byte_t (*test_magic)[EFS_MAGIC_LEN])
{
    if (!test_magic)
        return false;

    static const char currect[EFS_MAGIC_LEN] = {
        EFS_MAGIC_BYTE_0,
        EFS_MAGIC_BYTE_1,
        EFS_MAGIC_BYTE_2,
        EFS_MAGIC_BYTE_3,
    };
    return memcmp(*test_magic, currect, EFS_MAGIC_LEN) == 0;
}
