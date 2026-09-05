/*
 * Copyright (c) 2026. Tetex7
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

//
// Created by tete on 8/23/26.
//

#include "EfsListFilesSubProgram.hpp"

#include "EfsFile.hpp"
#include <print>

void EfsListFilesSubProgram::pre_start()
{
    auto& vm = getInfoPack().vm.value().get();
    if (!vm.contains("input"))
        throw std::logic_error("EfsListFilesSubProgram::pre_start() no input file");
}

int EfsListFilesSubProgram::sub_program_main()
{
    EfsFile file = EfsFile(getInfoPack().input_file);

    const efs_header_t& efs_header = file.getHeader();

    std::size_t cluster_num = 0;

    std::println("blob name: {}", efs_header.blob_name);
    std::println("entry: {}", efs_header.num_entry);
    std::println("offset to blob: {}", efs_header.offset_to_blob);
    std::println("blob size: {}", efs_header.blob_size);
    std::println("efs version: {{{:02X}-{:04X}-{:02X}}}", efs_header.blob_version.prefix, efs_header.blob_version.body, efs_header.blob_version.suffix);

    std::println("\n");
    for (const auto& entry : file.getEntries())
    {
        if (entry.type == EFS_ENTRY_TYPE_CLUSTER)
        {
            cluster_num++;
        }
        std::println("name: {}", entry.name);
        std::println("size: {}", entry.size);
        std::println("type: {}", efs_entry_type_to_string(entry.type));
        std::println("offset: {}", entry.offset);
        std::println("UID: {{{:02X}-{:04X}-{:02X}}}", entry.uid.prefix, entry.uid.body, entry.uid.suffix);
        std::println("");
    }
    //CLUSTER
    std::println("{} blob(s), {} cluster(s), {} all", file.getEntries().size() - cluster_num, cluster_num, file.getEntries().size());
    return 0;
}
