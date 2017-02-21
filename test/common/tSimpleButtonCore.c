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
 * active           uint8_t          VAR_active
 *
 * call port function #_TCPF_#
 * call port: cKeyboardInputManager signature: sTWKeyboardInputManagerControl context:task
 *   void           cKeyboardInputManager_setEventReceiver( Descriptor( sTWKeyboardEvent ) receiver );
 *   void           cKeyboardInputManager_clearEventReceiver( );
 *   uint8_t        cKeyboardInputManager_isEventReceiver( Descriptor( sTWKeyboardEvent ) receiver );
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
 * call port: cAction signature: sAction context:task optional:true
 *   bool_t     is_cAction_joined()                     check if joined
 *   void           cAction_activated( );
 * call port: cKeyboardEvent signature: sTWKeyboardEvent context:task
 *   void           cKeyboardEvent_keyDown( uint16_t keyCode );
 *   void           cKeyboardEvent_keyUp( uint16_t keyCode );
 *   void           cKeyboardEvent_enter( );
 *   void           cKeyboardEvent_leave( );
 *   [ref_desc]
 *      Descriptor( sTWKeyboardEvent ) cKeyboardEvent_refer_to_descriptor();
 *      Descriptor( sTWKeyboardEvent ) cKeyboardEvent_ref_desc()      (same as above; abbreviated version);
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

	if (VAR_active) {
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

	if (cKeyboardInputManager_isEventReceiver(cKeyboardEvent_refer_to_descriptor())) {
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

/* #[<ENTRY_PORT>]# eTouchEvent
 * entry port: eTouchEvent
 * signature:  sTWTouchEvent
 * context:    task
 * #[</ENTRY_PORT>]# */

/* #[<ENTRY_FUNC>]# eTouchEvent_touchStart
 * name:         eTouchEvent_touchStart
 * global_name:  tSimpleButtonCore_eTouchEvent_touchStart
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eTouchEvent_touchStart(CELLIDX idx, uint8_t touchId, TWPoint location)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);

	if (touchId != 0) {
		return;
	}

	VAR_active = 1;

	cKeyboardInputManager_setEventReceiver(cKeyboardEvent_refer_to_descriptor());
	cViewControl_setNeedsUpdate();
}

/* #[<ENTRY_FUNC>]# eTouchEvent_touchMove
 * name:         eTouchEvent_touchMove
 * global_name:  tSimpleButtonCore_eTouchEvent_touchMove
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eTouchEvent_touchMove(CELLIDX idx, uint8_t touchId, TWPoint location)
{
	(void) idx;
}

/* #[<ENTRY_FUNC>]# eTouchEvent_touchEnd
 * name:         eTouchEvent_touchEnd
 * global_name:  tSimpleButtonCore_eTouchEvent_touchEnd
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eTouchEvent_touchEnd(CELLIDX idx, uint8_t touchId, TWPoint location)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);

	if (touchId != 0) {
		return;
	}

	VAR_active = 0;

	TWRect bounds;
	cBoundsSource_get(&bounds);

	// is the mouse pointer still inside the button?
	// (user can cancel the action by moving the mouse pointer
	// outside the button)
	if (location.x >= 0 && location.y >= 0 && location.x < bounds.w && location.y < bounds.h) {
		if (is_cAction_joined()) {
			cAction_activated();
		}
	}

	cViewControl_setNeedsUpdate();
}

/* #[<ENTRY_FUNC>]# eTouchEvent_touchCancel
 * name:         eTouchEvent_touchCancel
 * global_name:  tSimpleButtonCore_eTouchEvent_touchCancel
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eTouchEvent_touchCancel(CELLIDX idx, uint8_t touchId, TWPoint location)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);

	if (touchId != 0) {
		return;
	}

	VAR_active = 0;

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

/* #[<ENTRY_FUNC>]# eKeyboardEvent_enter
 * name:         eKeyboardEvent_enter
 * global_name:  tSimpleButtonCore_eKeyboardEvent_enter
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eKeyboardEvent_enter(CELLIDX idx)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);
	cViewControl_setNeedsUpdate();
}

/* #[<ENTRY_FUNC>]# eKeyboardEvent_leave
 * name:         eKeyboardEvent_leave
 * global_name:  tSimpleButtonCore_eKeyboardEvent_leave
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eKeyboardEvent_leave(CELLIDX idx)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);
	cViewControl_setNeedsUpdate();
}

/* #[<POSTAMBLE>]#
 *   Put non-entry functions below.
 * #[</POSTAMBLE>]#*/
