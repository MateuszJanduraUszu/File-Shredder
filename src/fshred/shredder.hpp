// shredder.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _FSHRED_SHREDDER_HPP_
#define _FSHRED_SHREDDER_HPP_
#include <fshred/file.hpp>
#include <fshred/utils.hpp>
#include <cstddef>
#include <cstdint>

namespace fshred {
    class _Dod_5220_22_m_e { // DoD 5220.22-M (E) standard
    public:
        _Dod_5220_22_m_e() noexcept;
        ~_Dod_5220_22_m_e() noexcept;

        // runs the first pass of DoD 5220.22-M (E)
        bool _Run_pass_1(byte_t* const _Buf, const size_t _Size) noexcept;
        
        // runs the second pass of DoD 5220.22-M (E)
        bool _Run_pass_2(byte_t* const _Buf, const size_t _Size) noexcept;

        // runs the third pass of DoD 5220.22-M (E)
        bool _Run_pass_3(byte_t* const _Buf, const size_t _Size) noexcept;

        // resets the fixed value
        bool _Reset() noexcept;

    private:
        // generates a new fixed value
        static bool _Get_fixed_value(byte_t& _Val) noexcept;

        byte_t _Myval; // DoD 5220.22-M (E) fixed value
    };

    class _Dod_5220_22_m_ece { // DoD 5220.22-M (ECE) standard
    public:
        _Dod_5220_22_m_ece() noexcept;
        ~_Dod_5220_22_m_ece() noexcept;

        // runs the first pass of DoD 5220.22-M (ECE)
        bool _Run_pass_1(byte_t* const _Buf, const size_t _Size) noexcept;

        // runs the second pass of DoD 5220.22-M (ECE)
        bool _Run_pass_2(byte_t* const _Buf, const size_t _Size) noexcept;

        // runs the third pass of DoD 5220.22-M (ECE)
        bool _Run_pass_3(byte_t* const _Buf, const size_t _Size) noexcept;

        // runs the fourth pass of DoD 5220.22-M (ECE)
        bool _Run_pass_4(byte_t* const _Buf, const size_t _Size) noexcept;

        // runs the fifth pass of DoD 5220.22-M (ECE)
        bool _Run_pass_5(byte_t* const _Buf, const size_t _Size) noexcept;

        // runs the sixth pass of DoD 5220.22-M (ECE)
        bool _Run_pass_6(byte_t* const _Buf, const size_t _Size) noexcept;

        // runs the seventh pass of DoD 5220.22-M (ECE)
        bool _Run_pass_7(byte_t* const _Buf, const size_t _Size) noexcept;

        // runs the specified pass (1-7)
        bool _Run_pass(byte_t* const _Buf, const size_t _Size, const uint8_t _Which) noexcept;

    private:
        _Dod_5220_22_m_e _Myeng; // runs 1-3 and 5-7 passes
    };
    
    class _File_shredder {
    public:
        explicit _File_shredder(file& _File) noexcept;
        ~_File_shredder() noexcept;

        // tries to securely shred the file
        bool _Shred() noexcept;

    private:
        // runs the specified pass through all data
        bool _Run_pass(const uint8_t _Which) noexcept;

        file& _Myfile;
        _Dod_5220_22_m_ece _Myeng;
    };

    bool securely_shred_file(file& _File) noexcept;
} // namespace fshred

#endif // _FSHRED_SHREDDER_HPP_