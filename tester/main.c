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

#include <elffs/rt/rt_manager.h>
#include <elffs/efs_path_utils.h>

#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[])
{
    elffs_manager_o manager = elffs_get_global_manager();

    FILE* f = elffs_manager_fopen(manager, "/dir/test.txt");

    const efs_uid_o uid = elffs_manager_getUIDByFileName(manager, "/test44.txt");

    const size_t len = elffs_manager_getEntriesNameList(manager, NULL, 0);

    const char* list[len];
    elffs_manager_getEntriesNameList(manager, list, len);

    for (size_t i = 0; i < len; i++)
    {
        printf("%s\n", list[i]);
    }

    printf("uid = 0x%X\n", uid->full_bytes);

    for (int c = fgetc(f); c != EOF; c = fgetc(f))
    {
        putchar(c);
    }
    fclose(f);

    putchar('\n');
    putchar('\n');
    efs_path_lookup_pair_t pair = {};
    efs_mb_path_split(&pair, "com.test.cp:/test/44.txt");
    printf("\"%s\"@\"%s\"\n", pair.path, pair.blob);
}
