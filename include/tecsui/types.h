/*
 * Copyright (C) 2016 Tomoaki Kawada
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */
#pragma once

#ifdef TECSGEN
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long uint32_t;
typedef unsigned char uint_fast8_t;
typedef unsigned short uint_fast16_t;
typedef unsigned long uint_fast32_t;
typedef char int8_t;
typedef short int16_t;
typedef long int32_t;
typedef char int_fast8_t;
typedef short int_fast16_t;
typedef long int_fast32_t;
typedef uint8_t bool;
typedef uint32_t size_t;
typedef size_t intptr_t;
typedef size_t uintptr_t;
#else
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#endif

// FIXME: i don't think this is supposed to be defined here
typedef signed int int_t;

typedef uint8_t TWViewStyle;

#define TWViewStyleVisible			((TWViewStyle)(1 << 0))

typedef int32_t TWDuration;
typedef uint32_t TWTimePoint;

#ifndef E_OK
#define	E_OK	0		/* success */
#define	E_ID	(-18)	/* illegal ID */
#endif
