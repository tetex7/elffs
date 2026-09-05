//
// Created by tete on 9/1/26.
//
#include <elffs/rt/rt_manager.h>
#include <stdlib.h>
#include <string.h>

extern const uint8_t _efs_data_bin_start[]; // NOLINT(*-reserved-identifier)
extern const uint8_t _efs_data_bin_end[]; // NOLINT(*-reserved-identifier)
extern const efs_size_t _efs_data_bin_size; // NOLINT(*-reserved-identifier)

static elffs_manager_t* manager = NULL;

__attribute__((constructor)) static void setup_efs_data_bin_start()
{
    if (_efs_data_bin_end - _efs_data_bin_start != _efs_data_bin_size)
    {
        puts("efs size mismatch");
        abort();
    }

    if (_efs_data_bin_start[0] == '8')
        return;

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
        puts("bad efs magic");
        abort();
    }

    manager = (elffs_manager_t*)malloc(sizeof(elffs_manager_t));

    if (manager == NULL)
    {
        puts("elffs malloc failed");
        abort();
    }

    manager->start = (uint8_t*)_efs_data_bin_start;
    manager->end = (uint8_t*)_efs_data_bin_end;
    manager->header = (efs_header_t*)_efs_data_bin_start;
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

ELFFS_RT_API elffs_manager_o elffs_get_global_manager()
{
    if (manager == NULL)
    {
        puts("elffs global manager not found");
        abort();
    }
    return manager;
}