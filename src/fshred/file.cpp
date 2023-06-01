// file.cpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#include <fshred/file.hpp>
#include <Windows.h>

namespace fshred {
    file::file(const path& _Target) : _Myhandle(_Open(_Target)) {}

    file::~file() noexcept {
        close();
    }

    [[nodiscard]] void* file::_Open(const path& _Target) {
        return ::CreateFileW(_Target.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ
            | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    }

    bool file::is_open() const noexcept {
        return _Myhandle != nullptr;
    }

    file::native_handle_type file::native_handle() const noexcept {
        return _Myhandle;
    }

    void file::close() noexcept {
        if (_Myhandle) {
            ::CloseHandle(_Myhandle);
            _Myhandle = nullptr;
        }
    }

    size_t file::read(byte_t* const _Buf, const size_t _Count) noexcept {
        if (!_Myhandle) { // no file is open
            return 0;
        }

        if (_Count == 0) { // nothing to read, do nothing
            return 0;
        }

        if (!_Buf) { // invalid buffer
            return 0;
        }

        unsigned long _Read = 0;
        return ::ReadFile(_Myhandle, _Buf, static_cast<unsigned long>(_Count), &_Read, nullptr) != 0
            ? static_cast<size_t>(_Read) : 0;
    }

    bool file::write(const byte_string_view _Bytes) noexcept {
        if (!_Myhandle) { // no file is open
            return false;
        }

        if (_Bytes.empty()) { // nothing to write, do nothing
            return true;
        }

        const unsigned long _Size = static_cast<unsigned long>(_Bytes.size());
        unsigned long _Written    = 0;
        if (::WriteFile(_Myhandle, _Bytes.data(), _Size, &_Written, nullptr) == 0) {
            return false;
        }

        return _Written == _Size;
    }

    uint64_t file::size() const noexcept {
        if (!_Myhandle) { // no file is open
            return 0;
        }

        unsigned long _High = 0;
        return static_cast<uint64_t>(
            ::GetFileSize(_Myhandle, &_High) | (static_cast<uint64_t>(_High) << 32));
    }

    bool file::resize(const uint64_t _New_size) noexcept {
        if (!_Myhandle) { // no file is open
            return false;
        }

        long _High = static_cast<long>((_New_size & 0xFFFF'FFFF'0000'0000) >> 32);
        return ::SetFilePointer(_Myhandle, static_cast<long>(_New_size), &_High, FILE_BEGIN)
            != INVALID_SET_FILE_POINTER && ::SetEndOfFile(_Myhandle) != 0;
    }
} // namespace fshred