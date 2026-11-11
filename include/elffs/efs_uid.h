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
        efs_u8_t prefix;
        efs_u16_t body;
        efs_u8_t suffix;
    };
    efs_u32_t full_bytes;
} *efs_uid_o, efs_uid_t;

ELFFS_STATIC_ASSERT(sizeof(efs_uid_t) == sizeof(efs_u32_t), "efs_uid_t is same size as uint32_t");;

ELFFS_API efs_result_t efs_uid_create(efs_uid_o uid, efs_u8_t prefix, efs_u16_t body, efs_u8_t suffix);

ELFFS_API efs_result_t efs_uid_from_u32(efs_uid_o uid, efs_u32_t value);

ELFFS_API efs_bool_t efs_uid_cmp(efs_uid_o uid, efs_uid_o other);

ELFFS_API efs_result_t efs_uid_random(efs_uid_o uid);

ELFFS_API efs_uid_o efs_uid_copy(efs_uid_o uid, efs_uid_o out);

ELFFS_API efs_uid_o efs_uid_from_string_hash(efs_uid_o out, const char* string);

ELFFS_API const char* efs_uid_as_string(char* buff, size_t len, efs_uid_o uid);

#ifdef ELFFS_ON_CLANG
ELFFS_API ELFFS_OVERLOAD
efs_result_t efs_uid_create(efs_uid_o uid)
{
    return efs_uid_random(uid);
}

ELFFS_API ELFFS_OVERLOAD
efs_result_t efs_uid_create(efs_uid_o uid, efs_u32_t full)
{
    return efs_uid_from_u32(uid, full);
}
#endif

ELFFS_END_CPP_COMPAT

#endif //ELFFS_EFS_UID_H
