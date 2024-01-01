// string.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _MJSTR_STRING_HPP_
#define _MJSTR_STRING_HPP_
#include <cstddef>
#include <iterator>
#include <mjstr/api.hpp>
#include <mjstr/char_traits.hpp>
#include <mjstr/string_view.hpp>

namespace mjx {
    template <class _Elem>
    class _MJSTR_API string_const_iterator { // random access constant iterator for string<CharT, Traits>
    public:
        using value_type        = _Elem;
        using difference_type   = ptrdiff_t;
        using pointer           = const _Elem*;
        using reference         = const _Elem&;
        using iterator_category = ::std::random_access_iterator_tag;

        string_const_iterator() noexcept;
#ifdef _DEBUG
        string_const_iterator(pointer _First, pointer _Last) noexcept;
#else // ^^^ _DEBUG ^^^ / vvv NDEBUG vvv
        explicit string_const_iterator(pointer _Ptr) noexcept;
#endif // _DEBUG

        // returns the first character of the current substring
        reference operator*() const noexcept;

        // returns the current substring
        pointer operator->() const noexcept;

        // returns the element at specified offset
        reference operator[](const difference_type _Off) const noexcept;

        // advances the iterator to the next element
        string_const_iterator& operator++() noexcept;

        // advances the iterator to the next element (performs post-incrementation)
        string_const_iterator operator++(int) noexcept;

        // retreats the iterator to the previous element
        string_const_iterator& operator--() noexcept;

        // retreats the iterator to the previous element (performs post-decrementation)
        string_const_iterator operator--(int) noexcept;

        // advances the iterator by _Off elements
        string_const_iterator& operator+=(const difference_type _Off) noexcept;

        // retreats the iterator by _Off elements
        string_const_iterator& operator-=(const difference_type _Off) noexcept;

        // returns a new iterator that is _Off elements ahead the current one
        string_const_iterator operator+(const difference_type _Off) const noexcept;

        // returns a new iterator that is _Off elements behind the current one
        string_const_iterator operator-(const difference_type _Off) const noexcept;

        // checks if two iterators are equal
        bool operator==(const string_const_iterator& _Other) const noexcept;

        // checks if two iterators are not equal
        bool operator!=(const string_const_iterator& _Other) const noexcept;

        // checks if the current iterator is greater than other one
        bool operator>(const string_const_iterator& _Other) const noexcept;

        // checks if the current iterator is greater than or equal to other one
        bool operator>=(const string_const_iterator& _Other) const noexcept;

        // checks if the current iterator is less than other one
        bool operator<(const string_const_iterator& _Other) const noexcept;

        // checks if the current iterator is less than or equal to other one
        bool operator<=(const string_const_iterator& _Other) const noexcept;

    private:
        template <class, class>
        friend class string;

        pointer _Myptr;
#ifdef _DEBUG
        pointer _Mybegin;
        pointer _Myend;
#endif // _DEBUG
    };

    using byte_string_const_iterator    = string_const_iterator<byte_t>;
    using utf8_string_const_iterator    = string_const_iterator<char>;
    using unicode_string_const_iterator = string_const_iterator<wchar_t>;
    
    template <class _Elem>
    class _MJSTR_API string_iterator
        : public string_const_iterator<_Elem> { // random access iterator for string<CharT, Traits>
    private:
        using _Mybase = string_const_iterator<_Elem>;

    public:
        using value_type        = _Elem;
        using difference_type   = ptrdiff_t;
        using pointer           = _Elem*;
        using reference         = _Elem&;
        using iterator_category = ::std::random_access_iterator_tag;
    
        string_iterator() noexcept;
#ifdef _DEBUG
        string_iterator(pointer _First, pointer _Last) noexcept;
#else // ^^^ _DEBUG ^^^ / vvv NDEBUG vvv
        explicit string_iterator(pointer _Ptr) noexcept;
#endif // _DEBUG

        // returns the first character of the current substring
        reference operator*() const noexcept;

        // returns the current substring
        pointer operator->() const noexcept;

        // returns the element at specified offset
        reference operator[](const difference_type _Off) const noexcept;

        // advances the iterator to the next element
        string_iterator& operator++() noexcept;

        // advances the iterator to the next element (performs post-incrementation)
        string_iterator operator++(int) noexcept;

        // retreats the iterator to the previous element
        string_iterator& operator--() noexcept;

        // retreats the iterator to the previous element (performs post-decrementation)
        string_iterator operator--(int) noexcept;

        // advances the iterator by _Off elements
        string_iterator& operator+=(const difference_type _Off) noexcept;

        // retreats the iterator by _Off elements
        string_iterator& operator-=(const difference_type _Off) noexcept;

        // returns a new iterator that is _Off elements ahead the current one
        string_iterator operator+(const difference_type _Off) const noexcept;

        // returns a new iterator that is _Off elements behind the current one
        string_iterator operator-(const difference_type _Off) const noexcept;
    };

