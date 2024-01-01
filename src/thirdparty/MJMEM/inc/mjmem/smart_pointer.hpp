// smart_pointer.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _MJMEM_SMART_POINTER_HPP_
#define _MJMEM_SMART_POINTER_HPP_
#include <atomic>
#include <mjmem/exception.hpp>
#include <mjmem/object_allocator.hpp>
#include <type_traits>
#include <utility>

namespace mjx {
    template <class _Ty1, class _Ty2, bool = ::std::is_empty_v<_Ty2> && !::std::is_final_v<_Ty2>>
    class optimized_pair : public _Ty2 { // store T1 and derive from T2, optimization active
    public:
        using first_type  = _Ty1;
        using second_type = _Ty2;

        optimized_pair() noexcept(::std::is_nothrow_default_constructible_v<_Ty1>
            && ::std::is_nothrow_default_constructible_v<_Ty2>) : _Myval(), _Ty2() {}

        optimized_pair(const _Ty1& _Val1, const _Ty2& _Val2) noexcept(
            ::std::is_nothrow_copy_constructible_v<_Ty1> && ::std::is_nothrow_copy_constructible_v<_Ty2>)
            : _Myval(_Val1), _Ty2(_Val2) {}

        optimized_pair(_Ty1&& _Val1, _Ty2&& _Val2) noexcept(::std::is_nothrow_move_constructible_v<_Ty1>
            && ::std::is_nothrow_move_constructible_v<_Ty2>)
            : _Myval(::std::move(_Val1)), _Ty2(::std::move(_Val2)) {}

        optimized_pair(const optimized_pair&) = default;
        optimized_pair(optimized_pair&&)      = default;

        optimized_pair& operator=(const optimized_pair& _Other) noexcept(
            ::std::is_nothrow_copy_assignable_v<_Ty1> && ::std::is_nothrow_copy_assignable_v<_Ty2>) {
            if (this != ::std::addressof(_Other)) {
                _Myval                    = _Other._Myval;
                static_cast<_Ty2&>(*this) = static_cast<const _Ty2&>(_Other);
            }

            return *this;
        }

        optimized_pair& operator=(optimized_pair&& _Other) noexcept(
            ::std::is_nothrow_move_assignable_v<_Ty1> && ::std::is_nothrow_move_assignable_v<_Ty2>) {
            if (this != ::std::addressof(_Other)) {
                _Myval                    = ::std::move(_Other);
                static_cast<_Ty2&>(*this) = static_cast<_Ty2&&>(_Other);
            }

            return *this;
        }

        first_type& first() noexcept {
            return _Myval;
        }

        const first_type& first() const noexcept {
            return _Myval;
        }

        second_type& second() noexcept {
            return *this;
        }

        const second_type& second() const noexcept {
            return *this;
        }

        void swap(optimized_pair& _Other) noexcept(
            ::std::is_nothrow_swappable_v<_Ty1> && ::std::is_nothrow_swappable_v<_Ty2>) {
            ::std::swap(_Myval, _Other._Myval);
            ::std::swap(static_cast<_Ty2&>(*this), static_cast<_Ty2&>(_Other));
        }

    private:
        _Ty1 _Myval;
    };

    template <class _Ty1, class _Ty2>
    class optimized_pair<_Ty1, _Ty2, false> { // store both T1 and T2, optimization inactive
    public:
        using first_type  = _Ty1;
        using second_type = _Ty2;
    
        optimized_pair() noexcept(::std::is_nothrow_default_constructible_v<_Ty1>
            && ::std::is_nothrow_default_constructible_v<_Ty2>) : _Myval1(), _Myval2() {}

        optimized_pair(const _Ty1& _Val1, const _Ty2& _Val2) noexcept(
            ::std::is_nothrow_copy_constructible_v<_Ty1> && ::std::is_nothrow_copy_constructible_v<_Ty2>)
            : _Myval1(_Val1), _Myval2(_Val2) {}

