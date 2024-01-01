// block_allocator.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _MJMEM_BITMAP_ALLOCATOR_HPP_
#define _MJMEM_BITMAP_ALLOCATOR_HPP_
#include <climits>
#include <mjmem/allocator.hpp>
#include <mjmem/api.hpp>
#include <mjmem/pool_resource.hpp>

namespace mjx {
    class _MJMEM_API block_allocator : public allocator { // fixed-size block allocator
    public:
        using value_type      = allocator::value_type;
        using size_type       = allocator::size_type;
        using difference_type = allocator::difference_type;
        using pointer         = allocator::pointer;

        block_allocator(pool_resource& _Resource, const size_type _Block_size);
        ~block_allocator() noexcept override;

        block_allocator()                                  = delete;
        block_allocator(const block_allocator&)            = delete;
        block_allocator& operator=(const block_allocator&) = delete;

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
        // setups the bitmap
        void _Setup_bitmap();

        // returns the least number of blocks that can hold _Bytes bytes
        size_type _Required_block_count(const size_type _Bytes) const noexcept;

        // returns the greatest number of blocks possible to allocate
        size_type _Max_blocks() const noexcept;

        // allocates exactly one block
        pointer _Allocate_block() noexcept;

        // allocates _Count blocks
        pointer _Allocate_n_blocks(const size_type _Count) noexcept;

        struct _Bitmap { // stores array of words
            using _Word_type = unsigned char; // 1-byte element, can store state of 8 blocks
            
            _Bitmap() noexcept;
            ~_Bitmap() noexcept;

            // returns the bitmap as a word array
            _Word_type* _Get_words() noexcept;
            const _Word_type* _Get_words() const noexcept;

            // returns the number of bits in the last word
            size_type _Bits_per_last_word(const size_type _Max) const noexcept;

            // finds the first occurence of _Count free bits
            size_type _Find_zero_bits(const size_type _Count, const size_type _Max) const noexcept;

            // sets _Count bits starting from _Off bit
            void _Set_bits(size_type _Off, size_type _Count) noexcept;

            // zeros _Count bits starting from _Off bit
            void _Zero_bits(size_type _Off, size_type _Count) noexcept;

            static constexpr size_type _Bits_per_word     = sizeof(_Word_type) * CHAR_BIT;
            static constexpr _Word_type _All_bits_set     = (1 << _Bits_per_word) - 1;
            static constexpr size_type _Small_buffer_size = sizeof(void*);

            union {
                _Word_type _Buf[_Small_buffer_size]; // buffer for small bitmaps
                _Word_type* _Ptr; // pointer to heap-allocated buffer for larger bitmaps
            };

            size_type _Size;
            size_type _Free; // number of free bits
        };

        pool_resource& _Myres;
        size_type _Myblock;
#pragma warning(suppress : 4251) // C4251: _Bitmap needs to have dll-interface
        _Bitmap _Mymap;
    };
} // namespace mjx

#endif // _MJMEM_BITMAP_ALLOCATOR_HPP_