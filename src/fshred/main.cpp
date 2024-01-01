// main.cpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#include <cstdio>
#include <fshred/dialog.hpp>
#include <fshred/program.hpp>
#include <fshred/shredder.hpp>
#include <fshred/tinywin.hpp>
#include <mjfs/status.hpp>

namespace mjx {
    enum class _App_error : int {
        _Success,
        _Target_not_specified,
        _Bad_file,
        _Cannot_shred_file,
        _Cannot_delete_file,
        _Unknown_error
    };

    inline const wchar_t* _Translate_app_error(const _App_error _Error) noexcept {
        switch (_Error) {
        case _App_error::_Target_not_specified:
            return L"Target not specified or does not exist";
        case _App_error::_Bad_file:
            return L"Could not open the specified file";
        case _App_error::_Cannot_shred_file:
            return L"Failed to shred the file";
        case _App_error::_Cannot_delete_file:
            return L"Failed to delete the file";
        default:
            return L"(Unknown error)";
        }
    }

    inline void _Report_error(const _App_error _Error) noexcept {
        wchar_t _Msg[128] = {0}; // should fit longest possible message
        ::swprintf_s(_Msg,
            L"      An error occured during the program's runtime!\n\n"
            L"      Error message: %s", _Translate_app_error(_Error));
        report_error(_Msg);
    }

    inline _App_error _Unsafe_entry_point(program_args& _Args) {
        program_options _Options;
        program_args::parse(_Args, _Options);
        if (_Options.path_to_file.empty()) {
            return _App_error::_Target_not_specified;
        }

        if (_Options.confirmation_required) {
            confirmation_status _Status;
            if (_Options.delete_after_shredding) { // ask for permission to destroy the file
                _Status = confirm_operation(L"Destroy file",
                    L"      Are you sure you want to destroy this file?\n"
                    L"      This action cannot be undone.");
            } else { // ask for permission to destroy the file contents
                _Status = confirm_operation(L"Destroy file contents",
                    L"      Are you sure you want to destroy the contents of this file?\n"
                    L"      This action cannot be undone.");
            }

            if (_Status == confirmation_status::unconfirmed) { // no permission, do nothing
                return _App_error::_Success;
            }
        }

        {
            file_access _Access;
            file_flag _Flags;
            if (_Options.delete_after_shredding) { // request all access
                _Access = file_access::all;
                _Flags  = file_flag::delete_on_close;
            } else { // request write-only access
                _Access = file_access::write;
                _Flags  = file_flag::none;
            }

            file _File(_Options.path_to_file, _Access, file_share::none, _Flags);
            if (!_File.is_open() && _Options.delete_after_shredding) { // try to open with write-only access
                if (!_File.open(_Options.path_to_file, file_access::write, file_share::none, _Flags)) {
                    // could not grant the minimum required access, break
                    return _App_error::_Bad_file;
                }
            }

            if (!securely_shred_file(_File)) {
                return _App_error::_Cannot_shred_file;
            }
        } // closes and possibly deletes the file

        // Note: At this step, the file should be closed and, if the caller specified a special flag,
        //       deleted. Deletion of the file should be handled automatically based on the provided
        //       flags, so it is valid to check whether the file still exists at this point.
        if (_Options.delete_after_shredding && ::mjx::exists(_Options.path_to_file)) {
            return _App_error::_Cannot_delete_file;
        }

        return _App_error::_Success;
    }

    inline _App_error _Entry_point(program_args& _Args) noexcept {
        _App_error _Error;
        try {
            _Error = _Unsafe_entry_point(_Args);
        } catch (...) {
            _Error = _App_error::_Unknown_error;
        }

        if (_Error != _App_error::_Success) { // report an error
            _Report_error(_Error);
        }

        return _Error;
    }
} // namespace mjx

#ifndef _SAL_VERSION
#define _In_
#define _In_opt_
#endif // _SAL_VERSION

int __stdcall wWinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ wchar_t* _Combined_args, _In_ int) {
    ::mjx::program_args _Args(_Combined_args);
    return static_cast<int>(::mjx::_Entry_point(_Args));
}