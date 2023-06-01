// file.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _FSHRED_FILE_HPP_
#define _FSHRED_FILE_HPP_
#include <fshred/utils.hpp>
#include <cstdint>
#include <cstddef>
#include <filesystem>

namespace fshred {
    using path = ::std::filesystem::path;

    class file {
    public:
        using native_handle_type = void*;

        explicit file(const path& _Target);
        ~file() noexcept;

        // checks if any file is open
        bool is_open() const noexcept;

        // returns a pointer to the native implementation
        native_handle_type native_handle() const noexcept;

        // closes the file
        void close() noexcept;

        // tries to read _Count bytes from the file into the buffer
        size_t read(byte_t* const _Buf, const size_t _Count) noexcept;

        // tries to write _Bytes to the file
        bool write(const byte_string_view _Bytes) noexcept;

        // returns the file size
        uint64_t size() const noexcept;

        // tries to resize the file
        bool resize(const uint64_t _New_size) noexcept;

    private:
        // tries to open a file
        [[nodiscard]] static void* _Open(const path& _Target);

        void* _Myhandle;
    };
} // namespace fshred

#endif // _FSHRED_FILE_HPP_