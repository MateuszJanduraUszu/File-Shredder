// api.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _MJMEM_API_HPP_
#define _MJMEM_API_HPP_

#ifdef MJMEM_EXPORTS
#define _MJMEM_API __declspec(dllexport)
#else // ^^^ MJMEM_EXPORTS ^^^ / vvv !MJMEM_EXPORTS vvv
#define _MJMEM_API __declspec(dllimport)
#endif // MJMEM_EXPORTS
#endif // _MJMEM_API_HPP_