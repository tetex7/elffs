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

#include "elffs/efs_path_utils.h"

#include <string.h>

ELFFS_API efs_bool_t efs_mb_path_split(efs_path_lookup_pair_o out, const char* efs_mb_path)
{
    if (!out || !efs_mb_path)
        return false;
    if (strlen(efs_mb_path) > sizeof(efs_path_lookup_pair_t)-2)
        return false;

    efs_bool_t stag = false;
    char* blob = out->blob;
    char* path = out->path;

    for (const char* c = efs_mb_path; *c != '\0'; ++c)
    {
        if (!stag)
        {
            if (*c == ':' || *c == '@')
            {
                stag = true;
                continue;
            }
            *blob = *c;
            ++blob;
        }
        else
        {
            *path = *c;
            ++path;
        }
    }
    return stag;
}