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
 * call port: cDC signature: sTWDrawingContext context:task
 *   void           cDC_fillRect( TWColor color, const TWRect* rect );
 *   void           cDC_drawBitmap( const char* data, TWPixelFormat format, const TWSize* bitmapSize, uint32_t numBytes, const TWRect* inRect, const TWPoint* outLoc, TWColor monoColor );
 * call port: cStopwatch signature: sStopwatchControl context:task
 *   void           cStopwatch_start( );
 *   void           cStopwatch_stop( );
 *   void           cStopwatch_freeze( );
 *   void           cStopwatch_unfreeze( );
 *   void           cStopwatch_reset( );
 *   bool           cStopwatch_isRunning( );
 *   bool           cStopwatch_isFreezed( );
 *
 * #[</PREAMBLE>]# */

#include <stdio.h>
#include "tMainWindowCore_tecsgen.h"

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
 * global_name:  tMainWindowCore_ePaintEvent_paint
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
ePaintEvent_paint(CELLIDX idx)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);

	int w = 960, h = 640;
	TWRect r;

	TWSetRect(&r, 0, 0, w, h);
	cDC_fillRect(TWMakeColor(192, 192, 192), &r);
}

/* #[<ENTRY_PORT>]# eStartStopButtonActivated
 * entry port: eStartStopButtonActivated
 * signature:  sAction
 * context:    task
 * #[</ENTRY_PORT>]# */

/* #[<ENTRY_FUNC>]# eStartStopButtonActivated_activated
 * name:         eStartStopButtonActivated_activated
 * global_name:  tMainWindowCore_eStartStopButtonActivated_activated
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eStartStopButtonActivated_activated(CELLIDX idx)
{
    CELLCB  *p_cellcb = GET_CELLCB(idx);
    if (cStopwatch_isRunning()) {
        cStopwatch_stop();
    } else {
        cStopwatch_start();
    }
}

/* #[<ENTRY_PORT>]# eResetLapButtonActivated
 * entry port: eResetLapButtonActivated
 * signature:  sAction
 * context:    task
 * #[</ENTRY_PORT>]# */

/* #[<ENTRY_FUNC>]# eResetLapButtonActivated_activated
 * name:         eResetLapButtonActivated_activated
 * global_name:  tMainWindowCore_eResetLapButtonActivated_activated
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eResetLapButtonActivated_activated(CELLIDX idx)
{
    CELLCB  *p_cellcb = GET_CELLCB(idx);

    cStopwatch_stop();
    cStopwatch_reset();
}

/* #[<POSTAMBLE>]#
 *   Put non-entry functions below.
 * #[</POSTAMBLE>]#*/
