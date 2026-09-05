//
// Created by tete on 8/23/26.
//

#include "EfsFile.hpp"

EfsFile::EfsFile(const std::filesystem::path& file_name) : ifs(file_name, std::ios::binary), archive_path(file_name), header()
{
    constexpr std::array<char, EFS_MAGIC_LEN> currect = {
        EFS_MAGIC_BYTE_0,
        EFS_MAGIC_BYTE_1,
        EFS_MAGIC_BYTE_2,
        EFS_MAGIC_BYTE_3,
    };

    if (!std::filesystem::exists(file_name) || std::filesystem::is_directory(file_name))
        throw std::runtime_error(std::format("efs file \'{}\' does not exist or directory", file_name));

    std::array<char, EFS_MAGIC_LEN> magic = {};


    ifs.read(magic.data(), magic.size());

    if (magic != currect)
    {
        throw std::logic_error("efs magic is not correct");
    }

    fileSize = std::filesystem::file_size(file_name);

    ifs.seekg(0, std::ios::beg);

    entries.resize(sizeof(efs_entry_t) * header.num_entry);

    ifs.read(reinterpret_cast<char*>(&this->header), sizeof(efs_header_t));
    for (size_t i = 0; i < header.num_entry; i++)
    {
        efs_entry_t entry;
        ifs.read(reinterpret_cast<char*>(&entry), sizeof(efs_entry_t));
        entries.push_back(entry);
    }
    ifs.seekg(0, std::ios::beg);
}

EfsFile::EfsFile(const EfsFile& file) : EfsFile(file.archive_path) {}

const std::vector<efs_entry_t>& EfsFile::getEntries() const
{
    return entries;
}

const efs_header_t & EfsFile::getHeader() const
{
    return header;
}

std::ifstream& EfsFile::getFileStream()
{
    return ifs;
}

std::size_t EfsFile::getFileSize() const
{
    return fileSize;
}

std::vector<uint8_t> EfsFile::getFileBlob() const
{
    std::vector<uint8_t> blob;
    blob.resize(header.blob_size);

    auto pos = ifs.tellg();

    ifs.seekg(header.offset_to_blob, std::ios::beg);

    ifs.read(reinterpret_cast<char*>(blob.data()), blob.size());
    ifs.seekg(pos, std::ios::beg);
    return blob;

}






