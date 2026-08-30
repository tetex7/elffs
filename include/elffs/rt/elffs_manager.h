/*
 * Copyright (c) 2026. Tetex7
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

//
// Created by tete on 8/29/26.
//
#pragma once

#ifndef ELFFS_ELFFS_MANAGER_H
#define ELFFS_ELFFS_MANAGER_H

#include <stdio.h>
#include <elffs/efs_types.h>

#define EFS_MAN_SUPM() const uint8_t _efs_data_bin_start[] = {'8'}; const uint8_t _efs_data_bin_end[] = {'8'}

typedef struct elffs_manager_s
{
    const uint8_t* start;
    const uint8_t* end;
    efs_header_t* header;
    const char* blob_name_ref;
    efs_entry_t* entries;
    const uint8_t* blob_start;
    const uint8_t* blob_end;
} *elffs_manager_o, elffs_manager_t;

ELFFS_RT_API elffs_manager_o elffs_manager_new(const uint8_t start[], const uint8_t end[]);
ELFFS_RT_API void elffs_manager_distroy(elffs_manager_o self);


ELFFS_RT_API elffs_manager_o elffs_get_manager();
ELFFS_RT_API efs_header_t* elffs_manager_getHeader(elffs_manager_o self);
ELFFS_RT_API const char* elffs_manager_getBlobName(elffs_manager_o self);
ELFFS_RT_API efs_size_t elffs_manager_getNumEntries(elffs_manager_o self);
ELFFS_RT_API efs_entry_t* elffs_manager_getEntryByName(elffs_manager_o self, const char* name);
ELFFS_RT_API efs_uid_o elffs_manager_getUIDByFileName(elffs_manager_o self, const char* name);
ELFFS_RT_API efs_entry_t* elffs_manager_getEntries(elffs_manager_o self);
ELFFS_RT_API efs_bool_t elffs_manager_fileExists(elffs_manager_o self, const char* name);
ELFFS_RT_API efs_size_t elffs_manager_getEntriesNameList(elffs_manager_o self, const char* list[], size_t len, efs_entry_type_t type);

ELFFS_RT_API FILE* elffs_manager_fopen(elffs_manager_o self, const char* filename);

#endif //ELFFS_ELFFS_MANAGER_H
