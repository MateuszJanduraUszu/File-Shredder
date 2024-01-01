// pool_resource.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _MJMEM_POOL_RESOURCE_HPP_
#define _MJMEM_POOL_RESOURCE_HPP_
#include <mjmem/api.hpp>
#include <mjmem/dynamic_allocator.hpp>

namespace mjx {
    class _MJMEM_API pool_resource { // memory block that serves as a pool resource
    public:
        using value_type      = allocator::value_type;
        using size_type       = allocator::size_type;
        using difference_type = allocator::difference_type;
        using pointer         = allocator::pointer;
        using const_pointer   = const pointer;

        pool_resource() noexcept;
        pool_resource(const pool_resource& _Other);
        pool_resource(pool_resource&& _Other) noexcept;
        ~pool_resource() noexcept;

        explicit pool_resource(const size_type _Size);

        pool_resource& operator=(const pool_resource& _Other);
        pool_resource& operator=(pool_resource&& _Other) noexcept;

        // checks whether the resource is empty
        bool empty() const noexcept;

        // returns the resource data
        pointer data() noexcept;
        const_pointer data() const noexcept;
        
        // returns the resource size
        size_type size() const noexcept;

        // checks whether the specified memory block comes from this resource
        bool contains(const_pointer _Block, const size_type _Size) const noexcept;

        // swaps two resources
        void swap(pool_resource& _Other) noexcept;

        // releases the underlying resource data
        pointer release() noexcept;

        // deallocates the underlying resource data
        void destroy() noexcept;

    private:
        // copies other resource
        void _Copy_resource(const_pointer _Data, const size_type _Size);
        
        // steals other resource
        void _Steal_resource(pointer& _Data, size_type& _Size) noexcept;

        pointer _Mydata;
        size_type _Mysize;
    };

    _MJMEM_API bool operator==(const pool_resource& _Left, const pool_resource& _Right);
    _MJMEM_API bool operator!=(const pool_resource& _Left, const pool_resource& _Right);
} // namespace mjx

#endif // _MJMEM_POOL_RESOURCE_HPP_