    using byte_string_iterator    = string_iterator<byte_t>;
    using utf8_string_iterator    = string_iterator<char>;
    using unicode_string_iterator = string_iterator<wchar_t>;

    template <class _Elem, class _Traits = char_traits<_Elem>>
    class _MJSTR_API string {
    public:
        static_assert(_Validate_elem<_Elem>, "invalid element type for string<CharT, Traits>");
        static_assert(_Validate_traits<_Elem, _Traits>, "invalid traits type for string<CharT, Traits>");

        using value_type      = _Elem;
        using size_type       = size_t;
        using difference_type = ptrdiff_t;
        using pointer         = _Elem*;
        using const_pointer   = const _Elem*;
        using reference       = _Elem&;
        using const_reference = const _Elem&;
        using traits_type     = _Traits;

        using const_iterator = string_const_iterator<_Elem>;
        using iterator       = string_iterator<_Elem>;

        static constexpr size_type npos = static_cast<size_type>(-1);

        string() noexcept;
        string(const string& _Other);
        string(string&& _Other) noexcept;
        ~string() noexcept;

        string(const size_type _Count, const value_type _Ch);
        string(const_pointer _Ptr, const size_type _Count);
        string(const_pointer _Ptr);
        string(const string_view<_Elem, _Traits> _Str);

        string(nullptr_t)            = delete;
        string& operator=(nullptr_t) = delete;

        // returns a non-modifiable string_view into the entire string
        operator string_view<_Elem, _Traits>() const noexcept;

        // assigns characters to the string
        string& operator=(const string& _Str);
        string& operator=(string&& _Str) noexcept;
        string& operator=(const_pointer _Ptr);
        string& operator=(const value_type _Ch);
        string& operator=(const string_view<_Elem, _Traits> _Str);

        // appends characters to the end
        string& operator+=(const string& _Str);
        string& operator+=(const_pointer _Ptr);
        string& operator+=(const value_type _Ch);
        string& operator+=(const string_view<_Elem, _Traits> _Str);

        // accesses the specified character
        reference operator[](const size_type _Idx) noexcept;
        const_reference operator[](const size_type _Idx) const noexcept;

        // returns an iterator to the beginning
        iterator begin() noexcept;
        const_iterator begin() const noexcept;

        // returns an iterator to the end
        iterator end() noexcept;
        const_iterator end() const noexcept;

        // accesses the specified character with bounds checking
        reference at(const size_type _Idx);
        const_reference at(const size_type _Idx) const;

        // accesses the first character
        reference front() noexcept;
        const_reference front() const noexcept;

        // accesses the last character
        reference back() noexcept;
        const_reference back() const noexcept;

        // returns a pointer to the first character of a string
        pointer data() noexcept;
        const_pointer data() const noexcept;

        // returns a non-modifiable standard C character array version of the string
        const_pointer c_str() const noexcept;

        // checks whether the string is empty
        bool empty() const noexcept;

        // returns the number of characters that can be held in currently allocated storage
        size_type capacity() const noexcept;

        // returns the number of characters
        size_type size() const noexcept;

        // returns the maximum number of characters
        static size_type max_size() noexcept;
        
        // returns the string as a view
        string_view<_Elem, _Traits> view() const noexcept;

        // reserves storage
        void reserve(size_type _New_capacity);

        // copies characters
        size_type copy(pointer _Dest, size_type _Count, const size_type _Off = 0) const;

        // swaps the contents
        void swap(string& _Other) noexcept;

        // clears the contents
        void clear() noexcept;

        // changes the number of characters stored
        void resize(const size_type _New_size, const value_type _Ch = value_type{});

        // increases the number of characters stored
        void expand(const size_type _Count, const value_type _Ch = value_type{});

        // decreases the number of characters stored
        void shrink(const size_type _Count) noexcept;

        // reduces memory usage by freeing unused memory
        void shrink_to_fit();

        // assigns characters to the string
        string& assign(const size_type _Count, const value_type _Ch);
        string& assign(const string& _Str);
        string& assign(string&& _Str) noexcept;
        string& assign(const_pointer _Ptr, const size_type _Count);
        string& assign(const_pointer _Ptr);
        string& assign(const string_view<_Elem, _Traits> _Str);

