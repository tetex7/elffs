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
#include <stdio.h>
#undef _POSIX_C_SOURCE

#include <stdlib.h>
#include <string.h>
#include <elffs/elffs_manager.h>

ELFFS_API elffs_manager_o elffs_manager_new(const uint8_t start[], const uint8_t end[])
{
    if (start == NULL || end == NULL)
        return NULL;

    const size_t blob_size = end - start;

    if (blob_size <= EFS_MAGIC_LEN)
        return NULL;

    static const char currect[] = {
        EFS_MAGIC_BYTE_0,
        EFS_MAGIC_BYTE_1,
        EFS_MAGIC_BYTE_2,
        EFS_MAGIC_BYTE_3,
    };

    char maz[EFS_MAGIC_LEN] = {0};
    memcpy(maz, end, EFS_MAGIC_LEN);

    if (memcmp(maz, currect, EFS_MAGIC_LEN) != 0)
    {
        return NULL;
    }

    elffs_manager_o self = malloc(sizeof(elffs_manager_t));

    if (self == NULL)
        return NULL;

    self->start = (uint8_t*)start;
    self->end = (uint8_t*)end;
    self->header = (efs_header_t*)start;
    self->blob_name_ref = self->header->blob_name;
    self->entries = (efs_entry_t*)start+sizeof(efs_header_t);
    self->blob_start = (uint8_t*)start+self->header->offset_to_blob;
    self->blob_end = self->blob_start + self->header->blob_size;
    return self;
}

ELFFS_API void elffs_manager_distroy(elffs_manager_o self)
{
    if (self != NULL)
        free(self);
}

ELFFS_API efs_header_t* elffs_manager_getHeader(elffs_manager_o self)
{
    if (self == NULL)
        return NULL;
    return self->header;
}

ELFFS_API const char* elffs_manager_getBlobName(elffs_manager_o self)
{
    if (self == NULL)
        return NULL;
    return self->blob_name_ref;
}

ELFFS_API efs_size_t elffs_manager_getNumEntries(elffs_manager_o self)
{
    if (self == NULL)
        return 0;
    return self->header->num_entry;
}

ELFFS_API efs_entry_t* elffs_manager_getEntryByName(elffs_manager_o self, const char* name)
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

ELFFS_API efs_uid_o elffs_manager_getUIDByFileName(elffs_manager_o self, const char* name)
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

ELFFS_API efs_entry_t* elffs_manager_getEntries(elffs_manager_o self)
{
    return self->entries;
}

ELFFS_API efs_bool_t elffs_manager_fileExists(elffs_manager_o self, const char* name)
{
    return elffs_manager_getEntryByName(self, name) != NULL;
}

ELFFS_API efs_size_t elffs_manager_getEntriesNameList(elffs_manager_o self, const char* list[], size_t len)
{
    if (self == NULL)
        return 0;
    if (list == NULL)
    {
        return self->header->num_entry;
    }

    for (size_t i = 0; i < len; i++)
    {
        list[i] = self->entries[i].name;
    }
    return len;
}

ELFFS_API FILE* elffs_manager_fopen(elffs_manager_o self, const char* filename)
{
    for (size_t i = 0; i < self->header->num_entry; i++)
    {
        if (strncmp(self->entries[i].name, filename, EFS_ENTRY_PATH_MAX) == 0)
        {
            return fmemopen((void*)&self->blob_start[self->entries[i].offset], self->entries[i].size, "r");
        }
    }
    return NULL;
}

ELFFS_API efs_bool_t elffs_manager_openAsMemReference(elffs_manager_o self, const char* name, elffs_file_mem_reference_o memref_outref)
{
    if (self == NULL || name == NULL || memref_outref == NULL)
        return false;

    for (size_t i = 0; i < self->header->num_entry; i++)
    {
        if (strncmp(self->entries[i].name, name, EFS_ENTRY_PATH_MAX) == 0)
        {
            memref_outref->start = &self->blob_start[self->entries[i].offset];
            memref_outref->end = &self->blob_start[self->entries[i].offset + self->entries[i].size];
            memref_outref->size = self->entries[i].size;
            return true;
        }
    }
    return false;
}
