/*
 * Copyright (C) 2016 Tomoaki Kawada
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#include "tecsui/geometry.h"
#include "TWPrivate.h"

bool TWRectIntersect(const TWRect *a, const TWRect *b, TWRect *outRect)
{
	int x1 = a->x, y1 = a->y;
	int x2 = a->x + a->w, y2 = a->y + a->h;
	int x3 = b->x, y3 = b->y;
	int x4 = b->x + b->w, y4 = b->y + b->h;
	int x = MAX(x1, x3), y = MAX(y1, y3);
	int w = MIN(x2, x4) - x, h = MIN(y2, y4) - y;
	outRect->x = x; outRect->y = y;
	outRect->w = w; outRect->h = h;
	return w > 0 && h > 0;
}

void TWRectUnion(const TWRect *a, const TWRect *b, TWRect *outRect)
{
	int x1 = a->x, y1 = a->y;
	int x2 = a->x + a->w, y2 = a->y + a->h;
	int x3 = b->x, y3 = b->y;
	int x4 = b->x + b->w, y4 = b->y + b->h;
	int x = MIN(x1, x3), y = MIN(y1, y3);
	int w = MAX(x2, x4) - x, h = MAX(y2, y4) - y;
	outRect->x = x; outRect->y = y;
	outRect->w = w; outRect->h = h;
}
