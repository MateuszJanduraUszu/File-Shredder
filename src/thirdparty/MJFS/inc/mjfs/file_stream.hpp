// file_stream.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _MJFS_FILE_STREAM_HPP_
#define _MJFS_FILE_STREAM_HPP_
#include <cstddef>
#include <cstdint>
#include <mjfs/api.hpp>
#include <mjfs/file.hpp>
#include <mjstr/string.hpp>
#include <mjstr/string_view.hpp>

namespace mjx {
    enum class move_direction : bool {
        backward,
        forward
    };

    class _MJFS_API file_stream { // file read/write stream
    public:
        using char_type = byte_t;
        using pos_type  = uint64_t;
        using off_type  = uint64_t;
        using int_type  = size_t;

        file_stream() noexcept;
        file_stream(file_stream&& _Other) noexcept;
        ~file_stream() noexcept;

        explicit file_stream(file& _File) noexcept;

        file_stream& operator=(file_stream&& _Other) noexcept;

        file_stream(const file_stream&)            = delete;
        file_stream& operator=(const file_stream&) = delete;

        // checks if the stream is open
        bool is_open() const noexcept;

        // closes the stream
        void close() noexcept;

        // assigns a new file to the stream
        void bind_file(file& _New_file) noexcept;

        // returns the stream position
        pos_type tell() const noexcept;

        // changes the stream position
        bool seek(const pos_type _New_pos) noexcept;

        // changes the stream position to the end
        bool seek_to_end() noexcept;

        // move the stream position
        bool move(const off_type _Off,
            const move_direction _Direction = move_direction::forward) noexcept;

        // reads a byte sequence from the stream
        int_type read(char_type* const _Buf, const int_type _Count) noexcept;
        int_type read(byte_string& _Buf) noexcept;

        // writes a byte sequence to the stream
        bool write(const char_type* const _Data, const int_type _Count) noexcept;
        bool write(const byte_string_view _Data) noexcept;

        // writes the stream data to the file
        bool flush() noexcept;

    private:
        file* _Myfile;
    };
} // namespace mjx

#endif // _MJFS_FILE_STREAM_HPP_