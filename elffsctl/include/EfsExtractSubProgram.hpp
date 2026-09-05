//
// Created by tete on 8/31/26.
//

#ifndef ELFFS_EFSEXTRACTFILE_HPP
#define ELFFS_EFSEXTRACTFILE_HPP
#include "EfsFile.hpp"
#include "ISubProgram.hpp"

class EfsExtractSubProgram final : public ISubProgram
{
private:
    EfsFile file;
public:
    explicit EfsExtractSubProgram(const InfoPack& infoPack) : ISubProgram(infoPack), file(infoPack.input_file) {}

    void pre_start() override;
    int sub_program_main() override;


    ~EfsExtractSubProgram() override = default;
};

#endif //ELFFS_EFSEXTRACTFILE_HPP
