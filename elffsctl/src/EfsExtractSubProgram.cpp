//
// Created by tete on 8/31/26.
//
#include <print>
#include "EfsExtractSubProgram.hpp"

void EfsExtractSubProgram::pre_start()
{
    auto& vm = getInfoPack().vm.value().get();
    if (!vm.contains("input") || !vm.contains("output"))
    {
        throw std::logic_error("EfsExtractFile::pre_start(): no \"input\" or \"output\"");
    }
}

int EfsExtractSubProgram::sub_program_main()
{
    auto files = file.getFileBlob();

    for (const auto& entry : file.getEntries())
    {
        std::string_view name = entry.name;
        if (name == getInfoPack().efs_path)
        {
            if (entry.type != EFS_ENTRY_TYPE_BLOB)
            {
                throw std::runtime_error( std::format("EfsExtractFile::sub_program_main(): entry {} is not a BLOB", name));
            }
            std::ofstream out(getInfoPack().output_file);
            out.write(reinterpret_cast<char*>(&files[entry.offset]), entry.size);
            std::println("written {} bytes to {}", entry.size, getInfoPack().output_file);
            return 0;
        }
    }
    throw std::runtime_error(std::format("{} not found", getInfoPack().efs_path));
    return 0;
}
