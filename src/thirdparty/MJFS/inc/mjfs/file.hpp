// file.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _MJFS_FILE_HPP_
#define _MJFS_FILE_HPP_
#include <cstdint>
#include <mjfs/api.hpp>
#include <mjfs/bitmask.hpp>
#include <mjfs/path.hpp>

namespace mjx {
    enum class file_access : unsigned long {
        none    = 0,
        read    = 0x8000'0000, // GENERIC_READ
        write   = 0x4000'0000, // GENERIC_WRITE
        execute = 0x2000'0000, // GENERIC_EXECUTE
        all     = 0x1000'0000 // GENERIC_ALL
    };

    _DECLARE_BIT_OPS(file_access)

    enum class file_share : unsigned long {
        none   = 0,
        read   = 1, // FILE_SHARE_READ
        write  = 2, // FILE_SHARE_WRITE
        remove = 4, // FILE_SHARE_DELETE
        all    = read | write | remove
    };

    _DECLARE_BIT_OPS(file_share)
        
    enum class file_attribute : unsigned long {
        none          = 0,
        readonly      = 0x0000'0001, // FILE_ATTRIBUTE_READONLY
        hidden        = 0x0000'0002, // FILE_ATTRIBUTE_HIDDEN
        directory     = 0x0000'0010, // FILE_ATTRIBUTE_DIRECTORY
        normal        = 0x0000'0080, // FILE_ATTRIBUTE_NORMAL
        temporary     = 0x0000'0100, // FILE_ATTRIBUTE_TEMPORARY
        reparse_point = 0x0000'0400, // FILE_ATTRIBUTE_REPARSE_POINT
        unknown       = 0xFFFF'FFFF // INVALID_FILE_ATTRIBUTES
    };

    _DECLARE_BIT_OPS(file_attribute)

    enum class file_flag : unsigned long {
        none             = 0,
        backup_semantics = 0x0200'0000, // FILE_FLAG_BACKUP_SEMANTICS
        delete_on_close  = 0x0400'0000, // FILE_FLAG_DELETE_ON_CLOSE
        overlapped       = 0x4000'0000 // FILE_FLAG_OVERLAPPED
    };

    _DECLARE_BIT_OPS(file_flag)

    enum class file_perms : unsigned char {
        none,
        readonly,
        all,
        unknown
    };

    class _MJFS_API file {
    public:
        using native_handle_type = void*;

        file() noexcept;
        file(file&& _Other) noexcept;
        ~file() noexcept;

        explicit file(const path& _Target, const file_access _Access = file_access::all,
            const file_share _Share = file_share::none, const file_flag _Flags = file_flag::none);

        file& operator=(file&& _Other) noexcept;

        file(const file&)            = delete;
        file& operator=(const file&) = delete;

        // checks if the file is open
        bool is_open() const noexcept;

        // returns the underlying implementation-defined handle
        const native_handle_type native_handle() const noexcept;

        // closes the file
        void close() noexcept;

        // opens a new file
        bool open(const path& _Target, const file_access _Access = file_access::all,
            const file_share _Share = file_share::none, const file_flag _Flags = file_flag::none);

        // assigns a new file handle
        bool set_handle(const native_handle_type _New_handle) noexcept;

        // returns the file size
        uint64_t size() const noexcept;

        // renames the file
        bool rename(const path& _New_path);

        // resizes the file
        bool resize(const uint64_t _New_size) noexcept;

        // returns the file attributes
        file_attribute attributes() const noexcept;

        // sets a new attributes to the file
        bool attributes(const file_attribute _New_attributes) noexcept;

        // returns the file permissions
        file_perms permissions() const noexcept;

        // sets a new permissions to the file
        bool permissions(const file_perms _New_perms) noexcept;

    private:
        static constexpr native_handle_type _Invalid_handle = reinterpret_cast<native_handle_type>(-1);

        native_handle_type _Myhandle;
    };

    _MJFS_API bool create_file(
        const path& _Path, const file_share _Share, const file_attribute _Attributes,
        const file_flag _Flags, const file_perms _Perms, file* const _File = nullptr);
    _MJFS_API bool create_file(const path& _Path, file* const _File = nullptr);

    _MJFS_API bool delete_file(const path& _Target);
    _MJFS_API bool delete_file(file& _File);

    _MJFS_API bool rename(const path& _Old_path, const path& _New_path);
} // namespace mjx

#endif // _MJFS_FILE_HPP_