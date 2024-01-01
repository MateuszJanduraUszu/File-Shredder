// api.hpp

// Copyright (c) Mateusz Jandura. All rights reserved.
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef _MJSTR_API_HPP_
#define _MJSTR_API_HPP_

#ifdef MJSTR_EXPORTS
#define _MJSTR_API __declspec(dllexport)
#else // ^^^ MJSTR_EXPORTS ^^^ / vvv !MJSTR_EXPORTS vvv
#define _MJSTR_API __declspec(dllimport)
#endif // MJSTR_EXPORTS
#endif // _MJSTR_API_HPP_