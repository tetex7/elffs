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

#ifndef ELFFS_EFSFILE_HPP
#define ELFFS_EFSFILE_HPP
#include <fstream>
#include <vector>
#include <filesystem>

#include <elffs/efs_types.h>

class EfsFile final
{
private:
    mutable std::ifstream ifs;
    const std::filesystem::path archive_path;

    std::size_t fileSize;
    efs_header_t header;
    std::vector<efs_entry_t> entries;
public:
    explicit EfsFile(const std::filesystem::path& file_name);

    explicit EfsFile(const EfsFile& file);

    const std::vector<efs_entry_t>& getEntries() const;
    const efs_header_t& getHeader() const;

    std::ifstream& getFileStream();
    std::size_t getFileSize() const;

    std::vector<uint8_t> getFileBlob() const;

    ~EfsFile() = default;
};

#endif //ELFFS_EFSFILE_HPP