        optimized_pair(_Ty1&& _Val1, _Ty2&& _Val2) noexcept(::std::is_nothrow_move_constructible_v<_Ty1>
            && ::std::is_nothrow_move_constructible_v<_Ty2>)
            : _Myval1(::std::move(_Val1)), _Myval2(::std::move(_Val2)) {}

        ~optimized_pair() noexcept {}

        optimized_pair(const optimized_pair&) = default;
        optimized_pair(optimized_pair&&)      = default;

        optimized_pair& operator=(const optimized_pair& _Other) noexcept(
            ::std::is_nothrow_copy_assignable_v<_Ty1> && ::std::is_nothrow_copy_assignable_v<_Ty2>) {
            if (this != ::std::addressof(_Other)) {
                _Myval1 = _Other._Myval1;
                _Myval2 = _Other._Myval2;
            }

            return *this;
        }

        optimized_pair& operator=(optimized_pair&& _Other) noexcept(
            ::std::is_nothrow_move_assignable_v<_Ty1> && ::std::is_nothrow_move_assignable_v<_Ty2>) {
            if (this != ::std::addressof(_Other)) {
                _Myval1 = ::std::move(_Other._Myval1);
                _Myval2 = ::std::move(_Other._Myval2);
            }

            return *this;
        }

        first_type& first() noexcept {
            return _Myval1;
        }

        const first_type& first() const noexcept {
            return _Myval1;
        }

        second_type& second() noexcept {
            return _Myval2;
        }

        const second_type& second() const noexcept {
            return _Myval2;
        }

        void swap(optimized_pair& _Other) noexcept(
            ::std::is_nothrow_swappable_v<_Ty1> && ::std::is_nothrow_swappable_v<_Ty2>) {
            ::std::swap(_Myval1, _Other._Myval1);
            ::std::swap(_Myval2, _Other._Myval2);
        }

    private:
        _Ty1 _Myval1;
        _Ty2 _Myval2;
    };

    template <class _Ty1, class _Ty2>
    inline bool operator==(
        const optimized_pair<_Ty1, _Ty2>& _Left, const optimized_pair<_Ty1, _Ty2>& _Right) {
        return _Left.first() == _Right.first() && _Left.second() == _Right.second();
    }

    template <class _Ty1, class _Ty2>
    inline bool operator!=(
        const optimized_pair<_Ty1, _Ty2>& _Left, const optimized_pair<_Ty1, _Ty2>& _Right) {
        return !(_Left == _Right);
    }

    template <class _Ty1, class _Ty2>
    inline optimized_pair<_Ty1, _Ty2> make_optimized_pair(_Ty1&& _Val1, _Ty2&& _Val2) noexcept(
        ::std::is_nothrow_move_constructible_v<_Ty1> && ::std::is_nothrow_move_constructible_v<_Ty2>) {
        return optimized_pair<_Ty1, _Ty2>(::std::move(_Val1), ::std::move(_Val2));
    }

    template <class _Ty>
    class unique_smart_ptr { // smart pointer with unique object ownership semantics
    public:
        using element_type = _Ty;
        using pointer      = _Ty*;

        unique_smart_ptr() noexcept : _Myptr(nullptr) {}

        unique_smart_ptr(nullptr_t) noexcept : _Myptr(nullptr) {}

        explicit unique_smart_ptr(pointer _Ptr) noexcept : _Myptr(_Ptr) {}

        unique_smart_ptr(unique_smart_ptr&& _Other) noexcept : _Myptr(_Other.release()) {}

        ~unique_smart_ptr() noexcept {
            if (_Myptr) {
                ::mjx::delete_object(_Myptr);
                _Myptr = nullptr;
            }
        }

        unique_smart_ptr& operator=(unique_smart_ptr&& _Other) noexcept {
            reset(_Other.release());
            return *this;
        }

        unique_smart_ptr& operator=(nullptr_t) noexcept {
            reset();
            return *this;
        }

        unique_smart_ptr(const unique_smart_ptr&)            = delete;
        unique_smart_ptr& operator=(const unique_smart_ptr&) = delete;

        element_type& operator*() const noexcept(noexcept(*::std::declval<pointer>())) {
            return *_Myptr;
        }

        pointer operator->() const noexcept {
            return _Myptr;
        }

