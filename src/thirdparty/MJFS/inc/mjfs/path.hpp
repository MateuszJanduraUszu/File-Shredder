// path.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _MJFS_PATH_HPP_
#define _MJFS_PATH_HPP_
#include <mjfs/api.hpp>
#include <mjstr/string.hpp>
#include <mjstr/string_view.hpp>
#include <type_traits>

namespace mjx {
    template <class _Source>
    inline constexpr bool _Is_valid_path_source = false;

    template <>
    inline constexpr bool _Is_valid_path_source<unicode_string> = true;

    template <>
    inline constexpr bool _Is_valid_path_source<unicode_string_view> = true;

    template <class _Source>
    using _Enable_if_valid_path_source_t = ::std::enable_if_t<_Is_valid_path_source<_Source>, int>;

    class path_iterator;

    class _MJFS_API path { // filesystem path representation
    public:
        using value_type     = wchar_t;
        using string_type    = unicode_string;
        using const_iterator = path_iterator;
        using iterator       = const_iterator;

        static constexpr value_type preferred_separator = L'\\';

        enum format {
            native_format,
            generic_format,
            auto_format
        };

        path() noexcept;
        path(const path& _Other);
        path(path&& _Other) noexcept;
        ~path() noexcept;

        path(const value_type* const _Str, format _Fmt = auto_format);
        path(string_type&& _Str, format _Fmt = auto_format) noexcept;

        template <class _Source, _Enable_if_valid_path_source_t<_Source> = 0>
        path(const _Source& _Src, format _Fmt = auto_format) : _Mystr(_Src.data(), _Src.size()) {
            _Apply_format(_Fmt);
        }

        path& operator=(const path& _Other);
        path& operator=(path&& _Other) noexcept;

        template <class _Source, _Enable_if_valid_path_source_t<_Source> = 0>
        path& operator=(const _Source& _Src) {
            _Mystr.assign(_Src.data(), _Src.size());
            return *this;
        }

        // returns the native version of the path (normal string)
        operator string_type() const;

        // assigns a new path
        path& assign(string_type&& _Str) noexcept;
        
        template <class _Source, _Enable_if_valid_path_source_t<_Source> = 0>
        path& assign(const _Source& _Src) {
            _Mystr.assign(_Src.data(), _Src.size());
            return *this;
        }

        path& operator/=(const path& _Other);

        template <class _Source, _Enable_if_valid_path_source_t<_Source> = 0>
        path& operator/=(const _Source& _Src) {
            return operator/=(path{_Src});
        }

        template <class _Source, _Enable_if_valid_path_source_t<_Source> = 0>
        path& append(const _Source& _Src) {
            return operator/=(path{_Src});
        }

        path& operator+=(const path& _Other);
        path& operator+=(const string_type& _Str);
        path& operator+=(const unicode_string_view _Str);
        path& operator+=(const value_type* const _Str);
        path& operator+=(const value_type _Ch);

        template <class _Source, _Enable_if_valid_path_source_t<_Source> = 0>
        path& operator+=(const _Source& _Src) {
            _Mystr += _Src;
            return *this;
        }

        template <class _Source, _Enable_if_valid_path_source_t<_Source> = 0>
        path& concat(const _Source& _Src) {
            _Mystr += _Src;
            return *this;
        }

        // erases the contents
        void clear() noexcept;

        // converts directory separators to preferred directory separators
        path& make_preferred() noexcept;

        // removes filename path component
        path& remove_filename();

        // replaces the last path component with another path
        path& replace_filename(const path& _Replacement);

        // replaces the extension
        path& replace_extension(const path& _Replacement);

        // swaps two paths
        void swap(path& _Other) noexcept;

        // returns the native version of the path (C-string)
        const value_type* c_str() const noexcept;

        // returns the native version of the path (standard string)
        const string_type& native() const noexcept;

        // checks if the path is empty
        bool empty() const noexcept;

        // returns the root-name of the path, if present
        path root_name() const noexcept;

        // returns the root directory of the path, if present
        path root_directory() const noexcept;

        // returns the root path of the path, if present
        path root_path() const noexcept;

        // returns path relative to the root path
        path relative_path() const noexcept;

        // returns the path of the parent path
        path parent_path() const noexcept;

        // returns the filename path component
        path filename() const noexcept;

        // returns the stem path component (filename without the final extension)
        path stem() const noexcept;

        // returns the file extension path component
        path extension() const noexcept;

        // checks if the path has the root-name
        bool has_root_name() const noexcept;

        // checks if the path has the root directory
        bool has_root_directory() const noexcept;

        // checks if the path has the root path
        bool has_root_path() const noexcept;

        // checks if the path has the relative path
        bool has_relative_path() const noexcept;

        // checks if the path has the parent path
        bool has_parent_path() const noexcept;

        // checks if the path has the filename
        bool has_filename() const noexcept;

        // checks if the path has the stem (filename without the final extension)
        bool has_stem() const noexcept;

        // checks if the path has the extension
        bool has_extension() const noexcept;

        // checks if the path is absolute
        bool is_absolute() const noexcept;

        // checks if the path is relative
        bool is_relative() const noexcept;

        // returns an iterator to the beginning of the path
        iterator begin() const;

        // returns an iterator to the end of the path
        iterator end() const;

    private:
        // applies the specified format
        void _Apply_format(const format _Fmt) noexcept;

        // replaces the specifed slashes with a replacement
        void _Replace_slashes_with(const wchar_t _Slash, const wchar_t _Replacement) noexcept;

        string_type _Mystr;
    };

    _MJFS_API bool operator==(const path& _Left, const path& _Right);
    _MJFS_API bool operator!=(const path& _Left, const path& _Right);
    _MJFS_API path operator/(const path& _Left, const path& _Right);

    class _MJFS_API path_iterator { // input iterator for path
    public:
        using value_type        = path;
        using difference_type   = ptrdiff_t;
        using pointer           = const path*;
        using reference         = const path&;
        using iterator_category = ::std::input_iterator_tag;

        path_iterator() noexcept;
        path_iterator(const path_iterator& _Other);
        path_iterator(path_iterator&& _Other) noexcept;
        ~path_iterator() noexcept;

        explicit path_iterator(const path* const _Path) noexcept;
        explicit path_iterator(const path* const _Path, path&& _Element, const size_t _Off = 0) noexcept;

        path_iterator& operator=(const path_iterator& _Other);
        path_iterator& operator=(path_iterator&& _Other) noexcept;

        // advances the iterator to the next element
        path_iterator& operator++();

        // advances the iterator to the next element (performs post-incrementation)
        path_iterator operator++(int);
        
        // returns a reference to the current path element
        reference operator*() const noexcept;

        // returns a pointer to the current path element
        pointer operator->() const noexcept;

    private:
        friend _MJFS_API bool operator==(const path_iterator&, const path_iterator&);

        // checks whether remaining path contains more valid elements
        bool _Has_more_elements() const noexcept;

        const path* _Mypath; // pointer to the full path
        path _Myelem; // current path element
        size_t _Myoff; // current path element's offset
    };

    _MJFS_API bool operator==(const path_iterator& _Left, const path_iterator& _Right);
    _MJFS_API bool operator!=(const path_iterator& _Left, const path_iterator& _Right);

    _MJFS_API path current_path();
    _MJFS_API bool current_path(const path& _New_path);
} // namespace mjx

#endif // _MJFS_PATH_HPP_