// main.cpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#include <fshred/dialog.hpp>
#include <fshred/program.hpp>
#include <fshred/shredder.hpp>
#include <Windows.h>

namespace fshred {
    enum class _App_error : int {
        _Success,
        _Target_not_specified,
        _Bad_file,
        _Cannot_shred_file,
        _Cannot_delete_file,
        _Unknown_error
    };

    inline _App_error _Unsafe_entry_point(program_args& _Args) {
        program_options _Options;
        program_args::parse(_Args, _Options);
        if (_Options.path_to_file.empty()) {
            return _App_error::_Target_not_specified;
        }

        confirmation_status _Status;
        if (_Options.delete_after_shredding) { // ask for permission to destroy the file
            _Status = ::fshred::confirm_operation(L"Destroy file",
                L"      Are you sure you want to destroy this file?\n"
                L"      This action cannot be undone.");
        } else { // ask for permission to destroy the file contents
            _Status = ::fshred::confirm_operation(L"Destroy file contents",
                L"      Are you sure you want to destroy the contents of this file?\n"
                L"      This action cannot be undone.");
        }

        if (_Status == confirmation_status::unconfirmed) { // no permission, do nothing
            return _App_error::_Success;
        }

        file _File(_Options.path_to_file);
        if (!_File.is_open()) {
            return _App_error::_Bad_file;
        }

        if (!::fshred::securely_shred_file(_File)) {
            return _App_error::_Cannot_shred_file;
        }

        if (_Options.delete_after_shredding) {
            _File.close(); // must be closed
            if (!::fshred::delete_file(_Options.path_to_file)) {
                return _App_error::_Cannot_shred_file;
            }
        }

        return _App_error::_Success;
    }

    inline _App_error _Entry_point(program_args& _Args) {
        try {
            return _Unsafe_entry_point(_Args);
        } catch (...) {
            return _App_error::_Unknown_error;
        }
    }
} // namespace fshred

#ifndef _SAL_VERSION
#define _In_
#define _In_opt_
#endif // _SAL_VERSION

int wWinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ wchar_t* _Combined_args, _In_ int) {
    ::fshred::program_args _Args(_Combined_args);
    return static_cast<int>(::fshred::_Entry_point(_Args));
}