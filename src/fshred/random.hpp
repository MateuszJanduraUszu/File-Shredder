// random.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _FSHRED_RANDOM_HPP_
#define _FSHRED_RANDOM_HPP_
#include <cstddef>
#include <mjstr/char_traits.hpp>

namespace mjx {
    bool fill_with_random_bytes(byte_t* const _Buf, const size_t _Count) noexcept;
} // namespace mjx

#endif // _FSHRED_RANDOM_HPP_