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
 * numScanlineClipperNodes uint16_t         ATTR_numScanlineClipperNodes
 * scissor          TWRect           VAR_scissor
 * scanlineClipper  TWScanlineClipperState  VAR_scanlineClipper
 * scanlineClipperLineScanner TWScanlineClipperLineScanState  VAR_scanlineClipperLineScanner
 * scanlineClipperNodes TWScanlineClipperNode*  VAR_scanlineClipperNodes
 *
 * call port function #_TCPF_#
 * call port: cRenderTargetBitmapSource signature: sTWRenderTargetBitmapSource context:task
 *   void           cRenderTargetBitmapSource_get( void** outData, TWPixelFormat* outPixelFormat, uint16_t* width, uint16_t* height );
 *
 * #[</PREAMBLE>]# */

/* Put prototype declaration and/or variale definition here #_PAC_# */
#include <assert.h>
#include "tTWRGBX32BitmapGraphicsRenderer_tecsgen.h"
#include "tecsui/private.h"
#include "tecsui/bitmap.h"
#include "tecsui/geometry.h"

#ifndef E_OK
#define	E_OK	0		/* success */
#define	E_ID	(-18)	/* illegal ID */
#endif

static inline uint32_t TWColorToRGBX32(TWColor color) {
	uint32_t ret = color.red;
	ret |= ((uint32_t)color.green) << 8;
	ret |= ((uint32_t)color.blue) << 16;
	return ret;
}

static inline uint32_t RGB565ToRGBX32(uint16_t rgb565) {
	uint32_t ret = (rgb565 & 31) << 3;
	ret |= ((uint32_t)(rgb565 & 0xf800)) << 8;
	ret |= (ret & 0xe000e0) >> 5;
	ret |= ((uint32_t)(rgb565 & 0x7e0)) << 5;
	ret |= ((uint32_t)(rgb565 & 0x600)) >> 1;
	return ret;
}

/* entry port function #_TEPF_# */
/* #[<ENTRY_PORT>]# eGraphicsDeviceOutput
 * entry port: eGraphicsDeviceOutput
 * signature:  sTWGraphicsDeviceOutput
 * context:    task
 * #[</ENTRY_PORT>]# */

/* #[<ENTRY_FUNC>]# eGraphicsDeviceOutput_getScreenSize
 * name:         eGraphicsDeviceOutput_getScreenSize
 * global_name:  tTWRGBX32BitmapGraphicsRenderer_eGraphicsDeviceOutput_getScreenSize
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

/* #[<ENTRY_FUNC>]# eGraphicsDeviceOutput_setClippingRect
 * name:         eGraphicsDeviceOutput_setClippingRect
 * global_name:  tTWRGBX32BitmapGraphicsRenderer_eGraphicsDeviceOutput_setClippingRect
 * oneway:       true
 * #[</ENTRY_FUNC>]# */
void
eGraphicsDeviceOutput_setClippingRect(CELLIDX idx, const TWRect* rect)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);

	TWScanlineClipperInitialize(&VAR_scanlineClipper, VAR_scanlineClipperNodes, ATTR_numScanlineClipperNodes);
	TWScanlineClipperSetClippingRect(&VAR_scanlineClipper, rect);

	TWScanlineClipperInitializeLineScanner(&VAR_scanlineClipperLineScanner);
}

/* #[<ENTRY_FUNC>]# eGraphicsDeviceOutput_subtractClippingRect
 * name:         eGraphicsDeviceOutput_subtractClippingRect
 * global_name:  tTWRGBX32BitmapGraphicsRenderer_eGraphicsDeviceOutput_subtractClippingRect
 * oneway:       true
 * #[</ENTRY_FUNC>]# */
void
eGraphicsDeviceOutput_subtractClippingRect(CELLIDX idx, const TWRect* rect)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);

	TWScanlineClipperSubtractClippingRect(&VAR_scanlineClipper, rect);

	TWScanlineClipperInitializeLineScanner(&VAR_scanlineClipperLineScanner);
}

