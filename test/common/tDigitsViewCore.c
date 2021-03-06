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
 * call port function #_TCPF_#
 * call port: cBoundsSource signature: sTWRectSource context:task
 *   void           cBoundsSource_get( TWRect* outRect );
 * call port: cDrawingContext signature: sTWDrawingContext context:task
 *   void           cDrawingContext_fillRect( TWColor color, const TWRect* rect );
 *   void           cDrawingContext_drawBitmap( const char* data, uint32_t numBytes, const TWBitmapInfo* info, uint16_t infoSize, const TWRect* inRect, const TWPoint* outLoc );
 * call port: cViewControl signature: sTWViewControl context:task
 *   TWViewStyle    cViewControl_getStyle( );
 *   void           cViewControl_getBounds( TWRect* outRect );
 *   void           cViewControl_getGlobalBounds( TWRect* outRect );
 *   void           cViewControl_setNeedsUpdate( );
 * call port: cTextSource signature: sStringValueSource context:task
 *   void           cTextSource_get( char* outString, uint16_t bufferSize );
 * call port: cBitmapData signature: sBitmapData context:task
 *   void           cBitmapData_get( const char** data, uint32_t* numBytes, TWBitmapInfoHeader* infoHeader );
 *
 * #[</PREAMBLE>]# */

/* Put prototype declaration and/or variale definition here #_PAC_# */
#include <stdlib.h>
#include "tDigitsViewCore_tecsgen.h"
#ifndef E_OK
#define	E_OK	0		/* success */
#define	E_ID	(-18)	/* illegal ID */
#endif

#define kGlyphHeight Image_Digits_Height
static const struct DigitsViewGlyphInfo {
	uint8_t x, w;
} glyphs[] = {
	{0, 18}, // 0
	{18, 18}, // 1
	{36, 18}, // 2
	{54, 18}, // 3
	{72, 18}, // 4
	{90, 18}, // 5
	{108, 18}, // 6
	{126, 18}, // 7
	{144, 18}, // 8
	{162, 18}, // 9
	{181, 8}, // .
	{190, 9}, // :
};

static const struct DigitsViewGlyphInfo *
FindGlyph(char c)
{
	switch (c) {
		case '0': return &glyphs[0];
		case '1': return &glyphs[1];
		case '2': return &glyphs[2];
		case '3': return &glyphs[3];
		case '4': return &glyphs[4];
		case '5': return &glyphs[5];
		case '6': return &glyphs[6];
		case '7': return &glyphs[7];
		case '8': return &glyphs[8];
		case '9': return &glyphs[9];
		case '.': return &glyphs[10];
		case ':': return &glyphs[11];
		default: return NULL;
	}
}

/* entry port function #_TEPF_# */
/* #[<ENTRY_PORT>]# ePaintEvent
 * entry port: ePaintEvent
 * signature:  sTWPaintEvent
 * context:    task
 * #[</ENTRY_PORT>]# */

/* #[<ENTRY_FUNC>]# ePaintEvent_paint
 * name:         ePaintEvent_paint
 * global_name:  tDigitsViewCore_ePaintEvent_paint
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
ePaintEvent_paint(CELLIDX idx)
{
	CELLCB *p_cellcb = GET_CELLCB(idx);

	TWRect bounds;
	cBoundsSource_get(&bounds);

	int w = bounds.w, h = bounds.h;
	TWRect r;
	TWPoint p;

	TWColor shade1 = TWMakeColor(128, 128, 128);
	TWColor shade3 = TWMakeColor(255, 255, 255);

	TWSetRect(&r, 0, 0, w, 1);
	cDrawingContext_fillRect(shade1, &r);

	TWSetRect(&r, 0, 1, 1, h - 2);
	cDrawingContext_fillRect(shade1, &r);

	TWSetRect(&r, 0, h - 1, w, 1);
	cDrawingContext_fillRect(shade3, &r);

	TWSetRect(&r, w - 1, 1, 1, h - 2);
	cDrawingContext_fillRect(shade3, &r);

	// Draw text
	char str[32];
	cTextSource_get(str, 32);
	str[31] = 0;

	const char *bitmapData;
	uint32_t bitmapSize;
	TWBitmapInfo info;
	cBitmapData_get(&bitmapData, &bitmapSize, &info.header);

	int textH = info.header.height;
	int textY = (h - textH) >> 1;

	info.mono.palette[0] = TWMakeColor(0, 0, 0);
	info.mono.palette[1] = TWMakeColor(255, 255, 0);

	int x = 1;
	for (const char *c = str; *c; ++c) {
		const struct DigitsViewGlyphInfo *glyph = FindGlyph(*c);
		if (glyph) {
			TWSetRect(&r, glyph->x, 0, glyph->w, textH);
			p = TWMakePoint(x, textY);
			cDrawingContext_drawBitmap(bitmapData, bitmapSize, &info, sizeof(info), &r, &p);
			x += glyph->w;
		}
	}

	// Fill margin
	TWSetRect(&r, x, 1, w - x - 1, h - 2);
	cDrawingContext_fillRect(info.mono.palette[0], &r);
	TWSetRect(&r, 1, 1, w - 2, textY - 1);
	cDrawingContext_fillRect(info.mono.palette[0], &r);
	TWSetRect(&r, 1, textY + textH, w - 2, h - 1 - textY - textH);
	cDrawingContext_fillRect(info.mono.palette[0], &r);
}

/* #[<ENTRY_PORT>]# eTextSource
 * entry port: eTextSource
 * signature:  sTWValueSourceCallback
 * context:    task
 * #[</ENTRY_PORT>]# */

/* #[<ENTRY_FUNC>]# eTextSource_changing
 * name:         eTextSource_changing
 * global_name:  tDigitsViewCore_eTextSource_changing
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eTextSource_changing(CELLIDX idx)
{
}

/* #[<ENTRY_FUNC>]# eTextSource_changed
 * name:         eTextSource_changed
 * global_name:  tDigitsViewCore_eTextSource_changed
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eTextSource_changed(CELLIDX idx)
{
	CELLCB *p_cellcb = GET_CELLCB(idx);

	cViewControl_setNeedsUpdate();
}

/* #[<POSTAMBLE>]#
 *   Put non-entry functions below.
 * #[</POSTAMBLE>]#*/
