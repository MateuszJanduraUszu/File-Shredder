// dialog.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _FSHRED_DIALOG_HPP_
#define _FSHRED_DIALOG_HPP_

namespace fshred {
    enum class confirmation_status : bool { unconfirmed, confirmed };

    confirmation_status confirm_operation(const wchar_t* const _Title, const wchar_t* const _Msg) noexcept;
} // namespace fshred

#endif // _FSHRED_DIALOG_HPP_