        explicit operator bool() const noexcept {
            return _Myptr != nullptr;
        }

        pointer get() const noexcept {
            return _Myptr;
        }

        pointer release() noexcept {
            pointer _Old_ptr = _Myptr;
            _Myptr           = nullptr;
            return _Old_ptr;
        }

        void reset(pointer _New_ptr = nullptr) noexcept {
            if (_Myptr) {
                ::mjx::delete_object(_Myptr);
            }

            _Myptr = _New_ptr;
        }

        void swap(unique_smart_ptr& _Other) noexcept {
            ::std::swap(_Myptr, _Other._Myptr);
        }

    private:
        pointer _Myptr;
    };

    template <class _Ty>
    inline bool operator==(const unique_smart_ptr<_Ty>& _Left, const unique_smart_ptr<_Ty>& _Right) noexcept {
        return _Left.get() == _Right.get();
    }

    template <class _Ty>
    inline bool operator==(const unique_smart_ptr<_Ty>& _Left, nullptr_t) noexcept {
        return !_Left;
    }

    template <class _Ty>
    inline bool operator==(nullptr_t, const unique_smart_ptr<_Ty>& _Right) noexcept {
        return !_Right;
    }

    template <class _Ty>
    inline bool operator!=(const unique_smart_ptr<_Ty>& _Left, const unique_smart_ptr<_Ty>& _Right) noexcept {
        return _Left.get() != _Right.get();
    }

    template <class _Ty>
    inline bool operator!=(const unique_smart_ptr<_Ty>& _Left, nullptr_t) noexcept {
        return static_cast<bool>(_Left);
    }

    template <class _Ty>
    inline bool operator!=(nullptr_t, const unique_smart_ptr<_Ty>& _Right) noexcept {
        return static_cast<bool>(_Right);
    }

    template <class _Ty, class... _Types>
    inline unique_smart_ptr<_Ty> make_unique_smart_ptr(_Types&&... _Args) {
        return unique_smart_ptr<_Ty>(::mjx::create_object<_Ty>(::std::forward<_Types>(_Args)...));
    }

    template <class _Ty>
    class unique_smart_array { // smart pointer with unique object ownership semantics (for arrays)
    public:
        using element_type = _Ty;
        using pointer      = _Ty*;

        struct array {
            pointer ptr = nullptr;
            size_t size = 0;
        };

        unique_smart_array() noexcept : _Myptr(nullptr), _Mysize(0) {}

        unique_smart_array(nullptr_t) noexcept : _Myptr(nullptr), _Mysize(0) {}

        unique_smart_array(pointer _Ptr, const size_t _Size) noexcept : _Myptr(_Ptr), _Mysize(_Size) {}

        unique_smart_array(unique_smart_array&& _Other) noexcept
            : _Myptr(_Other._Myptr), _Mysize(_Other._Mysize) {
            _Other._Myptr  = nullptr;
            _Other._Mysize = 0;
        }

        ~unique_smart_array() noexcept {
            if (_Myptr) {
                ::mjx::delete_object_array(_Myptr, _Mysize);
                _Myptr  = nullptr;
                _Mysize = 0;
            }
        }

        unique_smart_array& operator=(unique_smart_array&& _Other) noexcept {
            const array& _Array = _Other.release();
            reset(_Array.ptr, _Array.size);
            return *this;
        }

        unique_smart_array& operator=(nullptr_t) noexcept {
            reset();
            return *this;
        }

        unique_smart_array(const unique_smart_array&)            = delete;
        unique_smart_array& operator=(const unique_smart_array&) = delete;

        element_type& operator*() const noexcept {
            return *_Myptr;
        }

        pointer operator->() const noexcept {
            return _Myptr;
        }

        element_type& operator[](const size_t _Idx) const {
            if (_Idx >= _Mysize) {
                resource_overrun::raise();
            }

            return _Myptr[_Idx];
        }

        explicit operator bool() const noexcept {
            return _Myptr != nullptr;
        }

        pointer get() const noexcept {
            return _Myptr;
        }

        size_t size() const noexcept {
            return _Mysize;
        }

