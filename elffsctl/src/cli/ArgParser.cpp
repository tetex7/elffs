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
#include "cli/ArgParser.hpp"

#include <stdexcept>

namespace cli
{
    StreamLikeCliArgReader::StreamLikeCliArgReader(int argc, const char* argv[]) : _cli_args(argc)
    {
        _cli_args.reserve(argc);
        for (std::size_t i = 0; i < argc; i++)
        {
            _cli_args[i] = std::string_view(argv[i]);
        }
        _ctx = _cli_args.cbegin();
    }

    const std::string_view& StreamLikeCliArgReader::peek() const
    {
        if (hasNext())
        {
            return *std::next(_ctx);
        }
        throw std::out_of_range("out of cli args range");
    }

    const std::string_view& StreamLikeCliArgReader::next() const
    {
        if (hasNext())
        {
            auto lctx = _ctx;
            _ctx = std::next(_ctx);
            return *lctx;
        }
        throw std::out_of_range("out of cli args range");
    }

    void StreamLikeCliArgReader::skip() const
    {
        _ctx = std::next(_ctx);
    }

    bool StreamLikeCliArgReader::hasNext() const
    {
        return _ctx != std::cend(_cli_args);
    }

    std::size_t StreamLikeCliArgReader::getLength() const
    {
        return _cli_args.size();
    }

    std::size_t StreamLikeCliArgReader::getCount() const
    {
        return std::distance(this->_ctx, std::cend(_cli_args));
    }

    ArgParser::ArgParser(int argc, const char* argv[]) : _reader(argc, argv){}

    std::string ArgParser::getValue(const std::string& name) const
    {
        return  "";
    }
}
