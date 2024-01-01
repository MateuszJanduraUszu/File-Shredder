// object_allocator.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _MJMEM_OBJECT_ALLOCATOR_HPP_
#define _MJMEM_OBJECT_ALLOCATOR_HPP_
#include <mjmem/allocator.hpp>
#include <new>
#include <type_traits>

namespace mjx {
    template <class _Ty>
    class object_allocator { // type-specific wrapper around the global allocator
    public:
        static_assert(!::std::is_const_v<_Ty>, "T cannot be const");
        static_assert(!::std::is_reference_v<_Ty>, "T cannot be a reference");
        static_assert(!::std::is_function_v<_Ty>, "T cannot be a function object");
        static_assert(!::std::is_volatile_v<_Ty>, "T cannot be volatile");

        using value_type      = _Ty;
        using size_type       = allocator::size_type;
        using difference_type = allocator::difference_type;
        using pointer         = _Ty*;
        using const_pointer   = const _Ty*;
        using reference       = _Ty&;
        using const_reference = const _Ty&;

        template <class _Other>
        struct rebind {
            using other = object_allocator<_Other>;
        };

        object_allocator() noexcept                        = default;
        object_allocator(const object_allocator&) noexcept = default;
        object_allocator(object_allocator&&) noexcept      = default;
        ~object_allocator() noexcept                       = default;
    
        template <class _Other>
        object_allocator(const object_allocator<_Other>&) noexcept {}

        object_allocator& operator=(const object_allocator&) noexcept = default;
        object_allocator& operator=(object_allocator&&) noexcept      = default;

        template <class _Other>
        object_allocator& operator=(const object_allocator<_Other>&) noexcept {
            return *this;
        }

        pointer allocate(const size_type _Count) {
            return static_cast<pointer>(::mjx::get_allocator().allocate(_Count * sizeof(_Ty)));
        }

        pointer allocate_aligned(const size_type _Count, const size_type _Align) {
            return static_cast<pointer>(
                ::mjx::get_allocator().allocate_aligned(_Count * sizeof(_Ty), _Align));
        }

        void deallocate(pointer _Ptr, const size_type _Count) noexcept {
            ::mjx::get_allocator().deallocate(_Ptr, _Count * sizeof(_Ty));
        }

        size_type max_size() const noexcept {
            return ::mjx::get_allocator().max_size() / sizeof(_Ty);
        }

        bool is_equal(const object_allocator&) const noexcept {
            return true; // always equal
        }
    };

    template <class _Ty>
    inline bool operator==(const object_allocator<_Ty>& _Left, const object_allocator<_Ty>& _Right) noexcept {
        return true;
    }

    template <class _Ty>
    inline bool operator!=(const object_allocator<_Ty>& _Left, const object_allocator<_Ty>& _Right) noexcept {
        return false;
    }

    template <class _Ty, class... _Types>
    inline _Ty* create_object(_Types&&... _Args) {
        object_allocator<_Ty> _Al;
        return ::new (static_cast<void*>(_Al.allocate(1))) _Ty(::std::forward<_Types>(_Args)...);
    }

    template <class _Ty>
    inline void delete_object(_Ty* const _Obj) noexcept(::std::is_nothrow_destructible_v<_Ty>) {
        if constexpr (!::std::is_trivially_destructible_v<_Ty>) {
            _Obj->~_Ty();
        }

        object_allocator<_Ty> _Al;
        _Al.deallocate(_Obj, 1);
    }

    template <class _Ty>
    inline _Ty* allocate_object_array(const size_t _Count) {
        object_allocator<_Ty> _Al;
        return _Al.allocate(_Count);
    }

    template <class _Ty>
    inline void delete_object_array(
        _Ty* const _Objects, const size_t _Count) noexcept(::std::is_nothrow_destructible_v<_Ty>) {
        if constexpr (!::std::is_trivially_destructible_v<_Ty>) {
            for (size_t _Idx = 0; _Idx < _Count; ++_Idx) {
                _Objects[_Idx].~_Ty();
            }
        }

        object_allocator<_Ty> _Al;
        _Al.deallocate(_Objects, _Count);
    }
} // namespace mjx

#endif // _MJMEM_OBJECT_ALLOCATOR_HPP_