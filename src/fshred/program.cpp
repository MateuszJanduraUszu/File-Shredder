// program.cpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#include <fshred/program.hpp>
#include <filesystem>
#include <string_view>
#include <Windows.h>

namespace fshred {
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

    void program_args::parse(program_args& _Args, program_options& _Options) noexcept {
        bool _Found_path        = false;
        bool _Found_delete_flag = false;
        int _Count              = _Args.count();
        wchar_t** _Raw_args     = _Args.args();
        ::std::wstring_view _Arg;
        while (_Count-- > 0 && (!_Found_path || !_Found_delete_flag)) {
            _Arg = ::std::wstring_view{*_Raw_args++};
            if (!_Found_path) {
                if (::std::filesystem::exists(path{_Arg})) {
                    _Options.path_to_file = path{_Arg};
                    _Found_path           = true;
                    continue;
                }
            }

            if (!_Found_delete_flag) {
                if (_Arg == L"-d") {
                    _Options.delete_after_shredding = true;
                    _Found_delete_flag              = true;
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
} // namespace fshred