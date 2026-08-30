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
#define _POSIX_C_SOURCE 200809L // NOLINT(*-reserved-identifier)
#include <stdlib.h>
#include <string.h>
#include <../include/elffs/rt/elffs_manager.h>


extern const uint8_t _efs_data_bin_start[]; // NOLINT(*-reserved-identifier)
extern const uint8_t _efs_data_bin_end[]; // NOLINT(*-reserved-identifier)

static elffs_manager_t* manager = NULL;

__attribute__((constructor)) static void setup_efs_data_bin_start()
{
    if (_efs_data_bin_start[0] == '8')
        return;

    manager = (elffs_manager_t*)malloc(sizeof(elffs_manager_t));

    if (manager == NULL)
    {
        puts("elffs malloc failed");
        abort();
    }

    manager->start = (uint8_t*)_efs_data_bin_start;
    manager->end = (uint8_t*)_efs_data_bin_end;
    manager->header = (efs_header_t*)_efs_data_bin_start;

    static const char currect[] = {
        EFS_MAGIC_BYTE_0,
        EFS_MAGIC_BYTE_1,
        EFS_MAGIC_BYTE_2,
        EFS_MAGIC_BYTE_3,
    };

    char maz[EFS_MAGIC_LEN] = {0};
    memcpy(maz, _efs_data_bin_start, EFS_MAGIC_LEN);

    if (memcmp(maz, currect, EFS_MAGIC_LEN) != 0)
    {
        free(manager);
        puts("bad efs magic");
        abort();
    }

    manager->blob_name_ref = manager->header->blob_name;
    manager->entries = (efs_entry_t*)(manager->start+sizeof(efs_header_t));
    manager->blob_start = manager->start+manager->header->offset_to_blob;
    manager->blob_end = manager->blob_start + manager->header->blob_size;
}

__attribute__((destructor)) static void teardown_efs_data_bin_start()
{
    if (manager != NULL)
        free(manager);
}

ELFFS_RT_API elffs_manager_o elffs_get_manager()
{
    if (manager == NULL)
    {
        puts("elffs_manager not found");
        abort();
    }
    return manager;
}

ELFFS_RT_API efs_header_t* elffs_manager_getHeader(elffs_manager_o self)
{
    if (self == NULL)
        return NULL;
    return self->header;
}

ELFFS_RT_API const char* elffs_manager_getBlobName(elffs_manager_o self)
{
    if (self == NULL)
        return NULL;
    return self->blob_name_ref;
}

ELFFS_RT_API efs_size_t elffs_manager_getNumEntries(elffs_manager_o self)
{
    if (self == NULL)
        return 0;
    return self->header->num_entry;
}

ELFFS_RT_API efs_entry_t* elffs_manager_getEntryByName(elffs_manager_o self, const char* name)
{
    if (self == NULL)
        return NULL;
    if (name == NULL)
        return NULL;
    size_t name_len = strlen(name);

    if (name_len > EFS_ENTRY_PATH_MAX)
        return NULL;

    for (size_t i = 0; i < self->header->num_entry; i++)
    {
        size_t en_len = strlen(self->entries[i].name);
        if (name_len == en_len && strncmp(self->entries[i].name, name, en_len) == 0)
        {
            return &self->entries[i];
        }
    }
    return NULL;
}

ELFFS_RT_API efs_uid_o elffs_manager_getUIDByFileName(elffs_manager_o self, const char* name)
{
    if (!self || !name)
    {
        return NULL;
    }
    efs_entry_t* entry = elffs_manager_getEntryByName(self, name);
    if (entry == NULL)
        return NULL;
    return &entry->uid;
}

ELFFS_RT_API efs_entry_t* elffs_manager_getEntries(elffs_manager_o self)
{
    return self->entries;
}

ELFFS_RT_API efs_bool_t elffs_manager_fileExists(elffs_manager_o self, const char* name)
{
    return elffs_manager_getEntryByName(self, name) != NULL;
}

ELFFS_RT_API FILE* elffs_manager_fopen(elffs_manager_o self, const char* filename)
{
    for (size_t i = 0; i < self->header->num_entry; i++)
    {
        if (strncmp(self->entries[i].name, filename, EFS_ENTRY_PATH_MAX) == 0)
        {
            return fmemopen(&self->blob_start[self->entries[i].offset], self->entries[i].size, "r");
        }
    }
    return NULL;
}
