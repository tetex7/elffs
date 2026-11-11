/*
 * Copyright (c) 2026. Tetex7
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

//
// Created by tete on 11/3/26.
//

#ifndef ELFFS_EFSFILE_H
#define ELFFS_EFSFILE_H

#include <stdio.h>

#include <elffs/export_defs.h>
#include <elffs/efs_int_types.h>

ELFFS_BEGIN_CPP_COMPAT

typedef struct efsFile_decompression_buffer_s
{
    efs_byte_t (*buffer)[];
    efs_size32_t size;
    efs_result_t (*decompress)(struct efsFile_decompression_buffer_s* buffer);
} efsFile_decompression_buffer_t;

enum efsFile_reader_mode_e
{
    EFS_FILE_READER_MEMORY,
    EFS_FILE_READER_REAL_FILE,
};

typedef efs_byte_t efsFile_reader_mode_t;

typedef struct efsFile_ext_super_s
{
    efs_word_t ext_id;
} efsFile_ext_super_t;

typedef struct efsFile_s
{
    efsFile_reader_mode_t mode;

    // this was going be useless padding so lets use it
    efs_byte_t mode_ext[7];

    // it's yes this is a more or less a virtual
    efs_result_t (*close_dtor)(struct efsFile_s* file);



    efsFile_ext_super_t* ext;

    efsFile_decompression_buffer_t decompression_buffer;

    union
    {
        struct
        {
            const efs_byte_t (*buffer)[];
            efs_size32_t position;
            efs_size32_t size;
        } memoryFile;
        FILE* fileStream;
    } data;
} efsFile;

ELFFS_API efs_bool_t efsFile_feof(efsFile* file);
ELFFS_API efs_fbyte_t efsFile_fgetc(efsFile* file);
ELFFS_API efs_result_t efsFile_fseek(efsFile* file, efs_off32_t offset, int whence);
ELFFS_API efs_size_t efsFile_ftell(efsFile* file);
ELFFS_API efs_size_t efsFile_fread(efsFile* file, efs_byte_t buffer[], efs_size32_t size);

ELFFS_API efs_result_t efsFile_fclose(efsFile* file);

ELFFS_END_CPP_COMPAT

#endif //ELFFS_EFSFILE_H