/* #[<ENTRY_FUNC>]# eGraphicsDeviceOutput_setScissorRect
 * name:         eGraphicsDeviceOutput_setScissorRect
 * global_name:  tTWRGBX32BitmapGraphicsRenderer_eGraphicsDeviceOutput_setScissorRect
 * oneway:       true
 * #[</ENTRY_FUNC>]# */
void
eGraphicsDeviceOutput_setScissorRect(CELLIDX idx, const TWRect* rect)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);

	VAR_scissor = *rect;
}

/* #[<ENTRY_FUNC>]# eGraphicsDeviceOutput_fillRect
 * name:         eGraphicsDeviceOutput_fillRect
 * global_name:  tTWRGBX32BitmapGraphicsRenderer_eGraphicsDeviceOutput_fillRect
 * oneway:       true
 * #[</ENTRY_FUNC>]# */
void
eGraphicsDeviceOutput_fillRect(CELLIDX idx, TWColor color, const TWRect* rect)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);
	uint32_t *pixels;
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

	uint32_t pixel = TWColorToRGBX32(color);

	pixels += x1 + y1 * w;

	TWScanlineClipperSpanScanState spanScanner;
	if (!TWScanlineClipperMoveToLine(&VAR_scanlineClipper, &VAR_scanlineClipperLineScanner, y1, y2 - y1)) {
		return;
	}

	do {
		pixels += w * VAR_scanlineClipperLineScanner.skip;

		if (!TWScanlineClipperStartLine(&VAR_scanlineClipper, &spanScanner, x1, x2 - x1,
		  &VAR_scanlineClipperLineScanner)) {
			goto Continue;
		}

		uint32_t *pixels2 = pixels;
		do {
			pixels2 += spanScanner.skip;
			for (int i = 0; i < spanScanner.width; ++i) {
				*(pixels2++) = pixel;
			}
		} while (TWScanlineClipperLineAdvance(&spanScanner));

	Continue:
		pixels += w;
	} while (TWScanlineClipperMoveToNextLine(&VAR_scanlineClipper, &VAR_scanlineClipperLineScanner));
}

/* #[<ENTRY_FUNC>]# eGraphicsDeviceOutput_drawBitmap
 * name:         eGraphicsDeviceOutput_drawBitmap
 * global_name:  tTWRGBX32BitmapGraphicsRenderer_eGraphicsDeviceOutput_drawBitmap
 * oneway:       true
 * #[</ENTRY_FUNC>]# */
