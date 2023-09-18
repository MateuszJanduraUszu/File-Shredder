// api.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _MJFS_API_HPP_
#define _MJFS_API_HPP_

#ifdef MJFS_EXPORTS
#define _MJFS_API __declspec(dllexport)
#else // ^^^ MJFS_EXPORTS ^^^ / vvv !MJFS_EXPORTS vvv
#define _MJFS_API __declspec(dllimport)
#endif // MJFS_EXPORTS
#endif // _MJFS_API_HPP_