// directory.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _MJFS_DIRECTORY_HPP_
#define _MJFS_DIRECTORY_HPP_
#include <mjfs/api.hpp>
#include <mjfs/file.hpp>
#include <mjfs/path.hpp>
#include <mjmem/smart_pointer.hpp>

namespace mjx {
    namespace mjfs_impl {
        class _Dir_iter_base;
        class _Dir_iter;
        class _Recursive_dir_iter;
    } // namespace mjfs_impl

    enum class directory_options : unsigned char {
        none                     = 0x00,
        follow_directory_symlink = 0x10,
        skip_permission_denied   = 0x20
    };

    _DECLARE_BIT_OPS(directory_options);

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
        friend mjfs_impl::_Dir_iter_base;
        
        file_attribute _Myattr = file_attribute::none;
        path _Mypath;
    };

    class _Any_dir_iter { // base class for any kind of directory iterator
    public:
        using _Normal_t    = mjfs_impl::_Dir_iter;
        using _Recursive_t = mjfs_impl::_Recursive_dir_iter;

        virtual ~_Any_dir_iter() noexcept;

        virtual _Normal_t* _Normal() noexcept                   = 0;
        virtual const _Normal_t* _Normal() const noexcept       = 0;
        virtual _Recursive_t* _Recursive() noexcept             = 0;
        virtual const _Recursive_t* _Recursive() const noexcept = 0;
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

        directory_iterator(const directory_iterator&)     = default;
        directory_iterator(directory_iterator&&) noexcept = default;

        explicit directory_iterator(const path& _Target);
        directory_iterator(const path& _Target, const directory_options _Options);

        directory_iterator& operator=(const directory_iterator&) noexcept = default;
        directory_iterator& operator=(directory_iterator&&) noexcept      = default;

        // checks whether two iterators are equal
        bool operator==(const directory_iterator& _Other) const noexcept;

        // checks whether two iterators are not equal
        bool operator!=(const directory_iterator& _Other) const noexcept;

        // returns a reference to the current directory entry
        reference operator*() const noexcept;

        // returns a pointer to the current directory entry
        pointer operator->() const noexcept;

        // advances the iterator to the next entry
        directory_iterator& operator++();

    private:
#pragma warning(suppress : 4251) // C4251: _Any_dir_iter needs to have dll-interface
        smart_ptr<_Any_dir_iter> _Myimpl;
    };

    _MJFS_API directory_iterator begin(directory_iterator _Iter) noexcept;
    _MJFS_API directory_iterator end(directory_iterator _Iter) noexcept;

    class _MJFS_API recursive_directory_iterator {
    public:
        using value_type        = directory_entry;
        using difference_type   = ptrdiff_t;
        using pointer           = const directory_entry*;
        using reference         = const directory_entry&;
        using iterator_category = ::std::input_iterator_tag;

        recursive_directory_iterator() noexcept;
        ~recursive_directory_iterator() noexcept;

        recursive_directory_iterator(const recursive_directory_iterator&)     = default;
        recursive_directory_iterator(recursive_directory_iterator&&) noexcept = default;

        explicit recursive_directory_iterator(const path& _Target);
        recursive_directory_iterator(const path& _Target, const directory_options _Options);

        recursive_directory_iterator& operator=(const recursive_directory_iterator&)     = default;
        recursive_directory_iterator& operator=(recursive_directory_iterator&&) noexcept = default;

        // checks whether two iterators are equal
        bool operator==(const recursive_directory_iterator& _Other) const noexcept;

        // checks whether two iterators are not equal
        bool operator!=(const recursive_directory_iterator& _Other) const noexcept;

        // returns a reference to the current directory entry
        reference operator*() const noexcept;

        // returns a pointer to the current directory entry
        pointer operator->() const noexcept;

        // advances the iterator to the next entry
        recursive_directory_iterator& operator++();

        // returns the current active options that affect the iteration
        directory_options options() const noexcept;

        // returns the current recursion depth
        int depth() const noexcept;

        // checks whether the recursion is disabled for the current directory
        bool recursion_pending() const noexcept;

        // disables recursion to the currently referred subdirectory
        void disable_recursion_pending() noexcept;

        // moves the iterator one level up in the directory hierarchy
        void pop();

    private:
#pragma warning(suppress : 4251) // C4251: _Any_dir_iter needs to have dll-interface
        smart_ptr<_Any_dir_iter> _Myimpl;
    };

    _MJFS_API recursive_directory_iterator begin(recursive_directory_iterator _Iter) noexcept;
    _MJFS_API recursive_directory_iterator end(recursive_directory_iterator _Iter) noexcept;

    _MJFS_API bool create_directory(const path& _Path);
    _MJFS_API bool remove_directory(const path& _Target);
} // namespace mjx

#endif // _MJFS_DIRECTORY_HPP_