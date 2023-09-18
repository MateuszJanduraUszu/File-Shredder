// directory.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _MJFS_DIRECTORY_HPP_
#define _MJFS_DIRECTORY_HPP_
#include <memory>
#include <mjfs/api.hpp>
#include <mjfs/file.hpp>
#include <mjfs/path.hpp>

namespace mjfs {
    namespace details {
        class _Dir_iter;
    } // namespace details

    class _MJFS_API directory_entry { // represents a directory entry
    public:
        directory_entry() noexcept                  = default;
        directory_entry(const directory_entry&)     = default;
        directory_entry(directory_entry&&) noexcept = default;
        ~directory_entry() noexcept                 = default;

        explicit directory_entry(const path& _Target);

        directory_entry& operator=(const directory_entry&)     = default;
        directory_entry& operator=(directory_entry&&) noexcept = default;

        // assigns new content to the directory entry
        void assign(const path& _New_target);

        // changes the filename of the directory entry
        void replace_filename(const path& _Replacement);

        // updates the cached file attributes
        void refresh() noexcept;

        // returns the path the entry refers to
        const path& absolute_path() const noexcept;

        // checks whether the directory entry refers to existing file system object
        bool exists() const noexcept;

        // checks whether the directory entry refers to a directory
        bool is_directory() const noexcept;

        // checks whether the directory entry refers to a regular file
        bool is_regular_file() const noexcept;

        // checks whether the directory entry refers to a symlink
        bool is_symlink() const noexcept;

        // checks whether the directory entry refers to a junction
        bool is_junction() const noexcept;

    private:
        friend details::_Dir_iter;

        file_attribute _Myattr = file_attribute::none;
        path _Mypath;
    };

    class _MJFS_API directory_iterator { // an iterator to the contents of the directory
    public:
        using value_type        = directory_entry;
        using difference_type   = ptrdiff_t;
        using pointer           = const directory_entry*;
        using reference         = const directory_entry&;
        using iterator_category = ::std::input_iterator_tag;

        directory_iterator() noexcept;
        ~directory_iterator() noexcept;

        directory_iterator(const directory_iterator&) noexcept = default;
        directory_iterator(directory_iterator&&) noexcept = default;

        explicit directory_iterator(const path& _Target);

        directory_iterator& operator=(const directory_iterator&) noexcept = default;
        directory_iterator& operator=(directory_iterator&&) noexcept = default;

        // checks whether two iterators are equal
        bool operator==(const directory_iterator& _Other) const noexcept;

        // checks whether two iterators are not equal
        bool operator!=(const directory_iterator& _Other) const noexcept;

        // returns a reference to the current directory entry
        reference operator*() const;

        // returns a pointer to the current directory entry
        pointer operator->() const;

        // advances the iterator to the next entry
        directory_iterator& operator++();

    private:
#pragma warning(suppress : 4251) // C4251: _Dir_iter needs to have dll-interface
        ::std::shared_ptr<details::_Dir_iter> _Myimpl;
    };

    _MJFS_API directory_iterator begin(directory_iterator _Iter) noexcept;
    _MJFS_API directory_iterator end(directory_iterator _Iter) noexcept;

    _MJFS_API bool create_directory(const path& _Path);
    _MJFS_API bool remove_directory(const path& _Target);
} // namespace mjfs

#endif // _MJFS_DIRECTORY_HPP_