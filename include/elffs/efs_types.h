/*
 * Copyright (c) 2026. Tetex7
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

//
// Created by tete on 8/12/26.
//

#ifndef ELFFS_EFS_TYPES_H
#define ELFFS_EFS_TYPES_H

#include <elffs/export_defs.h>
#include <stdint.h>
#include <stddef.h>
#include <elffs/efs_uid.h>

ELFFS_BEGIN_CPP_COMPAT

#define EFS_ENTRY_PATH_MAX (200)
#define EFS_BLOB_NAME_MAX (100)
#define EFS_MAGIC_LEN (4)

#define EFS_MAGIC_BYTE_0 ('\x7F')
#define EFS_MAGIC_BYTE_1 ('E')
#define EFS_MAGIC_BYTE_2 ('F')
#define EFS_MAGIC_BYTE_3 ('S')
#define EFS_MAGIC_BYTE_FULL_STRING {EFS_MAGIC_BYTE_0, EFS_MAGIC_BYTE_1, EFS_MAGIC_BYTE_2, EFS_MAGIC_BYTE_3, '\0'}
#define EFS_MAGIC_BYTE_STRING {EFS_MAGIC_BYTE_0, EFS_MAGIC_BYTE_1, EFS_MAGIC_BYTE_2, EFS_MAGIC_BYTE_3}
#define EFS_PAD_VALUE (0xFF)


#define EFS_VERSION_1_UID ((efs_uid_t){1, 55466, 1})

#define EFS_CURRENT_VERSION (EFS_VERSION_1_UID)

enum efs_entry_type_e
{
    EFS_ENTRY_TYPE_NONE = 0,
    EFS_ENTRY_TYPE_BLOB,
    EFS_ENTRY_TYPE_CLUSTER,
};

typedef efs_byte_t efs_entry_type_t;

ELFFS_API const char* efs_entry_type_to_string(efs_entry_type_t type);

typedef struct ELFFS_NO_IMPLICIT_PADDING efs_header_s
{
    char magic[EFS_MAGIC_LEN];
    char blob_name[EFS_BLOB_NAME_MAX];
    efs_size_t num_entry;
    efs_size_t offset_to_blob;
    efs_size_t blob_size;
    efs_uid_t blob_version;
    efs_uid_t marking;
} efs_header_t;

ELFFS_API efs_bool_t efs_header_magic_check(const efs_byte_t (*test_magic)[EFS_MAGIC_LEN]);

#define EFS_ENTRY_EXT_SIZE (sizeof(efs_word_t)*8)

typedef struct ELFFS_NO_IMPLICIT_PADDING efs_entry_s
{
    char name[EFS_ENTRY_PATH_MAX];
    efs_entry_type_t type;
    efs_uid_t uid;
    efs_size_t size;
    efs_size_t offset;
    efs_byte_t ext_unused[EFS_ENTRY_EXT_SIZE];
} efs_entry_t;

typedef struct ELFFS_NO_IMPLICIT_PADDING efs_cluster_ext_data_s
{
    efs_uid_t owner;
    efs_f8_t flags;
    efs_byte_t etext[4];
} efs_cluster_ext_data_t;

ELFFS_STATIC_ASSERT(sizeof(efs_cluster_ext_data_t) < EFS_ENTRY_EXT_SIZE, "bad efs cluster ext data size");

ELFFS_END_CPP_COMPAT

#endif //ELFFS_EFS_TYPES_H
