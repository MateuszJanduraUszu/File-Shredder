// program.cpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#include <fshred/program.hpp>
#include <fshred/tinywin.hpp>
#include <mjfs/status.hpp>
#include <mjstr/string_view.hpp>
#include <shellapi.h>

namespace mjx {
    program_options::program_options() noexcept : path_to_file(), delete_after_shredding(false) {}

    program_options::~program_options() noexcept {}

    program_args::program_args(wchar_t* const _Combined_args) noexcept : _Mycount(0), _Myargs(nullptr) {
        _Split(_Combined_args, _Mycount, _Myargs);
    }

    program_args::~program_args() noexcept {
        if (_Myargs) {
            ::LocalFree(_Myargs);
            _Myargs = nullptr;
        }
    }

    void program_args::_Split(wchar_t* const _Combined_args, int& _Count, wchar_t**& _Args) noexcept {
        _Args = ::CommandLineToArgvW(_Combined_args, &_Count);
    }

    void program_args::parse(program_args& _Args, program_options& _Options) {
        bool _Path_found        = false;
        bool _Delete_flag_found = false;
        int _Count              = _Args.count();
        wchar_t** _Raw_args     = _Args.args();
        unicode_string_view _Arg;
        for (; _Count-- > 0 && (!_Path_found || !_Delete_flag_found); ++_Raw_args) {
            _Arg = *_Raw_args;
            if (!_Path_found) {
                if (::mjx::exists(_Arg)) {
                    _Options.path_to_file = _Arg;
                    _Path_found           = true;
                    continue;
                }
            }

            if (!_Delete_flag_found) {
                if (_Arg == L"-d") {
                    _Options.delete_after_shredding = true;
                    _Delete_flag_found              = true;
                }
            }
        }
    }

    wchar_t** program_args::args() const noexcept {
        return _Myargs;
    }

    const int program_args::count() const noexcept {
        return _Mycount;
    }
} // namespace mjx