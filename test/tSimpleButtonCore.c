/*
 * Copyright (C) 2016 Tomoaki Kawada
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
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
 *   void           cDrawingContext_drawBitmap( const char* data, TWPixelFormat format, const TWSize* bitmapSize, uint32_t numBytes, const TWRect* inRect, const TWPoint* outLoc, TWColor monoColor );
 * call port: cViewControl signature: sTWViewControl context:task
 *   TWViewStyle    cViewControl_getStyle( );
 *   void           cViewControl_getBounds( TWRect* outRect );
 *   void           cViewControl_getGlobalBounds( TWRect* outRect );
 *   void           cViewControl_setNeedsUpdate( );
 *   void           cViewControl_setFocus( );
 *   bool           cViewControl_isFocused( );
 *   void           cViewControl_setMouseCapture( );
 *   void           cViewControl_releaseMouseCapture( );
 *   bool           cViewControl_hasMouseCapture( );
 * call port: cAction signature: sAction context:task optional:true
 *   bool_t     is_cAction_joined()                     check if joined
 *   void           cAction_activated( );
 *
 * #[</PREAMBLE>]# */

/* Put prototype declaration and/or variale definition here #_PAC_# */
#include "tSimpleButtonCore_tecsgen.h"

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
 * global_name:  tSimpleButtonCore_ePaintEvent_paint
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
ePaintEvent_paint(CELLIDX idx)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);
	
	TWRect bounds;
	cBoundsSource_get(&bounds);

	int w = bounds.w, h = bounds.h;
	TWRect r;

	TWColor shade1 = TWMakeColor(192, 192, 192);
	TWColor shade2 = TWMakeColor(255, 255, 255);
	TWColor shade3 = TWMakeColor(64, 64, 64);
	TWColor shade4 = TWMakeColor(128, 128, 128);

	if (cViewControl_hasMouseCapture()) {
		shade1 = TWMakeColor(64, 64, 64);
		shade2 = TWMakeColor(128, 128, 128);
		shade3 = TWMakeColor(255, 255, 255);
		shade4 = TWMakeColor(192, 192, 192);
	}

	TWSetRect(&r, 0, 0, w, 1);
	cDrawingContext_fillRect(shade1, &r);

	TWSetRect(&r, 0, 1, 1, h - 2);
	cDrawingContext_fillRect(shade1, &r);

	TWSetRect(&r, 1, 1, w - 2, 1);
	cDrawingContext_fillRect(shade2, &r);

	TWSetRect(&r, 1, 2, 1, h - 4);
	cDrawingContext_fillRect(shade2, &r);

	TWSetRect(&r, 0, h - 1, w, 1);
	cDrawingContext_fillRect(shade3, &r);

	TWSetRect(&r, w - 1, 1, 1, h - 2);
	cDrawingContext_fillRect(shade3, &r);

	TWSetRect(&r, 1, h - 2, w - 2, 1);
	cDrawingContext_fillRect(shade4, &r);

	TWSetRect(&r, w - 2, 2, 1, h - 4);
	cDrawingContext_fillRect(shade4, &r);

	TWSetRect(&r, 2, 2, w - 4, h - 4);
	cDrawingContext_fillRect(TWMakeColor(192, 192, 192), &r);

	if (cViewControl_isFocused()) {
		TWColor color = TWMakeColor(0, 0, 0);
		r.w = 1; r.h = 1;
		for (int x = 4; x < w - 4; x += 2) {
			r.x = x; r.y = 4;
			cDrawingContext_fillRect(color, &r);
			r.y = h - 5;
			cDrawingContext_fillRect(color, &r);
		}
		for (int y = 4; y < h - 4; y += 2) {
			r.y = y; r.x = 4;
			cDrawingContext_fillRect(color, &r);
			r.x = w - 5;
			cDrawingContext_fillRect(color, &r);
		}
	}
}

/* #[<ENTRY_PORT>]# eMouseEvent
 * entry port: eMouseEvent
 * signature:  sTWMouseEvent
 * context:    task
 * #[</ENTRY_PORT>]# */

/* #[<ENTRY_FUNC>]# eMouseEvent_mouseDown
 * name:         eMouseEvent_mouseDown
 * global_name:  tSimpleButtonCore_eMouseEvent_mouseDown
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eMouseEvent_mouseDown(CELLIDX idx, TWPoint point, uint8_t button)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);

	if (button != 1) {
		return;
	}

	cViewControl_setFocus();
	cViewControl_setMouseCapture();
	cViewControl_setNeedsUpdate();
}

/* #[<ENTRY_FUNC>]# eMouseEvent_mouseMove
 * name:         eMouseEvent_mouseMove
 * global_name:  tSimpleButtonCore_eMouseEvent_mouseMove
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eMouseEvent_mouseMove(CELLIDX idx, TWPoint point)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);
}

/* #[<ENTRY_FUNC>]# eMouseEvent_mouseUp
 * name:         eMouseEvent_mouseUp
 * global_name:  tSimpleButtonCore_eMouseEvent_mouseUp
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eMouseEvent_mouseUp(CELLIDX idx, TWPoint point, uint8_t button)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);

	if (button != 1) {
		return;
	}

	TWRect bounds;
	cBoundsSource_get(&bounds);

	// is the mouse pointer still inside the button?
	// (user can cancel the action by moving the mouse pointer
	// outside the button)
	if (point.x >= 0 && point.y >= 0 && point.x < bounds.w && point.y < bounds.h) {
		if (is_cAction_joined()) {
			cAction_activated();
		}
	}

	cViewControl_releaseMouseCapture();
	cViewControl_setNeedsUpdate();
}

/* #[<ENTRY_PORT>]# eKeyboardEvent
 * entry port: eKeyboardEvent
 * signature:  sTWKeyboardEvent
 * context:    task
 * #[</ENTRY_PORT>]# */

/* #[<ENTRY_FUNC>]# eKeyboardEvent_keyDown
 * name:         eKeyboardEvent_keyDown
 * global_name:  tSimpleButtonCore_eKeyboardEvent_keyDown
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eKeyboardEvent_keyDown(CELLIDX idx, uint16_t keyCode)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);

	// keyCode is actually implmenetation defined but we assume
	// keyCode is actually an ASCII code for now.
	if (keyCode == ' ' || keyCode == 13) {
		if (is_cAction_joined()) {
			cAction_activated();
		}
	}
}

/* #[<ENTRY_FUNC>]# eKeyboardEvent_keyUp
 * name:         eKeyboardEvent_keyUp
 * global_name:  tSimpleButtonCore_eKeyboardEvent_keyUp
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eKeyboardEvent_keyUp(CELLIDX idx, uint16_t keyCode)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);
}

/* #[<ENTRY_PORT>]# eFocusEvent
 * entry port: eFocusEvent
 * signature:  sTWFocusEvent
 * context:    task
 * #[</ENTRY_PORT>]# */

/* #[<ENTRY_FUNC>]# eFocusEvent_enter
 * name:         eFocusEvent_enter
 * global_name:  tSimpleButtonCore_eFocusEvent_enter
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eFocusEvent_enter(CELLIDX idx)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);
	cViewControl_setNeedsUpdate();
}

/* #[<ENTRY_FUNC>]# eFocusEvent_leave
 * name:         eFocusEvent_leave
 * global_name:  tSimpleButtonCore_eFocusEvent_leave
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eFocusEvent_leave(CELLIDX idx)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);
	cViewControl_setNeedsUpdate();
}

/* #[<POSTAMBLE>]#
 *   Put non-entry functions below.
 * #[</POSTAMBLE>]#*/
