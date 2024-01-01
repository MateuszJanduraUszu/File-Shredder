// conversion.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _MJSTR_CONVERSION_HPP_
#define _MJSTR_CONVERSION_HPP_
#include <mjstr/api.hpp>
#include <mjstr/string.hpp>
#include <mjstr/string_view.hpp>

namespace mjx {
    _MJSTR_API byte_string to_byte_string(const utf8_string_view _Str);
    _MJSTR_API byte_string to_byte_string(const unicode_string_view _Str);

    _MJSTR_API utf8_string to_utf8_string(const byte_string_view _Str);
    _MJSTR_API utf8_string to_utf8_string(const unicode_string_view _Str);

    _MJSTR_API unicode_string to_unicode_string(const byte_string_view _Str);
    _MJSTR_API unicode_string to_unicode_string(const utf8_string_view _Str);
} // namespace mjx

#endif // _MJSTR_CONVERSION_HPP_