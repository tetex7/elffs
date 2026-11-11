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

#define _POSIX_C_SOURCE 200809L // NOLINT(*-reserved-identifier)
#include <stdio.h>
#undef _POSIX_C_SOURCE

#include "elffs/efs_streaming_manager.h"

#include <stdlib.h>
#include <string.h>


ELFFS_API efs_bool_t efs_streaming_manager_init(efs_streaming_manager_o self, const char* file_path)
{
    if (!self || !file_path) return false;

    self->bin = fopen(file_path, "rb");
    if (!self->bin)
        return false;
    return true;
}

static efs_result_t fclose_dtor(efsFile* file)
{
    if (!file)
        return EFS_RESULT_ERROR;
    fclose(file->data.fileStream);
    return EFS_RESULT_SUCCESS;
}

ELFFS_API efsFile* efs_streaming_manager_fopen(efs_streaming_manager_o self, const char* file_path)
{
    efsFile* file = malloc(sizeof(efsFile));

    if (!file)
        return NULL;

    memset(file, 0, sizeof(efsFile));

    file->close_dtor = &fclose_dtor;
    file->mode = EFS_FILE_READER_REAL_FILE;

    return NULL;
}
