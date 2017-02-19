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

typedef struct tagTWColor {
	uint8_t red, green, blue;
} TWColor;

static inline TWColor TWMakeColor(uint8_t red, uint8_t green, uint8_t blue)
{
	TWColor color = {red, green, blue}; return color;
}

#ifdef __cplusplus
}
#endif
