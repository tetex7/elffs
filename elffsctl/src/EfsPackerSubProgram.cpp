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

#include "EfsPackerSubProgram.hpp"

#include <filesystem>
#include <fstream>
#include <print>

namespace fs = std::filesystem;
static std::size_t get_entry_count(const fs::path& dir_path)
{
    if (!fs::exists(dir_path) || !fs::is_directory(dir_path))
    {
        return 0;
    }

    // Iterates through the direct contents of the directory
    auto dir_iter = fs::directory_iterator(dir_path);
    return std::distance(fs::begin(dir_iter), fs::end(dir_iter));
}

void EfsPackerSubProgram::getFileBlob(std::vector<std::uint8_t>& fileblob, std::vector<efs_entry_t>& entries, const std::filesystem::path& dir)
{
    if (!std::filesystem::is_directory(dir))
        return;

    for (const auto& entry : std::filesystem::recursive_directory_iterator(dir))
    {
        efs_entry_t efs_entry = {};
        std::string name = std::format("/{}", std::filesystem::proximate(entry.path(), dir).string());

        if (name.size() >= EFS_ENTRY_PATH_MAX)
            throw std::runtime_error("File name too long: " + name);

        std::memset(efs_entry.name, 0, EFS_ENTRY_PATH_MAX);
        std::memcpy(efs_entry.name, name.c_str(), name.size());

        // Offset points to the location within the file blob section
        efs_entry.offset = fileblob.size();
        efs_uid_from_string_hash(&efs_entry.uid, efs_entry.name);

        if (entry.is_directory())
        {
            efs_entry.type = EFS_ENTRY_TYPE_CLUSTER;
            efs_entry.size = get_entry_count(entry.path());
            entries.push_back(efs_entry);
            continue;
        }

        efs_entry.type = EFS_ENTRY_TYPE_BLOB;
        auto fsize = entry.file_size();
        efs_entry.size = fsize;

        if (entry.is_symlink())
        {
            //TODO
        }

        entries.push_back(efs_entry);
        // Bulk-read the file into the vector directly
        std::ifstream ifs(entry.path(), std::ios::binary);
        if (!ifs)
            throw std::runtime_error("Failed to open file: " + name);

        std::size_t current_size = fileblob.size();
        fileblob.resize(current_size + fsize);
        ifs.read(reinterpret_cast<char*>(fileblob.data() + current_size), fsize);
    }
}

void EfsPackerSubProgram::pack(const std::string& blob_name, const std::string& pack_dir, const std::string& output_file)
{

    efs_header_t efs_header = {
        {
            EFS_MAGIC_BYTE_0,
            EFS_MAGIC_BYTE_1,
            EFS_MAGIC_BYTE_2,
            EFS_MAGIC_BYTE_3,
        },
        "com.trs.test",
        0,
        0,
        0,
        EFS_CURRENT_VERSION,
        {0, 0, 0}
    };


    const char* user = std::getenv("USER");
    if (!user)
        throw std::runtime_error("User environment variable not set");

    const char* home = std::getenv("HOME");
    if (!home)
    {
        throw std::runtime_error("Home environment variable not set");
    }

    std::string pre_marking = user;
    pre_marking += home;

    efs_uid_from_string_hash(&efs_header.marking, pre_marking.c_str());

    if (blob_name.size() >= EFS_BLOB_NAME_MAX)
    {
        throw std::runtime_error("Blob name too long: " + blob_name);
    }

    std::memset(efs_header.blob_name, 0, EFS_BLOB_NAME_MAX);
    std::memcpy(efs_header.blob_name, blob_name.c_str(), blob_name.size());

    std::vector<std::uint8_t> fileblob;
    std::vector<efs_entry_t> entrys;

    getFileBlob(fileblob, entrys, pack_dir);

    efs_header.num_entry = entrys.size();

    efs_header.offset_to_blob = sizeof(efs_header_t) + (entrys.size() * sizeof(efs_entry_t));
    efs_header.blob_size = fileblob.size();

    std::println("blob name: {}", efs_header.blob_name);
    std::println("marking UID: {{{:02X}-{:04X}-{:02X}}}", efs_header.marking.prefix, efs_header.marking.body, efs_header.marking.suffix);
    std::println("efs version: {{{:02X}-{:04X}-{:02X}}}", efs_header.blob_version.prefix, efs_header.blob_version.body, efs_header.blob_version.suffix);
    std::println("entry: {}",  static_cast<std::size_t>(efs_header.num_entry));
    std::println("offset to blob: {}", static_cast<std::size_t>(efs_header.offset_to_blob));
    std::println("file blob size: {}", static_cast<std::size_t>(efs_header.blob_size));

    std::println("\n");
    for (const auto& entry : entrys)
    {
        std::println("name: {}", entry.name);
        std::println("size: {}", entry.size);
        std::println("type: {}", efs_entry_type_to_string(entry.type));
        std::println("offset: {}", entry.offset);
        std::println("UID: {{{:02X}-{:04X}-{:02X}}}", entry.uid.prefix, entry.uid.body, entry.uid.suffix);
        std::println("");
    }

    std::ofstream ofs(output_file, std::ios::binary);

    ofs.write(reinterpret_cast<const char*>(&efs_header), sizeof(efs_header_t));
    for (const efs_entry_t& entry : entrys)
    {
        ofs.write(reinterpret_cast<const char*>(&entry), sizeof(efs_entry_t));
    }
    ofs.write(reinterpret_cast<const char*>(fileblob.data()), fileblob.size());
}

void EfsPackerSubProgram::pre_start(){}

int EfsPackerSubProgram::sub_program_main()
{
    pack(getInfoPack().blob_name, getInfoPack().pack_dir, getInfoPack().output_file);
    return 0;
}
