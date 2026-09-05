/*
 * Copyright (c) 2026. Tetex7
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

//
// Created by tete on 8/12/26.
//

#include <memory>
#include <elf.h>
#include <print>
#include <iostream>
#include <fstream>
#include <filesystem>

#include <boost/program_options.hpp>

#include <elffs/efs_types.h>

#include "EfsListFilesSubProgram.hpp"
#include "EfsObjectFileSubProgram.hpp"
#include "EfsPackerSubProgram.hpp"
#include "ISubProgram.hpp"
#include "InfoPack.hpp"
#include "EfsExtractSubProgram.hpp"

namespace po = boost::program_options;

po::options_description desc("elffsctl Options");

int prog(int argc, char* argv[])
{
    InfoPack info_pack;


    desc.add_options()
        ("help,h", "Show help message")
        ("list", "List entry in a efs")
        ("pack,p", po::value<std::string>(&info_pack.pack_dir), "packs a dir")
        ("extract", "extract a file")
        ("make-object-file", "makes ld object file")
        ("efs-path", po::value<std::string>(&info_pack.efs_path)->value_name("path"), "a path in a efs")
        ("blob-name,b", po::value<std::string>(&info_pack.blob_name)->value_name("name")->default_value("efs_blob"), "name of the blob")
        ("output,o", po::value<std::string>(&info_pack.output_file)->default_value("/dev/stdout"), "Output file path")
        ("input,i", po::value(&info_pack.input_file), "Input .efs file");

    po::positional_options_description pos;
    pos.add("input", 1);
    pos.add("output", 1);

    po::variables_map vm;

    try
    {
        po::store(
            po::command_line_parser(argc, argv)
                .options(desc)
                .positional(pos)
                .run(),
            vm
        );

        if (vm.count("help"))
        {
            std::cout << "Usage: " << std::filesystem::path(argv[0]).filename().generic_string() << " <input> <output>" << "\n";
            std::cout << desc << "\n";
            return 0;
        }

        po::notify(vm); // triggers required() checks

    }
    catch (const std::exception& e)
    {
        std::cerr << "Usage: " << std::filesystem::path(argv[0]).filename().generic_string() << " <input> <output>" << "\n";
        std::println(std::cerr, "Argument error: {}", e.what());
        std::cerr << desc << std::endl;
        return 1;
    }

    info_pack.vm = vm;

    std::optional<std::polymorphic<ISubProgram>> sub_program;

    if (vm.contains("pack"))
    {
        sub_program = ISubProgram::newSubProgram<EfsPackerSubProgram>(info_pack);
    }
    else if (vm.contains("list"))
    {
        sub_program = ISubProgram::newSubProgram<EfsListFilesSubProgram>(info_pack);
    }
    else if (vm.contains("make-object-file") && vm.contains("input"))
    {
        sub_program = ISubProgram::newSubProgram<EfsObjectFileSubProgram>(info_pack);
    }
    else if (vm.contains("extract"))
    {
        sub_program = ISubProgram::newSubProgram<EfsExtractSubProgram>(info_pack);
    }

    if (sub_program.has_value())
    {
        sub_program.value()->pre_start();
        const int r = sub_program.value()->sub_program_main();
        if (r != 0) return r;
    }
    else
    {
        throw std::logic_error("No subprogram found");
    }

    return EXIT_SUCCESS;
}


int main(int argc, char *argv[])
{
    try
    {
        return prog(argc, argv);
    }
    catch (const std::exception& e)
    {
        std::println(std::cerr, "Exception: {}", e.what());
        std::cerr << "Usage: " << std::filesystem::path(argv[0]).filename().generic_string() << " <input> <output>" << "\n";
        std::cerr << desc << std::endl;
    }
    return 1;
}