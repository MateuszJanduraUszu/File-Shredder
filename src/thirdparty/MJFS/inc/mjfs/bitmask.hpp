// bitmask.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _MJFS_BITMASK_HPP_
#define _MJFS_BITMASK_HPP_
#include <type_traits>

#define _DECLARE_BIT_OPS(_Ty)                                                              \
    constexpr _Ty operator&(const _Ty _Left, const _Ty _Right) noexcept {                  \
        using _Int_t = ::std::underlying_type_t<_Ty>;                                      \
        return static_cast<_Ty>(static_cast<_Int_t>(_Left) & static_cast<_Int_t>(_Right)); \
    }                                                                                      \
                                                                                           \
    constexpr _Ty operator|(const _Ty _Left, const _Ty _Right) noexcept {                  \
        using _Int_t = ::std::underlying_type_t<_Ty>;                                      \
        return static_cast<_Ty>(static_cast<_Int_t>(_Left) | static_cast<_Int_t>(_Right)); \
    }                                                                                      \
                                                                                           \
    constexpr _Ty operator^(const _Ty _Left, const _Ty _Right) noexcept {                  \
        using _Int_t = ::std::underlying_type_t<_Ty>;                                      \
        return static_cast<_Ty>(static_cast<_Int_t>(_Left) ^ static_cast<_Int_t>(_Right)); \
    }                                                                                      \
                                                                                           \
    constexpr _Ty operator~(const _Ty _Val) noexcept {                                     \
        using _Int_t = ::std::underlying_type_t<_Ty>;                                      \
        return static_cast<_Ty>(~static_cast<_Int_t>(_Val));                               \
    }                                                                                      \
                                                                                           \
    constexpr _Ty operator&=(_Ty& _Mask, const _Ty _Val) noexcept {                        \
        _Mask = _Mask & _Val;                                                              \
        return _Mask;                                                                      \
    }                                                                                      \
                                                                                           \
    constexpr _Ty operator|=(_Ty& _Mask, const _Ty _Val) noexcept {                        \
        _Mask = _Mask | _Val;                                                              \
        return _Mask;                                                                      \
    }                                                                                      \
                                                                                           \
    constexpr _Ty operator^=(_Ty& _Mask, const _Ty _Val) noexcept {                        \
        _Mask = _Mask ^ _Val;                                                              \
        return _Mask;                                                                      \
    }

namespace mjx {
    template <class _Ty>
    constexpr bool _Has_bits(const _Ty _Bitmask, const _Ty _Bits) noexcept {
        return (_Bitmask & _Bits) != _Ty{0};
    }
} // namespace mjx

#endif // _MJFS_BITMASK_HPP_