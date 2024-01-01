// synchronized_allocator.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _MJMEM_SYNCHRONIZED_ALLOCATOR_HPP_
#define _MJMEM_SYNCHRONIZED_ALLOCATOR_HPP_
#include <mjmem/allocator.hpp>
#include <mjmem/api.hpp>

namespace mjx {
    class _MJMEM_API synchronized_allocator : public allocator { // thread-safe memory allocator
    public:
        using value_type      = allocator::value_type;
        using size_type       = allocator::size_type;
        using difference_type = allocator::difference_type;
        using pointer         = allocator::pointer;

        explicit synchronized_allocator(allocator& _Al) noexcept;
        ~synchronized_allocator() noexcept override;

        synchronized_allocator()                                         = delete;
        synchronized_allocator(const synchronized_allocator&)            = delete;
        synchronized_allocator& operator=(const synchronized_allocator&) = delete;

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

    private:
        struct _Srw_lock {
            void* _Opaque;
        };
        
        allocator& _Mywrapped; // wrapped allocator
        mutable _Srw_lock _Mylock;
    };
} // namespace mjx

#endif // _MJMEM_SYNCHRONIZED_ALLOCATOR_HPP_