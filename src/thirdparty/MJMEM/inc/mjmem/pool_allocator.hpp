// pool_allocator.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _MJMEM_POOL_ALLOCATOR_HPP_
#define _MJMEM_POOL_ALLOCATOR_HPP_
#include <mjmem/allocator.hpp>
#include <mjmem/api.hpp>
#include <mjmem/pool_resource.hpp>

namespace mjx {
    class _MJMEM_API pool_allocator : public allocator { // variable-size block allocator
    public:
        using value_type      = allocator::value_type;
        using size_type       = allocator::size_type;
        using difference_type = allocator::difference_type;
        using pointer         = allocator::pointer;

        // constant indicating no allocation limit
        static constexpr size_type unbounded_block_size = static_cast<size_type>(-1);

        pool_allocator(pool_resource& _Resource, const size_type _Max_block_size = unbounded_block_size);
        ~pool_allocator() noexcept override;

        pool_allocator()                                 = delete;
        pool_allocator(const pool_allocator&)            = delete;
        pool_allocator& operator=(const pool_allocator&) = delete;

        // allocates uninitialized storage
        pointer allocate(const size_type _Count) override;

        // allocates uninitialized storage with the specifed alignment
        pointer allocate_aligned(const size_type _Count, const size_type _Align) override;

        // deallocates storage
        void deallocate(pointer _Ptr, const size_type _Count) noexcept override;

        // returns the largest supported allocation size
        size_type max_size() const noexcept override;

        // compares for equality with another allocator
        bool is_equal(const allocator& _Al) const noexcept override;

        // returns the number of free blocks
        size_type free_blocks() const noexcept;

    private:
        // allocates uninitialized storage
        pointer _Allocate(const size_type _Count);

        struct _List_node {
            _List_node* _Next = nullptr;
            _List_node* _Prev = nullptr;
            size_type _Off    = 0;
            size_type _Size   = 0;

            ~_List_node() noexcept = default;
        };

        class _List { // sorted list of free blocks
        public:        
            _List() noexcept;
            ~_List() noexcept;

            // returns the list size
            size_type _Size() const noexcept;

            // reserves a new block
            size_type _Reserve_block(const size_type _Size) noexcept;

            // releases the reserved block
            void _Release_block(const size_type _Off, const size_type _Size);

        private:
            // creates a new node
            static _List_node* _Create_node();

            // deallocates the specified node
            static void _Deallocate_node(_List_node* const _Node) noexcept;

            // breaks connection between nodes connected with _Node
            void _Unlink_node(_List_node* const _Node) noexcept;

            // unlinks and deallocates an existing node
            void _Delete_node(_List_node* const _Node) noexcept;

            // inserts a new node
            void _Insert_node(_List_node* const _New_node) noexcept;

            // inserts an existing node again to keep list sorted
            void _Reinsert_node(_List_node* const _Node) noexcept;

            // tries to merge the specified block to the existing one
            bool _Merge_block(const size_type _Off, const size_type _Size) noexcept;

            _List_node* _Myhead; // pointer to the first node
            _List_node* _Mytail; // pointer to the last node
            size_type _Mysize; // total number of nodes
        };

        pool_resource& _Myres;
        size_type _Mymax; // maximum number of bytes possible to allocate
#pragma warning(suppress : 4251) // C4251: _List needs to have dll-interface
        _List _Mylist;
    };
} // namespace mjx

#endif // _MJMEM_POOL_ALLOCATOR_HPP_