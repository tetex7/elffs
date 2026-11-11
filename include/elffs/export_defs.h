/*
 * Copyright (c) 2026. Tetex7
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef ELFFS_EXPORT_DEFS_HPP
#define ELFFS_EXPORT_DEFS_HPP

#if defined(_WIN32) || defined(__CYGWIN__)
#   ifdef ELFFS_EXPORTS
#       define ELFFS_API __declspec(dllexport)
#       define ELFFS_INTERNAL
#   else
#       define ELFFS_API __declspec(dllimport)
#       define ELFFS_INTERNAL
#   endif
#   define ELFFS_RT_API
#elif __GNUC__ >= 4 || defined(__clang__)
#   define ELFFS_API __attribute__((visibility("default")))
#   define ELFFS_INTERNAL __attribute__((visibility("hidden")))
#   define ELFFS_RT_API __attribute__((visibility("hidden")))
#else
#   define ELFFS_API
#   define ELFFS_INTERNAL
#   define ELFFS_RT_API
#endif

#ifdef	__cplusplus
#   define ELFFS_BEGIN_CPP_COMPAT extern "C" {
#   define ELFFS_END_CPP_COMPAT }
#else
#   define ELFFS_BEGIN_CPP_COMPAT
#   define ELFFS_END_CPP_COMPAT
#endif


#if defined(_WIN32) || defined(__CYGWIN__)
#   define ELFFS_NO_IMPLICIT_PADDING __declspec(align(1))
#else
#   define ELFFS_NO_IMPLICIT_PADDING __attribute__((packed))
#endif

#if defined(__clang__)
#   define ELFFS_ON_CLANG 1
#   define ELFFS_OVERLOAD __attribute__((overloadable)) __attribute__((always_inline)) static inline
#endif

#ifdef	__cplusplus
#   define ELFFS_STATIC_ASSERT(expr, diagnostic) static_assert(expr, diagnostic)
#else
#   define ELFFS_STATIC_ASSERT(expr, diagnostic) \
    extern int (*__ELFFS_STATIC_ASSERT_function (void)) \
      [!!sizeof (struct { int __error_if_negative: (expr) ? 2 : -1; })]
#endif

#if defined(__unix__) || defined(__unix) || defined(__APPLE__)
#   define ELFFS_ON_POSIX 1
#endif


#endif //LIBELFFS_EXPORT_DEFS_HPP
