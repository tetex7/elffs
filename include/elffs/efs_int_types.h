/*
 * Copyright (c) 2026. Tetex7
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

//
// Created by tete on 8/30/26.
//

#ifndef ELFFS_EFS_INT_TYPES_H
#define ELFFS_EFS_INT_TYPES_H
#include <stdbool.h>
#include <stdint.h>

typedef int8_t efs_i8_t;
typedef uint8_t efs_u8_t;

typedef int16_t efs_i16_t;
typedef uint16_t efs_u16_t;

typedef int32_t efs_i32_t;
typedef uint32_t efs_u32_t;

typedef int64_t efs_i64_t;
typedef uint64_t efs_u64_t;

#ifdef EFS_USE_I128
typedef __int128_t efs_i128_t;
typedef __uint128_t efs_u128_t;
#endif

typedef bool efs_bool_t;

enum efs_result_e
{
    EFS_RESULT_SUCCESS = false,
    EFS_RESULT_ERROR = true
};

typedef efs_bool_t efs_result_t;


typedef int8_t efs_sbyte_t;
typedef uint8_t efs_byte_t;

//for efsFile_fgetc
typedef efs_i16_t efs_fbyte_t;

//Yes I know this does not conform to the x86 thing of what a word is
//but this is common integer size for EFS binary
typedef efs_i64_t efs_sword_t;
typedef efs_u64_t efs_word_t;

typedef efs_u32_t efs_size32_t;
typedef efs_i32_t efs_off32_t;
typedef efs_i32_t efs_ssize32_t;

typedef efs_u64_t efs_size_t;
typedef efs_i64_t efs_off_t;
typedef efs_i64_t efs_ssize_t;

typedef efs_u8_t efs_f8_t;
typedef efs_u16_t efs_f16_t;
typedef efs_u32_t efs_f32_t;
typedef efs_u64_t efs_f64_t;

typedef const char* efs_string;

#endif //ELFFS_EFS_INT_TYPES_H
