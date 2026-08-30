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

#ifndef ELFFS_EFS_UID_H
#define ELFFS_EFS_UID_H

#include <elffs/export_defs.h>
#include <elffs/efs_int_types.h>

ELFFS_BEGIN_CPP_COMPAT

typedef union ELFFS_NO_IMPLICIT_PADDING efs_uid_u
{
    struct ELFFS_NO_IMPLICIT_PADDING
    {
        uint8_t prefix;
        uint16_t body;
        uint8_t suffix;
    };
    uint32_t full_bytes;
} *efs_uid_o, efs_uid_t;

//_Static_assert(sizeof(efs_uid_t) == sizeof(uint32_t), "efs_uid_t is same size as uint32_t");;

ELFFS_API bool efs_uid_from_ui32(efs_uid_o uid, uint32_t value);

ELFFS_API bool efs_uid_cmp(efs_uid_o uid, efs_uid_o other);

ELFFS_API bool efs_uid_random(efs_uid_o uid);

ELFFS_API efs_uid_o efs_uid_copy(efs_uid_o uid, efs_uid_o out);

ELFFS_API uint16_t efs_uid_as_string(char* buff, size_t len, efs_uid_o uid);

ELFFS_END_CPP_COMPAT

#endif //ELFFS_EFS_UID_H
