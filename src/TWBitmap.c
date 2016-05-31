/*
 * Copyright (C) 2016 Tomoaki Kawada
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#include <assert.h>
#include "tecsui/bitmap.h"

size_t TWComputeBitmapStride(TWPixelFormat format, size_t width)
{
	switch (format) {
		case TWPixelFormat1bppMonotone:
			return (width + 7) >> 3;
		case TWPixelFormat32bppRGBX:
			return width << 2;
		default:
			assert(0);
			return (size_t) -1;
	}
}
