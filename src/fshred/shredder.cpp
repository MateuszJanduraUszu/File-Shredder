// shredder.cpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#include <fshred/shredder.hpp>
#include <fshred/random.hpp>
#include <cstring>
#include <utility>

namespace fshred {
    _Dod_5220_22_m_e::_Dod_5220_22_m_e() noexcept : _Myval() {}

    _Dod_5220_22_m_e::~_Dod_5220_22_m_e() noexcept {}

    bool _Dod_5220_22_m_e::_Get_fixed_value(byte_t& _Val) noexcept {
        return ::fshred::fill_with_random_bytes(&_Val, 1);
    }

    bool _Dod_5220_22_m_e::_Run_pass_1(byte_t* const _Buf, const size_t _Size) noexcept {
        // "pass 1: Overwrite the data with a defined fixed value"
        ::memset(_Buf, static_cast<int>(_Myval), _Size);
        return true;
    }

    bool _Dod_5220_22_m_e::_Run_pass_2(byte_t* const _Buf, const size_t _Size) noexcept {
        // "pass 2: Overwrite the data with the complement value of the first run"
        ::memset(_Buf, static_cast<int>(~_Myval), _Size);
        return true;
    }

    bool _Dod_5220_22_m_e::_Run_pass_3(byte_t* const _Buf, const size_t _Size) noexcept {
        // "pass 3: Overwrite the data with pseudo random values"
        return ::fshred::fill_with_random_bytes(_Buf, _Size);
    }

    bool _Dod_5220_22_m_e::_Reset() noexcept {
        return _Get_fixed_value(_Myval); // generates a new fixed value
    }

    _Dod_5220_22_m_ece::_Dod_5220_22_m_ece() noexcept : _Myeng() {}

    _Dod_5220_22_m_ece::~_Dod_5220_22_m_ece() noexcept {}

    bool _Dod_5220_22_m_ece::_Run_pass_1(byte_t* const _Buf, const size_t _Size) noexcept {
        // "pass 1-3: overwrite the data with DoD 5220.22-M (E) Standard"
        return _Myeng._Reset() && _Myeng._Run_pass_1(_Buf, _Size);
    }

    bool _Dod_5220_22_m_ece::_Run_pass_2(byte_t* const _Buf, const size_t _Size) noexcept {
        // "pass 1-3: overwrite the data with DoD 5220.22-M (E) Standard"
        return _Myeng._Run_pass_2(_Buf, _Size);
    }

    bool _Dod_5220_22_m_ece::_Run_pass_3(byte_t* const _Buf, const size_t _Size) noexcept {
        // "pass 1-3: overwrite the data with DoD 5220.22-M (E) Standard"
        return _Myeng._Run_pass_3(_Buf, _Size);
    }

    bool _Dod_5220_22_m_ece::_Run_pass_4(byte_t* const _Buf, const size_t _Size) noexcept {
        // "pass 4: overwrite the data with pseudo random values, the DoD 5220.22-M (C) Standard"
        return ::fshred::fill_with_random_bytes(_Buf, _Size);
    }

    bool _Dod_5220_22_m_ece::_Run_pass_5(byte_t* const _Buf, const size_t _Size) noexcept {
        // "pass 5-7: overwrite the data with DoD 5220.22-M (E) Standard"
        return _Myeng._Reset() && _Myeng._Run_pass_1(_Buf, _Size);
    }

    bool _Dod_5220_22_m_ece::_Run_pass_6(byte_t* const _Buf, const size_t _Size) noexcept {
        // "pass 5-7: overwrite the data with DoD 5220.22-M (E) Standard"
        return _Myeng._Run_pass_2(_Buf, _Size);
    }

    bool _Dod_5220_22_m_ece::_Run_pass_7(byte_t* const _Buf, const size_t _Size) noexcept {
        // "pass 5-7: overwrite the data with DoD 5220.22-M (E) Standard"
        return _Myeng._Run_pass_3(_Buf, _Size);
    }

    bool _Dod_5220_22_m_ece::_Run_pass(
        byte_t* const _Buf, const size_t _Size, const uint8_t _Which) noexcept {
        switch (_Which) {
        case 1:
            return _Run_pass_1(_Buf, _Size);
        case 2:
            return _Run_pass_2(_Buf, _Size);
        case 3:
            return _Run_pass_3(_Buf, _Size);
        case 4:
            return _Run_pass_4(_Buf, _Size);
        case 5:
            return _Run_pass_5(_Buf, _Size);
        case 6:
            return _Run_pass_6(_Buf, _Size);
        case 7:
            return _Run_pass_7(_Buf, _Size);
        default:
            return false;
        }
    }

    _File_shredder::_File_shredder(file& _File) noexcept : _Myfile(_File), _Myeng() {}

    _File_shredder::~_File_shredder() noexcept {}

    bool _File_shredder::_Run_pass(const uint8_t _Which) noexcept {
        uint64_t _Remaining = _Myfile.size();
        if (_Remaining == 0) { // no data to overwrite, do nothing
            return true;
        }

        static constexpr size_t _Buf_size = 4096;
        byte_t _Buf[_Buf_size];
        size_t _Chunk_size;
        _Myfile.seek(0); // start from the begin, should success
        while (_Remaining > 0) {
#ifdef _M_X64
            _Chunk_size = (::std::min)(_Buf_size, _Remaining);
#else // ^^^ _M_X64 ^^^ / vvv _M_IX86 vvv
            _Chunk_size = (::std::min)(_Buf_size, static_cast<size_t>(_Remaining));
#endif // _M_X64
            if (!_Myeng._Run_pass(_Buf, _Chunk_size, _Which)) {
                return false;
            }

            if (!_Myfile.write(byte_string_view{_Buf, _Chunk_size})) {
                return false;
            }

#ifdef _M_X64
            _Remaining -= _Chunk_size;
#else // ^^^ _M_X64 ^^^ / vvv _M_IX86 vvv
            _Remaining -= static_cast<uint64_t>(_Chunk_size);
#endif // _M_X64
        }

        return true;
    }

    bool _File_shredder::_Shred() noexcept {
        for (uint8_t _Which = 1; _Which <= 7; ++_Which) {
            if (!_Run_pass(_Which)) {
                return false;
            }
        }

        return true;
    }

    bool securely_shred_file(file& _File) noexcept {
        _File_shredder _Shredder(_File);
        return _Shredder._Shred() && _File.resize(0);
    }
} // namespace fshred