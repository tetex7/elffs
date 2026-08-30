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

#ifndef ELFFS_INFOPACK_HPP
#define ELFFS_INFOPACK_HPP
#include <string>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

struct InfoPack
{
    std::string input_file;
    std::string output_file;
    std::string blob_name;
    std::string pack_dir;
    std::optional<std::reference_wrapper<po::variables_map>> vm;
};

#endif //ELFFS_INFOPACK_HPP
