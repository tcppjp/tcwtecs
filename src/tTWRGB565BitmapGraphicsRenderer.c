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

/* #[<PREAMBLE>]#
 * Don't edit the comments between #[<...>]# and #[</...>]#
 * These comment are used by tecsmerege when merging.
 *
 * attr access macro #_CAAM_#
 * scissor          TWRect           VAR_scissor
 *
 * call port function #_TCPF_#
 * call port: cRenderTargetBitmapSource signature: sTWRenderTargetBitmapSource context:task
 *   void           cRenderTargetBitmapSource_get( void** outData, TWPixelFormat* outPixelFormat, uint16_t* width, uint16_t* height );
 *
 * #[</PREAMBLE>]# */

/* Put prototype declaration and/or variale definition here #_PAC_# */
#include <assert.h>
#include "tTWRGB565BitmapGraphicsRenderer_tecsgen.h"
#include "tecsui/private.h"
#include "tecsui/bitmap.h"
#include "tecsui/geometry.h"

//
// TODO: OUT DATED!
//

#ifndef E_OK
#define	E_OK	0		/* success */
#define	E_ID	(-18)	/* illegal ID */
#endif

static inline uint16_t TWColorToRGB565(TWColor color) {
	uint16_t ret = (color.red + 4) >> 3;
	ret |= ((uint16_t)((color.green + 2) >> 2)) << 5;
	ret |= ((uint16_t)((color.blue + 4) >> 3)) << 11;
	return ret;
}

static inline uint16_t RGBX32ToRGB565(uint32_t rgbx32) {

}

/* entry port function #_TEPF_# */
/* #[<ENTRY_PORT>]# eGraphicsDeviceOutput
 * entry port: eGraphicsDeviceOutput
 * signature:  sTWGraphicsDeviceOutput
 * context:    task
 * #[</ENTRY_PORT>]# */

/* #[<ENTRY_FUNC>]# eGraphicsDeviceOutput_getScreenSize
 * name:         eGraphicsDeviceOutput_getScreenSize
 * global_name:  tTWRGB565BitmapGraphicsRenderer_eGraphicsDeviceOutput_getScreenSize
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eGraphicsDeviceOutput_getScreenSize(CELLIDX idx, TWSize* outSize)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);
	uint16_t w, h;
	cRenderTargetBitmapSource_get(NULL, NULL, &w, &h);
	outSize->w = w;
	outSize->h = h;
}

/* #[<ENTRY_FUNC>]# eGraphicsDeviceOutput_setScissorRect
 * name:         eGraphicsDeviceOutput_setScissorRect
 * global_name:  tTWRGB565BitmapGraphicsRenderer_eGraphicsDeviceOutput_setScissorRect
 * oneway:       true
 * #[</ENTRY_FUNC>]# */
void
eGraphicsDeviceOutput_setScissorRect(CELLIDX idx, const TWRect* rect)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);
	uint16_t w, h;
	cRenderTargetBitmapSource_get(NULL, NULL, &w, &h);
	TWRect bounds = {0, 0, (int16_t)w, (int16_t)h};
	TWRectIntersect(rect, &bounds, &VAR_scissor);
}

/* #[<ENTRY_FUNC>]# eGraphicsDeviceOutput_fillRect
 * name:         eGraphicsDeviceOutput_fillRect
 * global_name:  tTWRGB565BitmapGraphicsRenderer_eGraphicsDeviceOutput_fillRect
 * oneway:       true
 * #[</ENTRY_FUNC>]# */
void
eGraphicsDeviceOutput_fillRect(CELLIDX idx, TWColor color, const TWRect* rect)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);
	uint16_t *pixels;
	uint16_t w, h;
	cRenderTargetBitmapSource_get((void **)&pixels, NULL, &w, &h);

	int x1 = rect->x, y1 = rect->y;
	int x2 = x1 + rect->w, y2 = y1 + rect->h;
	x1 = MAX(x1, VAR_scissor.x);
	y1 = MAX(y1, VAR_scissor.y);
	x2 = MIN(x2, VAR_scissor.x + VAR_scissor.w);
	y2 = MIN(y2, VAR_scissor.y + VAR_scissor.h);
	if (x2 <= x1 || y2 <= y1) {
		return;
	}

	pixels += x1 + y1 * w;

	int rw = x2 - x1, rh = y2 - y1;
	uint32_t pixel = TWColorToRGB565(color);

	for (int y = 0; y < rh; ++y) {
		for (int x = 0; x < rw; ++x) {
			*(pixels++) = pixel;
		}
		pixels += w - rw;
	}
}

/* #[<ENTRY_FUNC>]# eGraphicsDeviceOutput_drawBitmap
 * name:         eGraphicsDeviceOutput_drawBitmap
 * global_name:  tTWRGB565BitmapGraphicsRenderer_eGraphicsDeviceOutput_drawBitmap
 * oneway:       true
 * #[</ENTRY_FUNC>]# */
