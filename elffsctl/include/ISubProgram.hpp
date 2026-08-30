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

#pragma once

#ifndef ELFFS_ISUBPROGRAM_HPP
#define ELFFS_ISUBPROGRAM_HPP
#include "InfoPack.hpp"

#include <memory>

class ISubProgram
{
private:
    const InfoPack& infoPack;
protected:
    explicit inline ISubProgram(const InfoPack& infoPack) : infoPack(infoPack) {}
    [[nodiscard]] inline const InfoPack& getInfoPack() const
    {
        return infoPack;
    }
public:
    virtual void pre_start() = 0;
    virtual int sub_program_main() = 0;


    virtual ~ISubProgram() = default;


    template <typename T> requires std::is_class_v<T> && std::is_constructible_v<T, const InfoPack&>
    static std::polymorphic<ISubProgram> newSubProgram(const InfoPack& info_pack)
    {
        return std::polymorphic<ISubProgram>(std::type_identity_t<T>(info_pack));
    }
};

#endif