        array release() noexcept {
            array _Old_array = {_Myptr, _Mysize};
            _Myptr           = nullptr;
            _Mysize          = 0;
            return _Old_array;
        }

        void reset(pointer _New_ptr, const size_t _New_size) noexcept {
            if (_Myptr) {
                ::mjx::delete_object_array(_Myptr, _Mysize);
            }

            _Myptr  = _New_ptr;
            _Mysize = _New_size;
        }

        void reset(nullptr_t = nullptr) noexcept {
            if (_Myptr) {
                ::mjx::delete_object_array(_Myptr, _Mysize);
                _Myptr  = nullptr;
                _Mysize = 0;
            }
        }

        void swap(unique_smart_array& _Other) noexcept {
            ::std::swap(_Myptr, _Other._Myptr);
            ::std::swap(_Mysize, _Other._Mysize);
        }

    private:
        pointer _Myptr;
        size_t _Mysize;
    };

    template <class _Ty>
    inline bool operator==(
        const unique_smart_array<_Ty>& _Left, const unique_smart_array<_Ty>& _Right) noexcept {
        return _Left.get() == _Right.get();
    }

    template <class _Ty>
    inline bool operator==(const unique_smart_array<_Ty>& _Left, nullptr_t) noexcept {
        return !_Left;
    }

    template <class _Ty>
    inline bool operator==(nullptr_t, const unique_smart_array<_Ty>& _Right) noexcept {
        return !_Right;
    }

    template <class _Ty>
    inline bool operator!=(
        const unique_smart_array<_Ty>& _Left, const unique_smart_array<_Ty>& _Right) noexcept {
        return _Left.get() != _Right.get();
    }

    template <class _Ty>
    inline bool operator!=(const unique_smart_array<_Ty>& _Left, nullptr_t) noexcept {
        return static_cast<bool>(_Left);
    }

    template <class _Ty>
    inline bool operator!=(nullptr_t, const unique_smart_array<_Ty>& _Right) noexcept {
        return static_cast<bool>(_Right);
    }

    template <class _Ty, class... _Types>
    inline unique_smart_array<_Ty> make_unique_smart_array(const size_t _Size) {
        return unique_smart_array<_Ty>(::mjx::allocate_object_array<_Ty>(_Size), _Size);
    }

    class _MJMEM_API reference_counter { // thread-safe reference counter
    public:
        reference_counter() noexcept;
        ~reference_counter() noexcept;

        explicit reference_counter(const long _Refs) noexcept;
        
        reference_counter(const reference_counter&)            = delete;
        reference_counter& operator=(const reference_counter&) = delete;

        // increments the number of references
        void increment() noexcept;

        // decrements the number of references
        long decrement() noexcept;

        // returns the number of references
        long use_count() const noexcept;

        // checks whether the number of references is exactly 1
        bool unique() const noexcept;

    private:
#pragma warning(suppress : 4251) // C4251: std::atomic<long> needs to have dll-interface
        ::std::atomic<long> _Myrefs;
    };

    template <class _Ty>
    class smart_ptr { // smart pointer with shared object ownership semantics
    public:
        using element_type = _Ty;
        using pointer      = _Ty*;

        smart_ptr() noexcept : _Myptr(nullptr), _Myctr(nullptr) {}

        smart_ptr(nullptr_t) noexcept : _Myptr(nullptr), _Myctr(nullptr) {}

        explicit smart_ptr(pointer _Ptr)
            : _Myptr(_Ptr), _Myctr(::mjx::create_object<reference_counter>()) {}

        smart_ptr(unique_smart_ptr<_Ty>&& _Sptr)
            : _Myptr(_Sptr.release()), _Myctr(::mjx::create_object<reference_counter>()) {}

        smart_ptr(const smart_ptr& _Other) noexcept : _Myptr(_Other._Myptr), _Myctr(_Other._Myctr) {
            if (_Myctr) {
                _Myctr->increment();
            }
        }

        smart_ptr(smart_ptr&& _Other) noexcept : _Myptr(_Other._Myptr), _Myctr(_Other._Myctr) {
            _Other._Myptr = nullptr;
            _Other._Myctr = nullptr;
        }