        // appends characters to the end
        string& append(const size_type _Count, const value_type _Ch);
        string& append(const string& _Str);
        string& append(const_pointer _Ptr, const size_type _Count);
        string& append(const_pointer _Ptr);
        string& append(const string_view<_Elem, _Traits> _Str);

        // appends a character to the end
        void push_back(const value_type _Ch);

        // remvoes the last character
        void pop_back() noexcept;

        // removes characters
        string& erase(const size_type _Off = 0, size_type _Count = npos);
        iterator erase(const const_iterator _Where);
        iterator erase(const const_iterator _First, const const_iterator _Last);

        // inserts characters
        string& insert(const size_type _Off, const size_type _Count, const value_type _Ch);
        string& insert(const size_type _Off, const_pointer _Ptr);
        string& insert(const size_type _Off, const_pointer _Ptr, const size_type _Count);
        string& insert(const size_type _Off, const string& _Str);
        iterator insert(const const_iterator _Where, const value_type _Ch);
        string& insert(const size_type _Off, const string_view<_Elem, _Traits> _Str);

        // replaces specified portion of the string
        string& replace(const size_type _Off, size_type _Count, const string& _Str);
        string& replace(const const_iterator _First, const const_iterator _Last, const string& _Str);
        string& replace(
            const size_type _Off, size_type _Count, const_pointer _Ptr, const size_type _Ptr_count);
        string& replace(const const_iterator _First,
            const const_iterator _Last, const_pointer _Ptr, const size_type _Count);
        string& replace(const size_type _Off, size_type _Count, const_pointer _Ptr);
        string& replace(const const_iterator _First, const const_iterator _Last, const_pointer _Ptr);
        string& replace(
            const size_type _Off, size_type _Count, const size_type _Ch_count, const value_type _Ch);
        string& replace(const const_iterator _First,
            const const_iterator _Last, const size_type _Count, const value_type _Ch);
        string& replace(const size_type _Off, size_type _Count, const string_view<_Elem, _Traits> _Str);
        string& replace(
            const const_iterator _First, const const_iterator _Last, const string_view<_Elem, _Traits> _Str);

        // finds the first occurrence of the given substring
        size_type find(const string& _Str, const size_type _Off = 0) const;
        size_type find(
            const_pointer _Ptr, const size_type _Off, const size_type _Count) const noexcept;
        size_type find(const value_type _Ch, const size_type _Off = 0) const noexcept;
        size_type find(
            const string_view<_Elem, _Traits> _Str, const size_type _Off = 0) const noexcept;

        // finds the last occurrence of the given substring
        size_type rfind(const string& _Str, const size_type _Off = npos) const;
        size_type rfind(
            const_pointer _Ptr, const size_type _Off, const size_type _Count) const noexcept;
        size_type rfind(const_pointer _Ptr, const size_type _Off = npos) const noexcept;
        size_type rfind(const value_type _Ch, const size_type _Off = npos) const noexcept;
        size_type rfind(
            const string_view<_Elem, _Traits> _Str, const size_type _Off = npos) const noexcept;

        // compares two strings
        int compare(const string& _Str) const;
        int compare(const_pointer _Ptr, const size_type _Count) const noexcept;
        int compare(const_pointer _Ptr) const noexcept;
        int compare(const string_view<_Elem, _Traits> _Str) const noexcept;

        // checks if the string starts with the given prefix
        bool starts_with(const string_view<_Elem, _Traits> _Str) const noexcept;
        bool starts_with(const value_type _Ch) const noexcept;
        bool starts_with(const_pointer _Ptr) const noexcept;

        // checks if the string ends with the given suffix
        bool ends_with(const string_view<_Elem, _Traits> _Str) const noexcept;
        bool ends_with(const value_type _Ch) const noexcept;
        bool ends_with(const_pointer _Ptr) const noexcept;

        // checks if the string contains the given substring or character
        bool contains(const string_view<_Elem, _Traits> _Str) const noexcept;
        bool contains(const value_type _Ch) const noexcept;
        bool contains(const_pointer _Ptr) const noexcept;

        // returns a substring
        string substr(const size_type _Off = 0, size_type _Count = npos) const;

    private:
        // allocates memory for the string capacity
        static pointer _Allocate_space_for_capacity(size_type& _Count);

        // destroys the string
        void _Tidy() noexcept;

        // throws an exception if the specified offset is out of range
        void _Check_offset(const size_type _Off) const;

        // throws an exception if the specified offset is out of range (special case)
        void _Check_offset_for_insertion(const size_type _Off) const;

        // steals the contents of another string
        void _Take_contents(string& _Other) noexcept;

