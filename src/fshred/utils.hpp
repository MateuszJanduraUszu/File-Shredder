// utils.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _FSHRED_UTILS_HPP_
#define _FSHRED_UTILS_HPP_
#include <string>
#include <string_view>
#include <Windows.h>

namespace fshred {
    using byte_t           = unsigned char;
    using byte_string      = ::std::basic_string<byte_t>;
    using byte_string_view = ::std::basic_string_view<byte_t>;

    class _Library_handle {
    public:
        explicit _Library_handle(const char* const _Name) noexcept : _Myptr(::LoadLibraryA(_Name)) {}

        ~_Library_handle() noexcept {
            if (_Myptr) {
                ::FreeLibrary(_Myptr);
                _Myptr = nullptr;
            }
        }

        bool _Valid() const noexcept {
            return _Myptr != nullptr;
        }

        HMODULE _Get() noexcept {
            return _Myptr;
        }

    private:
        HMODULE _Myptr;
    };

    template <class _Fn>
    inline _Fn _Load_symbol(HMODULE _Handle, const char* const _Symbol) noexcept {
        return reinterpret_cast<_Fn>(::GetProcAddress(_Handle, _Symbol));
    }

    template <class _Fn>
    inline _Fn _Load_symbol(const char* const _Lib, const char* const _Symbol) noexcept {
        _Library_handle _Handle(_Lib);
        if (!_Handle._Valid()) {
            return nullptr;
        }

        return _Load_symbol<_Fn>(_Handle._Get(), _Symbol);
    }
} // namespace fshred

#endif // _FSHRED_UTILS_HPP_