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

typedef struct tagTWPoint {
	int16_t x, y;
} TWPoint;

typedef struct tagTWSize {
	int16_t w, h;
} TWSize;

typedef struct tagTWRect {
	int16_t x, y, w, h;
} TWRect;

static inline TWPoint TWMakePoint(int16_t x, int16_t y)
{
	TWPoint p = {x, y}; return p;
}

static inline TWSize TWMakeSize(int16_t w, int16_t h)
{
	TWSize p = {w, h}; return p;
}

static inline TWRect TWMakeRect(int16_t x, int16_t y, int16_t w, int16_t h)
{
	TWRect r = {x, y, w, h}; return r;
}

static inline void TWSetRect(TWRect *r, int16_t x, int16_t y, int16_t w, int16_t h)
{
	r->x = x; r->y = y; r->w = w; r->h = h;
}

/**
 * Computes the rectangle representing the overlapping region of two
 * rectangles.
 * @return false if the region is empty.
 */
bool TWRectIntersect(const TWRect *a, const TWRect *b, TWRect *outRect);

/**
 * Computes the smallest rectangle that contains both of two rectangles.
 * Zero-area rectangles are considered as lines or points and taken into account
 * while the computation.
 */
void TWRectUnion(const TWRect *a, const TWRect *b, TWRect *outRect);

#ifdef __cplusplus
}
#endif
