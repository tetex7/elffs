//
// Created by tete on 8/23/26.
//

#ifndef ELFFS_EFSOBJECTFILESUBPROGRAM_HPP
#define ELFFS_EFSOBJECTFILESUBPROGRAM_HPP
#include "ISubProgram.hpp"

class EfsObjectFileSubProgram final : public ISubProgram
{
private:
    static void write_elf_object(const std::string& filename, const std::vector<uint8_t>& payload);
public:
    explicit inline EfsObjectFileSubProgram(const InfoPack& infoPack) : ISubProgram(infoPack) {}

    void pre_start() override;
    int sub_program_main() override;

    ~EfsObjectFileSubProgram() override = default;
};

#endif //ELFFS_EFSOBJECTFILESUBPROGRAM_HPP
