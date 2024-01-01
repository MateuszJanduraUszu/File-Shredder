// string_view.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _MJSTR_STRING_VIEW_HPP_
#define _MJSTR_STRING_VIEW_HPP_
#include <cstddef>
#include <iterator>
#include <mjstr/api.hpp>
#include <mjstr/char_traits.hpp>

namespace mjx {
    template <class _Elem>
    class _MJSTR_API string_view_iterator { // random access iterator for string_view<CharT, Traits>
    public:
        using value_type        = _Elem;
        using difference_type   = ptrdiff_t;
        using pointer           = const _Elem*;
        using reference         = const _Elem&;
        using iterator_category = ::std::random_access_iterator_tag;

        string_view_iterator() noexcept;
#ifdef _DEBUG
        string_view_iterator(pointer _First, pointer _Last) noexcept;
#else // ^^^ _DEBUG ^^^ / vvv NDEBUG vvv
        explicit string_view_iterator(pointer _Ptr) noexcept;
#endif // _DEBUG

        // returns the first character of the current substring
        reference operator*() const noexcept;

        // returns the current substring
        pointer operator->() const noexcept;

        // returns the element at specified offset
        reference operator[](const difference_type _Off) const noexcept;

        // advances the iterator to the next element
        string_view_iterator& operator++() noexcept;

        // advances the iterator to the next element (performs post-incrementation)
        string_view_iterator operator++(int) noexcept;

        // retreats the iterator to the previous element
        string_view_iterator& operator--() noexcept;
    
        // retreats the iterator to the previous element (performs post-decrementation)
        string_view_iterator operator--(int) noexcept;

        // advances the iterator by _Off elements
        string_view_iterator& operator+=(const difference_type _Off) noexcept;

        // retreats the iterator by _Off elements
        string_view_iterator& operator-=(const difference_type _Off) noexcept;

        // returns a new iterator that is _Off elements ahead the current one
        string_view_iterator operator+(const difference_type _Off) const noexcept;

        // returns a new iterator that is _Off elements behind the current one
        string_view_iterator operator-(const difference_type _Off) const noexcept;

        // checks if two iterators are equal
        bool operator==(const string_view_iterator& _Other) const noexcept;

        // checks if two iterators are not equal
        bool operator!=(const string_view_iterator& _Other) const noexcept;

        // checks if the current iterator is greater than other one
        bool operator>(const string_view_iterator& _Other) const noexcept;

        // checks if the current iterator is greater than or equal to other one
        bool operator>=(const string_view_iterator& _Other) const noexcept;
        
        // checks if the current iterator is less than other one
        bool operator<(const string_view_iterator& _Other) const noexcept;

        // checks if the current iterator is less than or equal to other one
        bool operator<=(const string_view_iterator& _Other) const noexcept;

    private:
        pointer _Myptr;
#ifdef _DEBUG
        pointer _Mybegin;
        pointer _Myend;
#endif // _DEBUG
    };

    using byte_string_view_iterator    = string_view_iterator<byte_t>;
    using utf8_string_view_iterator    = string_view_iterator<char>;
    using unicode_string_view_iterator = string_view_iterator<wchar_t>;

    template <class _Elem, class _Traits = char_traits<_Elem>>
    class _MJSTR_API string_view { // lightweight non-owning read-only view of a string
    public:
        static_assert(_Validate_elem<_Elem>, "invalid element type for string_view<CharT, Traits>");
        static_assert(_Validate_traits<_Elem, _Traits>, "invalid traits for string_view<CharT, Traits>");

        using value_type      = _Elem;
        using size_type       = size_t;
        using difference_type = ptrdiff_t;
        using pointer         = _Elem*;
        using const_pointer   = const _Elem*;
        using reference       = _Elem&;
        using const_reference = const _Elem&;
        using traits_type     = _Traits;

        using const_iterator = string_view_iterator<_Elem>;
        using iterator       = const_iterator;

        static constexpr size_type npos = static_cast<size_type>(-1);

        string_view() noexcept;
        string_view(const_pointer _Ptr, const size_type _Count) noexcept;
        string_view(const_pointer _Ptr) noexcept;

        string_view(const string_view&) noexcept            = default;
        string_view& operator=(const string_view&) noexcept = default;

        string_view(nullptr_t) = delete;

        // accesses the specified character
        const_reference operator[](const size_type _Idx) const noexcept;