        // constructs the string from a pointer
        void _Construct_from_ptr(const_pointer _Ptr, const size_type _Count);

        // constructs the string from a character sequence
        void _Construct_from_chars(const size_type _Count, const value_type _Ch);

        // resizes buffer capacity and stores new data at the beginning
        void _Reallocate_assign(const size_type _Count, const value_type _Ch);
        void _Reallocate_assign(const_pointer _Ptr, const size_type _Count);

        // increases buffer capacity and stores new data at the end
        void _Reallocate_insert_back(const size_type _Count, const value_type _Ch);
        void _Reallocate_insert_back(const_pointer _Ptr, const size_type _Count);

        // increases buffer capacity and stores new data at the specified position
        void _Reallocate_insert_at(const size_type _Off, const size_type _Count, const value_type _Ch);
        void _Reallocate_insert_at(const size_type _Off, const_pointer _Ptr, const size_type _Count);

        // increases buffer capacity and replaces the data at the specified position
        void _Reallocate_replace(
            const size_type _Off, const size_type _Count, const size_type _Ch_count, const value_type _Ch);
        void _Reallocate_replace(
            const size_type _Off, const size_type _Count, const_pointer _Ptr, const size_type _Ptr_count);

        static constexpr size_type _Alloc_align = (2 * sizeof(void*)) / sizeof(value_type);
        static constexpr size_type _Alloc_mask  = _Alloc_align - 1;

        static constexpr size_type _Small_buffer_size     = 16 / sizeof(value_type);
        static constexpr size_type _Small_buffer_capacity = _Small_buffer_size - 1;

        struct _Internal_buffer { // stores small or large buffer
            _Internal_buffer() noexcept;
            ~_Internal_buffer() noexcept;

            // checks whether small buffer is used
            bool _Is_small() const noexcept;

            // returns the currently used buffer
            pointer _Get() noexcept;
            const_pointer _Get() const noexcept;

            // switchs from large to small buffer
            void _Switch_to_small() noexcept;

            // swaps small buffer with large one
            void _Swap_small_with_large(_Internal_buffer& _Other) noexcept;

            // deallocates large buffer
            void _Deallocate_if_large() noexcept;

            size_type _Capacity; // number of characters that can be stored without reallocating memory
            size_type _Size; // number of characters currently stored in the string
            union {
                value_type _Small[_Small_buffer_size];
                pointer _Large;
            };
        };

#pragma warning(suppress : 4251) // C4251: _Internal_buffer needs to have dll-interface
        _Internal_buffer _Mybuf;
    };

    using byte_string    = string<byte_t, char_traits<byte_t>>;
    using utf8_string    = string<char, char_traits<char>>;
    using unicode_string = string<wchar_t, char_traits<wchar_t>>;

    template <class _Elem, class _Traits>
    inline bool operator==(const string<_Elem, _Traits>& _Left, const string<_Elem, _Traits>& _Right) {
        return _Left.compare(_Right) == 0;
    }

    template <class _Elem, class _Traits>
    inline bool operator==(const string<_Elem, _Traits>& _Left, const string_view<_Elem, _Traits> _Right) {
        return _Left.compare(_Right) == 0;
    }

    template <class _Elem, class _Traits>
    inline bool operator==(const string_view<_Elem, _Traits> _Left, const string<_Elem, _Traits>& _Right) {
        return _Right.compare(_Left) == 0;
    }

    template <class _Elem, class _Traits>
    inline bool operator==(const string<_Elem, _Traits>& _Left, const _Elem* const _Right) {
        return _Left.compare(_Right) == 0;
    }

    template <class _Elem, class _Traits>
    inline bool operator==(const _Elem* const _Left, const string<_Elem, _Traits>& _Right) {
        return _Right.compare(_Left) == 0;
    }

    template <class _Elem, class _Traits>
    inline bool operator!=(const string<_Elem, _Traits>& _Left, const string<_Elem, _Traits>& _Right) {
        return !(_Left == _Right);
    }

    template <class _Elem, class _Traits>
    inline bool operator!=(const string<_Elem, _Traits>& _Left, const string_view<_Elem, _Traits> _Right) {
        return !(_Left == _Right);
    }

    template <class _Elem, class _Traits>
    inline bool operator!=(const string_view<_Elem, _Traits> _Left, const string<_Elem, _Traits>& _Right) {
        return !(_Left == _Right);
    }

    template <class _Elem, class _Traits>
    inline bool operator!=(const string<_Elem, _Traits>& _Left, const _Elem* const _Right) {
        return !(_Left == _Right);
    }

