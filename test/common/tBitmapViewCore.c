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
 * call port: cBitmapData signature: sBitmapData context:task
 *   void           cBitmapData_get( const char** data, uint32_t* numBytes, TWBitmapInfoHeader* infoHeader );
 *
 * #[</PREAMBLE>]# */

/* Put prototype declaration and/or variale definition here #_PAC_# */
#include "tBitmapViewCore_tecsgen.h"

#ifndef E_OK
#define	E_OK	0		/* success */
#define	E_ID	(-18)	/* illegal ID */
#endif

/* entry port function #_TEPF_# */
/* #[<ENTRY_PORT>]# ePaintEvent
 * entry port: ePaintEvent
 * signature:  sTWPaintEvent
 * context:    task
 * #[</ENTRY_PORT>]# */

/* #[<ENTRY_FUNC>]# ePaintEvent_paint
 * name:         ePaintEvent_paint
 * global_name:  tBitmapViewCore_ePaintEvent_paint
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
ePaintEvent_paint(CELLIDX idx)
{
	CELLCB *p_cellcb = GET_CELLCB(idx);

    const char *bitmapData;
    uint32_t bitmapSize;
    TWBitmapInfo info;
    cBitmapData_get(&bitmapData, &bitmapSize, &info.header);

    // FIXME: make palette customizable
    info.mono.palette[0] = TWMakeColor(192, 192, 192);
    info.mono.palette[1] = TWMakeColor(0, 0, 0);

    TWRect r = {0, 0, info.header.width, info.header.height};
    TWPoint p = {0, 0};

    cDrawingContext_drawBitmap(bitmapData, bitmapSize, &info, sizeof(info), &r, &p);
}

/* #[<POSTAMBLE>]#
 *   Put non-entry functions below.
 * #[</POSTAMBLE>]#*/
