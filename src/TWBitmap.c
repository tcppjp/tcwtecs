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

#include <assert.h>
#include "tecsui/bitmap.h"

size_t TWComputeBitmapStride(TWPixelFormat format, size_t width)
{
	switch (format) {
		case TWPixelFormat1bppMonotone:
			return (width + 7) >> 3;
        case TWPixelFormat16bppRGB565:
            return width << 1;
		case TWPixelFormat32bppRGBX:
			return width << 2;
		default:
			assert(0);
			return (size_t) -1;
	}
}