void
eGraphicsDeviceOutput_drawBitmap(CELLIDX idx, const char* data, TWPixelFormat format, const TWSize* bitmapSize, uint32_t numBytes, const TWRect* inRect, const TWPoint* outLoc, TWColor monoColor)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);

	uint16_t *pixels;
	uint16_t tgt_w, tgt_h;
	cRenderTargetBitmapSource_get((void **)&pixels, NULL, &tgt_w, &tgt_h);

	int src_w = bitmapSize->w, src_h = bitmapSize->h;

	if (src_w <= 0 || src_h <= 0) {
		assert(0); // TODO: error report
		return;
	}

	// size check
	size_t stride = TWComputeBitmapStride(format, src_w);
	size_t real_size = stride * src_h; // TODO: overflow check
	if (numBytes < real_size) {
		assert(0); // TODO: error report
		return;
	}

	// perform clipping
	int in_x1 = inRect->x, in_y1 = inRect->y;
	int in_x2 = in_x1 + inRect->w, in_y2 = in_y1 + inRect->h;
	int out_x1 = outLoc->x, out_y1 = outLoc->y;
	int out_x2 = out_x1 + inRect->w, out_y2 = out_y1 + inRect->h;
	int sci_x1 = VAR_scissor.x, sci_y1 = VAR_scissor.y;
	int sci_x2 = sci_x1 + VAR_scissor.w, sci_y2 = sci_y1 + VAR_scissor.h;
	if (in_x1 < 0) {
		out_x1 -= in_x1; in_x1 = 0;
	}
	if (in_y1 < 0) {
		out_y1 -= in_y1; in_y1 = 0;
	}
	if (in_x2 > src_w) {
		out_x2 += src_w - in_x2; in_x2 = src_w;
	}
	if (in_y2 > src_h) {
		out_y2 += src_h - in_y2; in_y2 = src_h;
	}
	if (out_x1 < sci_x1) {
		out_x2 += sci_x1 - out_x1; out_x1 = sci_x1;
	}
	if (out_y1 < sci_y1) {
		out_y2 += sci_y1 - out_y1; out_y1 = sci_y1;
	}
	if (out_x2 > sci_x2) {
		in_x2 += sci_x2 - out_x2; out_x2 = sci_x2;
	}
	if (out_y2 > sci_y2) {
		in_y2 += sci_y2 - out_y2; out_y2 = sci_y2;
	}
	if (out_x2 <= out_x1 || out_y2 <= out_y1) {
		// culled
		return;
	}

	data += in_y1 * stride;
	pixels += out_x1 + out_y1 * tgt_w;

	switch (format) {
		case TWPixelFormat1bppMonotone: {
			uint32_t pixel = TWColorToRGB565(monoColor);
			data += in_x1 >> 3;

			for (int y = out_y1; y < out_y2; ++y) {
				const char *cur_scanline = data;
				unsigned int cur_byte = *(cur_scanline++);
				int cur_bit = in_x1 & 7;
				cur_byte >>= cur_bit;

				for (int x = out_x1; x < out_x2; ++x) {
					if (cur_bit == 8) {
						cur_bit = 0;
						cur_byte = *(cur_scanline++);
					}
					if (cur_byte & 1) {
						*pixels = pixel;
					}
					++cur_bit; cur_byte >>= 1; ++pixels;
				}
				data += stride;
				pixels += tgt_w - (out_x2 - out_x1);
			}
			break;
		}
		case TWPixelFormat32bppRGBX: {
			uint32_t *in_pixels = (uint32_t *)data + in_x1;
			for (int y = out_y1; y < out_y2; ++y) {
				for (int x = out_x1; x < out_x2; ++x) {
					*(pixels++) = RGBX32ToRGB565(*(in_pixels++));
				}
				pixels += tgt_w - (out_x2 - out_x1);
				in_pixels += src_w - (out_x2 - out_x1);
				// note: out_x2 - out_x1 == in_x2 - in_x1
				//       using common subexpressions allows optimization
			}
			break;
		}
		case TWPixelFormat16bppRGB565: {
			uint16_t *in_pixels = (uint16_t *)data + in_x1;
			for (int y = out_y1; y < out_y2; ++y) {
				for (int x = out_x1; x < out_x2; ++x) {
					*(pixels++) = *(in_pixels++);
				}
				pixels += tgt_w - (out_x2 - out_x1);
				in_pixels += src_w - (out_x2 - out_x1);
				// note: out_x2 - out_x1 == in_x2 - in_x1
				//       using common subexpressions allows optimization
			}
			break;
		}
		default:
			// unsupported or invalid
			assert(0);
			break;
	}
}

/* #[<ENTRY_FUNC>]# eGraphicsDeviceOutput_update
 * name:         eGraphicsDeviceOutput_update
 * global_name:  tTWRGB565BitmapGraphicsRenderer_eGraphicsDeviceOutput_update
 * oneway:       true
 * #[</ENTRY_FUNC>]# */
void
eGraphicsDeviceOutput_update(CELLIDX idx, const TWRect* rect)
{
	// no-op; there's nothing we can do
}

/* #[<POSTAMBLE>]#
 *   Put non-entry functions below.
 * #[</POSTAMBLE>]#*/