void
eGraphicsDeviceOutput_drawBitmap(CELLIDX idx, const char* data, uint32_t numBytes, const TWBitmapInfo* info, uint16_t infoSize, const TWRect* inRect, const TWPoint* outLoc)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);

	uint32_t *pixels;
	uint16_t tgt_w, tgt_h;
	cRenderTargetBitmapSource_get((void **)&pixels, NULL, &tgt_w, &tgt_h);

	assert(info);
	assert(data);
	assert(infoSize >= sizeof(info->header));

	TWPixelFormat format = info->header.format;

	switch (format) {
		case TWPixelFormat1bppMonotone:
			assert(infoSize >= sizeof(info->mono));
			break;
		default:
			break;
	}

	int src_w = info->header.width, src_h = info->header.height;

	// size check
	size_t stride = TWComputeBitmapStride(format, src_w);
	size_t real_size = stride * src_h; // TODO: overflow check
	assert(numBytes >= real_size);

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

	TWScanlineClipperSpanScanState spanScanner;
	if (!TWScanlineClipperMoveToLine(&VAR_scanlineClipper, &VAR_scanlineClipperLineScanner, out_x1, out_y2 - out_y1)) {
		return;
	}

	switch (format) {
		case TWPixelFormat1bppMonotone: {
			uint32_t pixel1 = TWColorToRGBX32(info->mono.palette[0]);
			uint32_t pixel2 = TWColorToRGBX32(info->mono.palette[1]);
			data += in_x1 >> 3;

			do {
				pixels += tgt_w * VAR_scanlineClipperLineScanner.skip;
				data += stride * VAR_scanlineClipperLineScanner.skip;

				if (!TWScanlineClipperStartLine(&VAR_scanlineClipper, &spanScanner, out_x1, out_x2 - out_x1,
				  &VAR_scanlineClipperLineScanner)) {
					goto Continue1;
				}

				const char *cur_scanline = data;
				unsigned int cur_byte = *(cur_scanline++);
				int cur_bit = in_x1 & 7;
				cur_byte >>= cur_bit;

				uint32_t *pixels2 = pixels;
				do {
					pixels2 += spanScanner.skip;
					// FIXME: optimize
					for (int i = 0; i < spanScanner.skip; ++i) {
						if (cur_bit == 8) {
							cur_bit = 0;
							cur_byte = *(cur_scanline++);
						}
						++cur_bit; cur_byte >>= 1;
					}
					for (int i = 0; i < spanScanner.width; ++i) {
						if (cur_bit == 8) {
							cur_bit = 0;
							cur_byte = *(cur_scanline++);
						}
						*pixels2 = (cur_byte & 1) ? pixel2 : pixel1;
						++cur_bit; cur_byte >>= 1; ++pixels2;
					}
				} while (TWScanlineClipperLineAdvance(&spanScanner));

			Continue1:
				pixels += tgt_w;
				data += stride;
			} while (TWScanlineClipperMoveToNextLine(&VAR_scanlineClipper, &VAR_scanlineClipperLineScanner));
			break;
		}
		case TWPixelFormat32bppRGBX: {
			const uint32_t *in_pixels = (const uint32_t *)data + in_x1;

			do {
				pixels += tgt_w * VAR_scanlineClipperLineScanner.skip;
				in_pixels += src_w * VAR_scanlineClipperLineScanner.skip;

				if (!TWScanlineClipperStartLine(&VAR_scanlineClipper, &spanScanner, out_x1, out_x2 - out_x1,
				  &VAR_scanlineClipperLineScanner)) {
					goto Continue2;
				}

				const uint32_t *in_pixels2 = in_pixels;

				uint32_t *pixels2 = pixels;
				do {
					pixels2 += spanScanner.skip;
					in_pixels2 += spanScanner.skip;
					for (int i = 0; i < spanScanner.width; ++i) {
						*(pixels2++) = *(in_pixels2++);
					}
				} while (TWScanlineClipperLineAdvance(&spanScanner));

			Continue2:
				pixels += tgt_w;
				in_pixels += src_w;
			} while (TWScanlineClipperMoveToNextLine(&VAR_scanlineClipper, &VAR_scanlineClipperLineScanner));

			break;
		}
		case TWPixelFormat16bppRGB565: {
			const uint16_t *in_pixels = (const uint16_t *)data + in_x1;

			do {
				pixels += tgt_w * VAR_scanlineClipperLineScanner.skip;
				in_pixels += src_w * VAR_scanlineClipperLineScanner.skip;

				if (!TWScanlineClipperStartLine(&VAR_scanlineClipper, &spanScanner, out_x1, out_x2 - out_x1,
				  &VAR_scanlineClipperLineScanner)) {
					goto Continue3;
				}

				const uint16_t *in_pixels2 = in_pixels;

				uint32_t *pixels2 = pixels;
				do {
					pixels2 += spanScanner.skip;
					in_pixels2 += spanScanner.skip;
					for (int i = 0; i < spanScanner.width; ++i) {
						*(pixels2++) = RGB565ToRGBX32(*(in_pixels2++));
					}
				} while (TWScanlineClipperLineAdvance(&spanScanner));

			Continue3:
				pixels += tgt_w;
				in_pixels += src_w;
			} while (TWScanlineClipperMoveToNextLine(&VAR_scanlineClipper, &VAR_scanlineClipperLineScanner));

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
 * global_name:  tTWRGBX32BitmapGraphicsRenderer_eGraphicsDeviceOutput_update
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