    template <class _Elem, class _Traits>
    inline bool operator!=(const _Elem* const _Left, const string<_Elem, _Traits>& _Right) {
        return !(_Left == _Right);
    }

    template <class _Elem, class _Traits>
    inline string<_Elem, _Traits> operator+(
        const string<_Elem, _Traits>& _Left, const string<_Elem, _Traits>& _Right) noexcept {
        string<_Elem, _Traits> _Result = _Left;
        _Result                       += _Right;
        return ::std::move(_Result);
    }

    template <class _Elem, class _Traits>
    inline string<_Elem, _Traits> operator+(
        const string<_Elem, _Traits>& _Left, const _Elem* const _Right) noexcept {
        string<_Elem, _Traits> _Result = _Left;
        _Result                       += _Right;
        return ::std::move(_Result);
    }

    template <class _Elem, class _Traits>
    inline string<_Elem, _Traits> operator+(const string<_Elem, _Traits>& _Left, const _Elem _Right) {
        string<_Elem, _Traits> _Result = _Left;
        _Result.push_back(_Right);
        return ::std::move(_Result);
    }

    template <class _Elem, class _Traits>
    inline string<_Elem, _Traits> operator+(
        const string<_Elem, _Traits>& _Left, const string_view<_Elem, _Traits> _Right) {
        string<_Elem, _Traits> _Result = _Left;
        _Result                       += _Right;
        return ::std::move(_Result);
    }

    template <class _Elem, class _Traits>
    inline string<_Elem, _Traits> operator+(const _Elem* const _Left, const string<_Elem, _Traits>& _Right) {
        string<_Elem, _Traits> _Result = _Left;
        _Result                       += _Right;
        return ::std::move(_Result);
    }

    template <class _Elem, class _Traits>
    inline string<_Elem, _Traits> operator+(const _Elem _Left, const string<_Elem, _Traits>& _Right) {
        string<_Elem, _Traits> _Result = _Right;
        _Result.push_back(_Left);
        return ::std::move(_Result);
    }

    template <class _Elem, class _Traits>
    inline string<_Elem, _Traits> operator+(
        const string_view<_Elem, _Traits> _Left, const string<_Elem, _Traits>& _Right) {
        string<_Elem, _Traits> _Result = _Left;
        _Result                       += _Right;
        return ::std::move(_Result);
    }

    template <class _Elem, class _Traits>
    inline string<_Elem, _Traits> operator+(string<_Elem, _Traits>&& _Left, string<_Elem, _Traits>&& _Right) {
        _Left.append(::std::move(_Right));
        return ::std::move(_Left);
    }

    template <class _Elem, class _Traits>
    inline string<_Elem, _Traits> operator+(
        string<_Elem, _Traits>&& _Left, const string<_Elem, _Traits>& _Right) {
        _Left += _Right;
        return ::std::move(_Left);
    }

    template <class _Elem, class _Traits>
    inline string<_Elem, _Traits> operator+(string<_Elem, _Traits>&& _Left, const _Elem* const _Right) {
        _Left += _Right;
        return ::std::move(_Left);
    }

    template <class _Elem, class _Traits>
    inline string<_Elem, _Traits> operator+(string<_Elem, _Traits>&& _Left, const _Elem _Right) {
        _Left.push_back(_Right);
        return ::std::move(_Left);
    }

    template <class _Elem, class _Traits>
    inline string<_Elem, _Traits> operator+(
        string<_Elem, _Traits>&& _Left, const string_view<_Elem, _Traits> _Right) {
        _Left += _Right;
        return ::std::move(_Left);
    }

    template <class _Elem, class _Traits>
    inline string<_Elem, _Traits> operator+(
        const string<_Elem, _Traits>& _Left, string<_Elem, _Traits>&& _Right) {
        _Right += _Left;
        return ::std::move(_Right);
    }

    template <class _Elem, class _Traits>
    inline string<_Elem, _Traits> operator+(const _Elem* const _Left, string<_Elem, _Traits>&& _Right) {
        _Right += _Left;
        return ::std::move(_Right);
    }

    template <class _Elem, class _Traits>
    inline string<_Elem, _Traits> operator+(const _Elem _Left, string<_Elem, _Traits>&& _Right) {
        _Right.push_back(_Left);
        return ::std::move(_Right);
    }

    template <class _Elem, class _Traits>
    inline string<_Elem, _Traits> operator+(
        const string_view<_Elem, _Traits> _Left, string<_Elem, _Traits>&& _Right) {
        _Right += _Left;
        return ::std::move(_Right);
    }
} // namespace mjx

#endif // _MJSTR_STRING_HPP_