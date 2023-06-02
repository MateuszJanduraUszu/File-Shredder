// random.cpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#include <fshred/random.hpp>
#include <bcrypt.h>

namespace fshred {
    bool fill_with_random_bytes(byte_t* const _Buf, const size_t _Count) noexcept {
        using _Fn_t        = decltype(&::BCryptGenRandom);
        static _Fn_t _Func = _Load_symbol<_Fn_t>("Bcrypt.dll", "BCryptGenRandom"); // load once
        return _Func ? _Func(nullptr, _Buf, static_cast<unsigned long>(_Count),
            BCRYPT_USE_SYSTEM_PREFERRED_RNG) == 0 : false;
    }
} // namespace fshred