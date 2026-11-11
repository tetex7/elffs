/*
 * Copyright (c) 2026. Tetex7
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

//
// Created by tete on 8/13/26.
//

#include "elffs/efs_types.h"

#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

ELFFS_API efs_result_t efs_uid_create(efs_uid_o uid, efs_u8_t prefix, efs_u16_t body, efs_u8_t suffix)
{
    if (uid == NULL)
        return EFS_RESULT_ERROR;

    memset(uid, 0, sizeof(efs_uid_t));
    uid->prefix = prefix;
    uid->body = body;
    uid->suffix = suffix;

    return EFS_RESULT_SUCCESS;
}

ELFFS_API efs_result_t efs_uid_from_u32(efs_uid_o uid, efs_u32_t value)
{
    if (uid == NULL)
        return EFS_RESULT_ERROR;
    memset(uid, 0, sizeof(efs_uid_t));
    memcpy(uid, &value, sizeof(efs_uid_t));
    return EFS_RESULT_SUCCESS;
}

ELFFS_API efs_bool_t efs_uid_cmp(efs_uid_o uid, efs_uid_o other)
{
    if (uid == NULL || other == NULL)
        return false;

    if (uid == other)
        return true;

    return (
        uid->full_bytes == other->full_bytes
    );
}

ELFFS_API efs_result_t efs_uid_random(efs_uid_o uid)
{
    if (uid == NULL)
        return EFS_RESULT_ERROR;
#if defined(__unix__) || defined(__unix) || defined(__APPLE__)
    const int ra = open("/dev/random", O_RDONLY);

    if (ra == -1)
        return EFS_RESULT_ERROR;

    if (read(ra, uid, sizeof(efs_uid_t)) == -1)
    {
        close(ra);
        return EFS_RESULT_ERROR;
    }

    close(ra);
#else
    const uint32_t r = rand();
    efs_uid_from_u32(uid, r);
#endif
    return EFS_RESULT_SUCCESS;

}

ELFFS_API efs_uid_o efs_uid_copy(efs_uid_o uid, efs_uid_o out)
{
    if (uid == NULL || out == NULL)
        return NULL;

    if (uid == out || efs_uid_cmp(uid, out))
        return uid;

    memset(out, 0, sizeof(efs_uid_t));
    memcpy(out, uid, sizeof(efs_uid_t));

    return out;
}

ELFFS_API efs_uid_o efs_uid_from_string_hash(efs_uid_o out, const char* string)
{
    if (out == NULL || string == NULL)
        return NULL;

    uint32_t hash = 2166136261U;

    while (*string)
    {
        hash ^= (unsigned char)*string++;
        hash *= 16777619U;
    }

    efs_uid_from_u32(out, hash);
    return out;
}

ELFFS_API const char* efs_uid_as_string(char* buff, size_t len, efs_uid_o uid)
{
    static const efs_ssize_t uid_str_len = 12;
    if (len < 11 || !uid) {
        return 0;
    }

    if (buff == NULL)
        buff = malloc(uid_str_len);

    int written = snprintf(buff, len, "{%02X-%04X-%02X}",
                           uid->prefix,
                           uid->body,
                           uid->suffix);

    return written == uid_str_len ? buff : NULL;
}
