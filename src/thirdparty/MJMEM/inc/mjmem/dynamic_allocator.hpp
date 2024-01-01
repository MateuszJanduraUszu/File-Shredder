// dynamic_allocator.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _MJMEM_DYNAMIC_ALLOCATOR_HPP_
#define _MJMEM_DYNAMIC_ALLOCATOR_HPP_
#include <mjmem/allocator.hpp>
#include <mjmem/api.hpp>

namespace mjx {
    class _MJMEM_API dynamic_allocator : public allocator { // stateless memory allocator
    public:
        using value_type      = allocator::value_type;
        using size_type       = allocator::size_type;
        using difference_type = allocator::difference_type;
        using pointer         = allocator::pointer;

        dynamic_allocator() noexcept;
        dynamic_allocator(const dynamic_allocator& _Other) noexcept;
        dynamic_allocator(dynamic_allocator&& _Other) noexcept;
        ~dynamic_allocator() noexcept override;

        dynamic_allocator& operator=(const dynamic_allocator& _Other) noexcept;
        dynamic_allocator& operator=(dynamic_allocator&& _Other) noexcept;

        // allocates uninitialized storage
        pointer allocate(const size_type _Count) override;

        // allocates uninitialized storage with the specifed alignment
        pointer allocate_aligned(const size_type _Count, const size_type _Align) override;

        // deallocates storage
        void deallocate(pointer _Ptr, const size_type _Count) noexcept override;

        // returns the largest supported allocation size
        size_type max_size() const noexcept override;

        // compares for equality with another allocator
        bool is_equal(const allocator& _Other) const noexcept override;
    };
} // namespace mjx

#endif // _MJMEM_DYNAMIC_ALLOCATOR_HPP_