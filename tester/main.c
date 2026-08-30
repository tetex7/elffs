/*
 * Copyright (c) 2026. Tetex7
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

//
// Created by tete on 8/28/26.
//
#define _POSIX_C_SOURCE 200809L
#include <string.h>
#include <elffs/efs_types.h>
#include <unistd.h>
#include <stdio.h>

extern const uint8_t _efs_data_bin_start[];
extern const uint8_t _efs_data_bin_end[];

/*int main(int argc, char *argv[])
{
    char currect[EFS_MAGIC_LEN] = {
        EFS_MAGIC_BYTE_0,
        EFS_MAGIC_BYTE_1,
        EFS_MAGIC_BYTE_2,
        EFS_MAGIC_BYTE_3,
    };

    char magic[EFS_MAGIC_LEN+1] = {0};


    memcpy(magic, _efs_data_bin_start, EFS_MAGIC_LEN);

    if (memcmp(magic, currect, EFS_MAGIC_LEN) != 0)
    {
        printf("bad magic\n");
        return 1;
    }

    printf("good magic\n");
    printf("%s\n", magic);

    efs_header_t* header = (efs_header_t*)_efs_data_bin_start;
    printf("blob name: %s\n\n", header->blob_name);

    efs_entry_t* entries = (efs_entry_t*)(_efs_data_bin_start+sizeof(efs_header_t));
    for (uint32_t i = 0; i < header->num_entry; i++)
    {
        printf("entry name: %s\n\n", entries[i].name);
    }

    uint8_t* bob = (uint8_t*)(_efs_data_bin_start+header->offset_to_blob);

    FILE* f = fmemopen(bob+entries[0].offset, entries[0].size, "r");

    for (uint32_t i = 0; i < entries[0].size; i++)
    {
        int g = fgetc(f);
        if (g != EOF)
        {
            printf("%c", g);
        }
    }
    fclose(f);
    return 0;
}
*/

#include <elffs/rt/elffs_manager.h>

int main(int argc, char* argv[])
{
    elffs_manager_o manager = elffs_get_manager();

    FILE* f = elffs_manager_fopen(manager, "/dir/test.txt");

    efs_uid_o uid = elffs_manager_getUIDByFileName(manager, "/test44.txt");

    printf("uid = 0x%X\n", uid->full_bytes);

    for (int c = fgetc(f); c != EOF; c = fgetc(f))
    {
        putchar(c);
    }
    fclose(f);
}
