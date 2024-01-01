// program.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _FSHRED_PROGRAM_HPP_
#define _FSHRED_PROGRAM_HPP_
#include <mjfs/path.hpp>

namespace mjx {
    class program_options {
    public:
        path path_to_file;
        bool delete_after_shredding;

        program_options() noexcept;
        ~program_options() noexcept;
    };
    
    class program_args {
    public:
        explicit program_args(wchar_t* const _Combined_args) noexcept;
        ~program_args() noexcept;

        // parses program arguments
        static void parse(program_args& _Args, program_options& _Options);

        // returns the arguments
        wchar_t** args() const noexcept;

        // returns the number of arguments
        const int count() const noexcept;

    private:
        // splits combined arguments
        static void _Split(wchar_t* const _Combined_args, int& _Count, wchar_t**& _Args) noexcept;

        wchar_t** _Myargs;
        int _Mycount;
    };
} // namespace mjx

#endif // _FSHRED_PROGRAM_HPP_