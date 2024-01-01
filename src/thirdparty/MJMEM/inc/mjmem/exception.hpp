// exception.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _MJMEM_EXCEPTION_HPP_
#define _MJMEM_EXCEPTION_HPP_
#include <mjmem/api.hpp>

namespace mjx {
    class _MJMEM_API allocation_failure {
    public:
        allocation_failure() noexcept;
        ~allocation_failure() noexcept;

        // raises allocation failure exception
        __declspec(noreturn) static void raise();
    };

    class _MJMEM_API allocation_limit_exceeded {
    public:
        allocation_limit_exceeded() noexcept;
        ~allocation_limit_exceeded() noexcept;

        // raises allocation limit exceeded exception
        __declspec(noreturn) static void raise();
    };

    class _MJMEM_API resource_overrun {
    public:
        resource_overrun() noexcept;
        ~resource_overrun() noexcept;

        // raises resource overrun exception
        __declspec(noreturn) static void raise();
    };
} // namespace mjx

#endif // _MJMEM_EXCEPTION_HPP_