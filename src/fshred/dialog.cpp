// dialog.cpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#include <fshred/dialog.hpp>
#include <fshred/tinywin.hpp>

namespace mjx {
    confirmation_status confirm_operation(const wchar_t* const _Title, const wchar_t* const _Msg) noexcept {
        switch (::MessageBoxW(nullptr, _Msg, _Title, MB_ICONWARNING | MB_YESNO)) {
        case IDYES:
            return confirmation_status::confirmed;
        default:
            return confirmation_status::unconfirmed;
        }
    }

    void report_error(const wchar_t* const _Msg) noexcept {
        ::MessageBoxW(nullptr, _Msg, L"File Shredder - An error occured!", MB_ICONERROR | MB_OK);
    }
} // namespace mjx