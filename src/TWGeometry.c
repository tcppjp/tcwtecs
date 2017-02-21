/*
 * Copyright (C) 2017 Tomoaki Kawada
 *
 * This file is part of tcwtecs.
 *
 * tcwtecs is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * tcwtecs is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with tcwtecs.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "tecsui/geometry.h"
#include "tecsui/private.h"

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

bool TWRectContainsPoint(const TWRect *rect, const TWPoint *point)
{
	return point->x >= rect->x && point->y >= rect->y &&
		point->x < rect->x + rect->w && point->y < rect->y + rect->h;
}
