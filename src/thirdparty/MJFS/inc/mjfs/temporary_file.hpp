// temporary_file.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _MJFS_TEMPORARY_FILE_HPP_
#define _MJFS_TEMPORARY_FILE_HPP_
#include <mjfs/api.hpp>
#include <mjfs/file.hpp>

namespace mjx {
    class _MJFS_API temporary_file : public file { // self-delete file representation
    public:
        using native_handle_type = file::native_handle_type;

        temporary_file() noexcept;
        temporary_file(temporary_file&& _Other) noexcept;
        ~temporary_file() noexcept;

        temporary_file& operator=(temporary_file&& _Other) noexcept;
    
        temporary_file(const temporary_file&)            = delete;
        temporary_file& operator=(const temporary_file&) = delete;

        // checks whether the file is temporary
        bool is_temporary() const noexcept;

        // converts the file to regular
        bool make_regular() noexcept;
    };

    _MJFS_API bool create_temporary_file(
        const path& _Path, const file_share _Share, const file_attribute _Attributes,
            const file_flag _Flags, const file_perms _Perms, temporary_file& _File);
    _MJFS_API bool create_temporary_file(const path& _Path, temporary_file& _File);
} // namespace mjx

#endif // _MJFS_TEMPORARY_FILE_HPP_