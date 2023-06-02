// random.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _FSHRED_RANDOM_HPP_
#define _FSHRED_RANDOM_HPP_
#include <fshred/utils.hpp>
#include <cstddef>

namespace fshred {
    bool fill_with_random_bytes(byte_t* const _Buf, const size_t _Count) noexcept;
} // namespace fshred

#endif // _FSHRED_RANDOM_HPP_