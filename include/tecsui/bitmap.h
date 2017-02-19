/*
 * Copyright (C) 2016 Tomoaki Kawada
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */
#pragma once

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef uint8_t TWPixelFormat;

/**
 * 1 bit per pixel bitmap (LSB to MSB). Each scanline is byte aligned.
 */
#define TWPixelFormat1bppMonotone 	((TWPixelFormat)1)

/**
 * 32 bits per pixel full-color bitmap. No padding.
 * FIXME: define whether this is XRGB ot XBGR. Be careful for endianness!
 */
#define TWPixelFormat32bppRGBX 		((TWPixelFormat)2)

/**
 * 16 bits per pixel (565) high-color bitmap. No padding.
 */
#define TWPixelFormat16bppRGB565    ((TWPixelFormat)3)

size_t TWComputeBitmapStride(TWPixelFormat format, size_t width);

#ifdef __cplusplus
}
#endif
