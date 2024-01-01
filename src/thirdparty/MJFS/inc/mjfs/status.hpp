// status.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _MJFS_STATUS_HPP_
#define _MJFS_STATUS_HPP_
#include <cstdint>
#include <mjfs/api.hpp>
#include <mjfs/path.hpp>

namespace mjx {
    _MJFS_API bool exists(const path& _Target);
    _MJFS_API bool is_directory(const path& _Target);
    _MJFS_API bool is_regular_file(const path& _Target);
    _MJFS_API bool is_symlink(const path& _Target);
    _MJFS_API bool is_junction(const path& _Target);

    struct space_info {
        uintmax_t capacity;
        uintmax_t free;
        uintmax_t available;
    };
    
    _MJFS_API space_info space(const path& _Target);

    _MJFS_API bool is_hidden(const path& _Target);
    _MJFS_API bool make_hidden(const path& _Target);
    _MJFS_API bool make_visible(const path& _Target);

    _MJFS_API bool is_readonly(const path& _Target);
    _MJFS_API bool make_readonly(const path& _Target);
    _MJFS_API bool make_writable(const path& _Target);
} // namespace mjx

#endif // _MJFS_STATUS_HPP_