        ~smart_ptr() noexcept {
            _Release();
        }

        smart_ptr& operator=(const smart_ptr& _Other) noexcept {
            smart_ptr(_Other).swap(*this);
            return *this;
        }

        smart_ptr& operator=(smart_ptr&& _Other) noexcept {
            smart_ptr(::std::move(_Other)).swap(*this);
            return *this;
        }

        smart_ptr& operator=(unique_smart_ptr<_Ty>&& _Sptr) noexcept {
            smart_ptr(::std::move(_Sptr)).swap(*this);
            return *this;
        }

        explicit operator bool() const noexcept {
            return _Myptr != nullptr;
        }

        element_type& operator*() const noexcept {
            // the behavior is undefined if the stored pointer is null
            return *_Myptr;
        }

        pointer operator->() const noexcept {
            return _Myptr;
        }

        pointer get() const noexcept {
            return _Myptr;
        }

        long use_count() const noexcept {
            return _Myctr ? _Myctr->use_count() : 0;
        }

        bool unique() const noexcept {
            return use_count() == 1;
        }

        void reset() noexcept {
            smart_ptr().swap(*this);
        }

        void reset(pointer _New_ptr) {
            smart_ptr(_New_ptr).swap(*this);
        }

        void swap(smart_ptr& _Other) noexcept {
            ::std::swap(_Myptr, _Other._Myptr);
            ::std::swap(_Myctr, _Other._Myctr);
        }

    private:
        void _Release() noexcept {
            if (_Myctr) {
                if (_Myctr->decrement() == 0) {
                    ::mjx::delete_object(_Myptr);
                    ::mjx::delete_object(_Myctr);
                    _Myptr = nullptr;
                    _Myctr = nullptr;
                }
            }
        }

        pointer _Myptr;
        reference_counter* _Myctr;
    };

    template <class _Ty>
    inline bool operator==(const smart_ptr<_Ty>& _Left, const smart_ptr<_Ty>& _Right) noexcept {
        return _Left.get() == _Right.get();
    }

    template <class _Ty>
    inline bool operator==(const smart_ptr<_Ty>& _Left, nullptr_t) noexcept {
        return !_Left;
    }

    template <class _Ty>
    inline bool operator==(nullptr_t, const smart_ptr<_Ty>& _Right) noexcept {
        return !_Right;
    }

    template <class _Ty>
    inline bool operator!=(const smart_ptr<_Ty>& _Left, const smart_ptr<_Ty>& _Right) noexcept {
        return _Left.get() != _Right.get();
    }

    template <class _Ty>
    inline bool operator!=(const smart_ptr<_Ty>& _Left, nullptr_t) noexcept {
        return static_cast<bool>(_Left);
    }

    template <class _Ty>
    inline bool operator!=(nullptr_t, const smart_ptr<_Ty>& _Right) noexcept {
        return static_cast<bool>(_Right);
    }

    template <class _Ty, class... _Types>
    inline smart_ptr<_Ty> make_smart_ptr(_Types&&... _Args) {
        return smart_ptr<_Ty>(::mjx::create_object<_Ty>(::std::forward<_Types>(_Args)...));
    }

    template <class _Ty>
    class smart_array { // smart pointer with shared object ownership semantics (for arrays)
    public:
        using element_type = _Ty;
        using pointer      = _Ty*;

        smart_array() noexcept : _Myptr(nullptr), _Mysize(0), _Myctr(nullptr) {}

        smart_array(nullptr_t) noexcept : _Myptr(nullptr), _Mysize(0), _Myctr(nullptr) {}

        smart_array(pointer _Ptr, const size_t _Size)
            : _Myptr(_Ptr), _Mysize(_Size), _Myctr(::mjx::create_object<reference_counter>()) {}

        smart_array(unique_smart_array<_Ty>&& _Sarr) : _Myptr(nullptr), _Mysize(0), _Myctr(nullptr) {
            const auto& _Array = _Sarr.release();
            _Myptr             = _Array.ptr;
            _Mysize            = _Array.size;
            _Myctr             = ::mjx::create_object<reference_counter>();
        }