        // returns an iterator to the beginning
        const_iterator begin() const noexcept;

        // returns an iterator to the end
        const_iterator end() const noexcept;

        // accesses the specified character with bounds checking
        const_reference at(const size_type _Idx) const;

        // accesses the first character
        const_reference front() const noexcept;

        // accesses the last character
        const_reference back() const noexcept;

        // returns a pointer to the first character of a view
        const_pointer data() const noexcept;

        // returns the number of characters
        size_type size() const noexcept;

        // checks whether the view is empty
        bool empty() const noexcept;

        // shrinks the view by moving its start forward
        void remove_prefix(const size_type _Count) noexcept;

        // shrinks the view by moving its end backward
        void remove_suffix(const size_type _Count) noexcept;

        // swaps the contents
        void swap(string_view& _Other) noexcept;

        // copies characters
        size_type copy(pointer _Dest, size_type _Count, const size_type _Off = 0) const;

        // returns a substring
        string_view substr(const size_type _Off = 0, size_type _Count = npos) const;

        // compares two views
        int compare(const string_view _Str) const noexcept;
        int compare(const_pointer _Ptr, const size_type _Count) const noexcept;
        int compare(const_pointer _Ptr) const noexcept;

        // checks if the string view starts with the given prefix
        bool starts_with(const string_view _Str) const noexcept;
        bool starts_with(const value_type _Ch) const noexcept;
        bool starts_with(const_pointer _Ptr) const noexcept;

        // checks if the string view ends with the given suffix
        bool ends_with(const string_view _Str) const noexcept;
        bool ends_with(const value_type _Ch) const noexcept;
        bool ends_with(const_pointer _Ptr) const noexcept;

        // checks if the string view contains the given substring or character
        bool contains(const string_view _Str) const noexcept;
        bool contains(const value_type _Ch) const noexcept;
        bool contains(const_pointer _Ptr) const noexcept;

        // find characters in the view
        size_type find(const string_view _Str, const size_type _Off = 0) const noexcept;
        size_type find(const value_type _Ch, const size_type _Off = 0) const noexcept;
        size_type find(
            const_pointer _Ptr, const size_type _Off, const size_type _Count) const noexcept;
        size_type find(const_pointer _Ptr, const size_type _Off = 0) const noexcept;

        // finds the last occurrence of a substring
        size_type rfind(const string_view _Str, const size_type _Off = npos) const noexcept;
        size_type rfind(const value_type _Ch, const size_type _Off = npos) const noexcept;
        size_type rfind(
            const_pointer _Ptr, const size_type _Off, const size_type _Count) const noexcept;
        size_type rfind(const_pointer _Ptr, const size_type _Off = npos) const noexcept;

    private:
        // throws an exception if offset is out of range
        void _Check_offset(const size_type _Off) const;

        const_pointer _Mydata;
        size_type _Mysize;
    };

    using byte_string_view    = string_view<byte_t, char_traits<byte_t>>;
    using utf8_string_view    = string_view<char, char_traits<char>>;
    using unicode_string_view = string_view<wchar_t, char_traits<wchar_t>>;

    template <class _Elem, class _Traits>
    inline bool operator==(
        const string_view<_Elem, _Traits> _Left, const string_view<_Elem, _Traits> _Right) noexcept {
        return _Left.compare(_Right) == 0;
    }

    template <class _Elem, class _Traits>
    inline bool operator==(const string_view<_Elem, _Traits> _Left, const _Elem* const _Right) noexcept {
        return _Left.compare(_Right) == 0;
    }

    template <class _Elem, class _Traits>
    inline bool operator==(const _Elem* const _Left, const string_view<_Elem, _Traits> _Right) noexcept {
        return _Right.compare(_Left) == 0;
    }

    template <class _Elem, class _Traits>
    inline bool operator!=(
        const string_view<_Elem, _Traits> _Left, const string_view<_Elem, _Traits> _Right) noexcept {
        return !(_Left == _Right);
    }

    template <class _Elem, class _Traits>
    inline bool operator!=(const string_view<_Elem, _Traits> _Left, const _Elem* const _Right) noexcept {
        return !(_Left == _Right);
    }

    template <class _Elem, class _Traits>
    inline bool operator!=(const _Elem* const _Left, const string_view<_Elem, _Traits> _Right) noexcept {
        return !(_Left == _Right);
    }
} // namespace mjx

#endif // _MJSTR_STRING_VIEW_HPP_