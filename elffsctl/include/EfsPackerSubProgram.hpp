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

#ifndef ELFFS_EFSPACKERSUBPROGRAM_HPP
#define ELFFS_EFSPACKERSUBPROGRAM_HPP
#include "ISubProgram.hpp"
#include <elffs/efs_types.h>
#include <filesystem>

class EfsPackerSubProgram final : public ISubProgram
{
private:
    static void getFileBlob(std::vector<std::uint8_t>& fileblob, std::vector<efs_entry_t>& entries, const std::filesystem::path& dir);
    static void pack(const std::string& blob_name, const std::string& pack_dir, const std::string& output_file);
public:
    explicit inline EfsPackerSubProgram(const InfoPack& infoPack) : ISubProgram(infoPack) {};

    void pre_start() override;
    int sub_program_main() override;

    ~EfsPackerSubProgram() override = default;
};

#endif //ELFFS_EFSPACKERSUBPROGRAM_HPP
