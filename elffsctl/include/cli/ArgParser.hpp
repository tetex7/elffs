/*
 * Copyright (c) 2026. Tetex7
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

//
// Created by tete on 9/20/26.
//

#pragma once


#ifndef ELFFS_ARGPARSER_HPP
#define ELFFS_ARGPARSER_HPP

#include <map>
#include <memory>
#include <string_view>
#include <vector>

namespace cli
{
    class StreamLikeCliArgReader final
    {
    private:
        std::vector<std::string_view> _cli_args;
        mutable std::vector<std::string_view>::const_iterator _ctx;
    public:
        StreamLikeCliArgReader(int argc, const char* argv[]);

        [[nodiscard]] const std::string_view& peek() const;
        [[nodiscard]] const std::string_view& next() const;
        void skip() const;
        [[nodiscard]] bool hasNext() const;

        std::size_t getLength() const;
        std::size_t getCount() const;

        ~StreamLikeCliArgReader() = default;
    };

    class ArgParser final : public std::map<std::string_view, std::string_view>
    {
        StreamLikeCliArgReader _reader;
        std::vector<std::string> boot;
    public:
        ArgParser(int argc, const char* argv[]);

        std::string getValue(const std::string& name) const;

        ArgParser& operator()(const std::string& name_format);

        ~ArgParser() = default;
    };
}

#endif //ELFFS_ARGPARSER_HPP