        smart_array(const smart_array& _Other) noexcept
            : _Myptr(_Other._Myptr), _Mysize(_Other._Mysize), _Myctr(_Other._Myctr) {
            if (_Myctr) {
                _Myctr->increment();
            }
        }

        smart_array(smart_array&& _Other) noexcept
            : _Myptr(_Other._Myptr), _Mysize(_Other._Mysize), _Myctr(_Other._Myctr) {
            _Other._Myptr  = nullptr;
            _Other._Mysize = 0;
            _Other._Myctr  = nullptr;
        }

        ~smart_array() noexcept {
            _Release();
        }

        smart_array& operator=(const smart_array& _Other) noexcept {
            smart_array(_Other).swap(*this);
            return *this;
        }

        smart_array& operator=(smart_array&& _Other) noexcept {
            smart_array(::std::move(_Other)).swap(*this);
            return *this;
        }

        smart_array& operator=(unique_smart_array<_Ty>&& _Sarr) noexcept {
            smart_array(::std::move(_Sarr)).swap(*this);
            return *this;
        }

        explicit operator bool() const noexcept {
            return _Myptr != nullptr;
        }

        element_type& operator*() const noexcept {
            // the behavior is undefined if the stored pointer is null
            return *_Myptr;
        }

        pointer operator->() const noexcept {
            return _Myptr;
        }

        element_type& operator[](const size_t _Idx) const {
            if (_Idx >= _Mysize) {
                resource_overrun::raise();
            }

            return _Myptr[_Idx];
        }

        pointer get() const noexcept {
            return _Myptr;
        }

        long use_count() const noexcept {
            return _Myctr ? _Myctr->use_count() : 0;
        }

        bool unique() const noexcept {
            return use_count() == 1;
        }

        void reset() noexcept {
            smart_array().swap(*this);
        }

        void reset(pointer _New_ptr, const size_t _New_size) {
            smart_array(_New_ptr, _New_size).swap(*this);
        }

        void swap(smart_array& _Other) noexcept {
            ::std::swap(_Myptr, _Other._Myptr);
            ::std::swap(_Mysize, _Other._Mysize);
            ::std::swap(_Myctr, _Other._Myctr);
        }

    private:
        void _Release() noexcept {
            if (_Myctr) {
                if (_Myctr->decrement() == 0) {
                    ::mjx::delete_object_array(_Myptr, _Mysize);
                    ::mjx::delete_object(_Myctr);
                    _Myptr  = nullptr;
                    _Mysize = 0;
                    _Myctr  = nullptr;
                }
            }
        }

        pointer _Myptr;
        size_t _Mysize;
        reference_counter* _Myctr;
    };

    template <class _Ty>
    inline bool operator==(const smart_array<_Ty>& _Left, const smart_array<_Ty>& _Right) noexcept {
        return _Left.get() == _Right.get();
    }

    template <class _Ty>
    inline bool operator==(const smart_array<_Ty>& _Left, nullptr_t) noexcept {
        return !_Left;
    }

    template <class _Ty>
    inline bool operator==(nullptr_t, const smart_array<_Ty>& _Right) noexcept {
        return !_Right;
    }

    template <class _Ty>
    inline bool operator!=(const smart_array<_Ty>& _Left, const smart_array<_Ty>& _Right) noexcept {
        return _Left.get() != _Right.get();
    }

    template <class _Ty>
    inline bool operator!=(const smart_array<_Ty>& _Left, nullptr_t) noexcept {
        return static_cast<bool>(_Left);
    }

    template <class _Ty>
    inline bool operator!=(nullptr_t, const smart_array<_Ty>& _Right) noexcept {
        return static_cast<bool>(_Right);
    }

    template <class _Ty>
    inline smart_array<_Ty> make_smart_array(const size_t _Size) {
        return smart_array<_Ty>(::mjx::allocate_object_array<_Ty>(_Size), _Size);
    }
} // namespace mjx

#endif // _MJMEM_SMART_POINTER_HPP_