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

#ifndef ELFFS_EFSLISTFILESSUBPROGRAM_HPP
#define ELFFS_EFSLISTFILESSUBPROGRAM_HPP
#include "ISubProgram.hpp"

class EfsListFilesSubProgram final : public ISubProgram
{
public:
    explicit inline EfsListFilesSubProgram(const InfoPack& infoPack) : ISubProgram(infoPack) {};
    void pre_start() override;
    int sub_program_main() override;

    ~EfsListFilesSubProgram() override = default;
};


#endif //ELFFS_EFSLISTFILESSUBPROGRAM_HPP
