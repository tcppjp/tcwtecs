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
 * call port: cSuperview signature: sTWSuperviewLink context:task optional:true
 *   bool_t     is_cSuperview_joined()                     check if joined
 *   void           cSuperview_setNeedsUpdate( const TWRect* bounds );
 *   void           cSuperview_getGlobalLocation( TWPoint* outLoc );
 * call port: cSubview signature: sTWSubviewLink context:task optional:true
 *   bool_t     is_cSubview_joined(int subscript)        check if joined
 *   bool           cSubview_mouseDown( subscript, TWPoint point, uint8_t button );
 *   bool           cSubview_mouseMove( subscript, TWPoint point );
 *   bool           cSubview_mouseUp( subscript, TWPoint point, uint8_t button );
 *   void           cSubview_keyDown( subscript, uint16_t keyCode );
 *   void           cSubview_keyUp( subscript, uint16_t keyCode );
 *   void           cSubview_paint( subscript, const TWRect* clipRect, const TWRect* globalBounds );
 *   int            cSubview_keyboardFocusTargetChanged( subscript, void* newTarget, void* oldTarget );
 *       subscript:  0...(NCP_cSubview-1)
 * call port: cDesktopLink signature: sTWDesktopLink context:task
 *   void*          cDesktopLink_getMouseCaptureTarget( );
 *   void           cDesktopLink_setMouseCaptureTarget( void* newTarget );
 *   void*          cDesktopLink_getKeyboardFocusTarget( );
 *   void           cDesktopLink_setKeyboardFocusTarget( void* newTarget );
 *   void           cDesktopLink_fillRect( TWColor color, const TWRect* rect );
 *   void           cDesktopLink_drawBitmap( const char* data, TWPixelFormat format, const TWSize* bitmapSize, uint32_t numBytes, const TWRect* inRect, const TWPoint* outLoc, TWColor monoColor );
 *   void           cDesktopLink_preparePaint( const TWRect* globalClipRect, const TWPoint* globalLoc );
 * call port: cMouseEvent signature: sTWMouseEvent context:task optional:true
 *   bool_t     is_cMouseEvent_joined()                     check if joined
 *   void           cMouseEvent_mouseDown( TWPoint point, uint8_t button );
 *   void           cMouseEvent_mouseMove( TWPoint point );
 *   void           cMouseEvent_mouseUp( TWPoint point, uint8_t button );
 * call port: cKeyboardEvent signature: sTWKeyboardEvent context:task optional:true
 *   bool_t     is_cKeyboardEvent_joined()                     check if joined
 *   void           cKeyboardEvent_keyDown( uint16_t keyCode );
 *   void           cKeyboardEvent_keyUp( uint16_t keyCode );
 * call port: cFocusEvent signature: sTWFocusEvent context:task optional:true
 *   bool_t     is_cFocusEvent_joined()                     check if joined
 *   void           cFocusEvent_enter( );
 *   void           cFocusEvent_leave( );
 * call port: cPaintEvent signature: sTWPaintEvent context:task optional:true
 *   bool_t     is_cPaintEvent_joined()                     check if joined
 *   void           cPaintEvent_paint( );
 * call port: cStyleSource signature: sTWViewStyleSource context:task
 *   TWViewStyle    cStyleSource_get( );
 * call port: cBoundsSource signature: sTWRectSource context:task
 *   void           cBoundsSource_get( TWRect* outRect );
 *
 * #[</PREAMBLE>]# */

#include <assert.h>
#include "tTWView_tecsgen.h"

#ifndef E_OK
#define	E_OK	0		/* success */
#define	E_ID	(-18)	/* illegal ID */
#endif

TWViewStyle GetViewStyle(CELLCB *p_cellcb) {
	return cStyleSource_get();
}

void GetViewBounds(CELLCB *p_cellcb, TWRect *outRect) {
	cBoundsSource_get(outRect);
}

void *GetIdentifier(CELLCB *p_cellcb) {
	return (void *)p_cellcb;
}

/* entry port function #_TEPF_# */
/* #[<ENTRY_PORT>]# eSuperview
 * entry port: eSuperview
 * signature:  sTWSubviewLink
 * context:    task
 * #[</ENTRY_PORT>]# */

/* #[<ENTRY_FUNC>]# eSuperview_mouseDown
 * name:         eSuperview_mouseDown
 * global_name:  tTWView_eSuperview_mouseDown
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
bool
eSuperview_mouseDown(CELLIDX idx, TWPoint point, uint8_t button)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);
	void *idt = GetIdentifier(p_cellcb);
	void *captured = cDesktopLink_getMouseCaptureTarget();
	TWRect bounds;
	GetViewBounds(p_cellcb, &bounds);
	point.x -= bounds.x;
	point.y -= bounds.y;
	if (captured != NULL) {
		if (captured != idt) {
		  PropagateMessage:
			// there's a captured view; broadcast the message
			for (int_t i = 0, count = NCP_cSubview; i < count; ++i) {
				if (cSubview_mouseDown(i, point, button)) {
					return true;
				}
			}
		}
		if (captured == idt) {
			// I am the receiver
			if (is_cMouseEvent_joined()) {
				cMouseEvent_mouseDown(point, button);
			}
			return true;
		}
		return false;
	} else {
		if ((GetViewStyle(p_cellcb) & TWViewStyleVisible) == 0) {
			// invisible!
			return false;
		}
		if (point.x >= 0 && point.y >= 0 &&
			point.x < bounds.w && point.y < bounds.h) {
			idt = NULL; // this will cause the "I am the reciver" part to be evaluated
			goto PropagateMessage;
		} else {
			return false;
		}
	}
}

/* #[<ENTRY_FUNC>]# eSuperview_mouseMove
 * name:         eSuperview_mouseMove
 * global_name:  tTWView_eSuperview_mouseMove
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
bool
eSuperview_mouseMove(CELLIDX idx, TWPoint point)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);
	void *idt = GetIdentifier(p_cellcb);
	void *captured = cDesktopLink_getMouseCaptureTarget();
	TWRect bounds;
	GetViewBounds(p_cellcb, &bounds);
	point.x -= bounds.x;
	point.y -= bounds.y;
	if (captured != NULL) {
		if (captured != idt) {
		  PropagateMessage:
			// there's a captured view; broadcast the message
			for (int_t i = 0, count = NCP_cSubview; i < count; ++i) {
				if (cSubview_mouseMove(i, point)) {
					return true;
				}
			}
		}
		if (captured == idt) {
			// I am the receiver
			if (is_cMouseEvent_joined()) {
				cMouseEvent_mouseMove(point);
			}
			return true;
		}
		return false;
	} else {
		if ((GetViewStyle(p_cellcb) & TWViewStyleVisible) == 0) {
			// invisible!
			return false;
		}
		if (point.x >= 0 && point.y >= 0 &&
			point.x < bounds.w && point.y < bounds.h) {
			idt = NULL; // this will cause the "I am the reciver" part to be evaluated
			goto PropagateMessage;
		} else {
			return false;
		}
	}
}

/* #[<ENTRY_FUNC>]# eSuperview_mouseUp
 * name:         eSuperview_mouseUp
 * global_name:  tTWView_eSuperview_mouseUp
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
bool
eSuperview_mouseUp(CELLIDX idx, TWPoint point, uint8_t button)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);
	void *idt = GetIdentifier(p_cellcb);
	void *captured = cDesktopLink_getMouseCaptureTarget();
	TWRect bounds;
	GetViewBounds(p_cellcb, &bounds);
	point.x -= bounds.x;
	point.y -= bounds.y;
	if (captured != NULL) {
		if (captured != idt) {
		  PropagateMessage:
			// there's a captured view; broadcast the message
			for (int_t i = 0, count = NCP_cSubview; i < count; ++i) {
				if (cSubview_mouseUp(i, point, button)) {
					return true;
				}
			}
		}
		if (captured == idt) {
			// I am the receiver
			if (is_cMouseEvent_joined()) {
				cMouseEvent_mouseUp(point, button);
			}
			return true;
		}
		return false;
	} else {	
		if ((GetViewStyle(p_cellcb) & TWViewStyleVisible) == 0) {
			// invisible!
			return false;
		}
		if (point.x >= 0 && point.y >= 0 &&
			point.x < bounds.w && point.y < bounds.h) {
			idt = NULL; // this will cause the "I am the reciver" part to be evaluated
			goto PropagateMessage;
		} else {
			return false;
		}
	}
}

/* #[<ENTRY_FUNC>]# eSuperview_keyDown
 * name:         eSuperview_keyDown
 * global_name:  tTWView_eSuperview_keyDown
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eSuperview_keyDown(CELLIDX idx, uint16_t keyCode)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);
	void *idt = GetIdentifier(p_cellcb);
	if (idt == cDesktopLink_getKeyboardFocusTarget()) {
		if (is_cKeyboardEvent_joined()) {
			cKeyboardEvent_keyDown(keyCode);
		}
	} else {
		for (int_t i = 0, count = NCP_cSubview; i < count; ++i) {
			cSubview_keyDown(i, keyCode);
		}
	}
}

/* #[<ENTRY_FUNC>]# eSuperview_keyUp
 * name:         eSuperview_keyUp
 * global_name:  tTWView_eSuperview_keyUp
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eSuperview_keyUp(CELLIDX idx, uint16_t keyCode)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);
	void *idt = GetIdentifier(p_cellcb);
	if (idt == cDesktopLink_getKeyboardFocusTarget()) {
		if (is_cKeyboardEvent_joined()) {
			cKeyboardEvent_keyUp(keyCode);
		}
	} else {
		for (int_t i = 0, count = NCP_cSubview; i < count; ++i) {
			cSubview_keyUp(i, keyCode);
		}
	}
}

/* #[<ENTRY_FUNC>]# eSuperview_paint
 * name:         eSuperview_paint
 * global_name:  tTWView_eSuperview_paint
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eSuperview_paint(CELLIDX idx, const TWRect* clipRect, const TWRect* globalBounds)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);
	
	if ((GetViewStyle(p_cellcb) & TWViewStyleVisible) == 0) {
		// invisible!
		return;
	}

	TWRect self_glob;
	GetViewBounds(p_cellcb, &self_glob);
	self_glob.x += globalBounds->x;
	self_glob.y += globalBounds->y;

	TWRect new_clip;
	if (!TWRectIntersect(&self_glob, clipRect, &new_clip)) {
		// culled
		return;
	}

	// paint myself
	if (is_cPaintEvent_joined()) {
		TWPoint self_glob_loc = {self_glob.x, self_glob.y};
		cDesktopLink_preparePaint(&new_clip, &self_glob_loc);
		cPaintEvent_paint();
	}

	// paint subviews
	for (int_t i = 0, count = NCP_cSubview; i < count; ++i) {
		cSubview_paint(i, &new_clip, &self_glob);
	}
}

/* #[<ENTRY_FUNC>]# eSuperview_keyboardFocusTargetChanged
 * name:         eSuperview_keyboardFocusTargetChanged
 * global_name:  tTWView_eSuperview_keyboardFocusTargetChanged
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
int
eSuperview_keyboardFocusTargetChanged(CELLIDX idx, void* newTarget, void* oldTarget)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);

	assert(newTarget != oldTarget);

	int result = 0;
	void *me = GetIdentifier(p_cellcb);
	if (newTarget == me) {
		result = 1;
	} else if (oldTarget == me) {
		result = -1;
	}

	for (int_t i = 0, count = NCP_cSubview; i < count; ++i) {
		result += cSubview_keyboardFocusTargetChanged(i, newTarget, oldTarget);
	}

	assert(result >= -1 && result <= 1);

	if (is_cFocusEvent_joined()) {
		if (result > 0) {
			cFocusEvent_enter();
		} else if (result < 0) {
			cFocusEvent_leave();
		}
	}

	return result;
}

/* #[<ENTRY_PORT>]# eSubview
 * entry port: eSubview
 * signature:  sTWSuperviewLink
 * context:    task
 * entry port array size:  NEP_eSubview
 * #[</ENTRY_PORT>]# */

/* #[<ENTRY_FUNC>]# eSubview_setNeedsUpdate
 * name:         eSubview_setNeedsUpdate
 * global_name:  tTWView_eSubview_setNeedsUpdate
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eSubview_setNeedsUpdate(CELLIDX idx, int_t subscript, const TWRect* bounds)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);

	if ((GetViewStyle(p_cellcb) & TWViewStyleVisible) == 0) {
		// invisible!
		return;
	}

	TWRect outer_bounds = *bounds;
	TWRect view_bounds;
	GetViewBounds(p_cellcb, &view_bounds);
	outer_bounds.x += view_bounds.x;
	outer_bounds.y += view_bounds.y;

	// clip
	if (!TWRectIntersect(&outer_bounds, &view_bounds, &outer_bounds)) {
		// outside this view
		return;
	}

	cSuperview_setNeedsUpdate(&outer_bounds);
}

/* #[<ENTRY_FUNC>]# eSubview_getGlobalLocation
 * name:         eSubview_getGlobalLocation
 * global_name:  tTWView_eSubview_getGlobalLocation
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eSubview_getGlobalLocation(CELLIDX idx, int_t subscript, TWPoint* outLoc)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);
	cSuperview_getGlobalLocation(outLoc);
	TWRect bounds;
	GetViewBounds(p_cellcb, &bounds);
	outLoc->x += bounds.x;
	outLoc->y += bounds.y;
}

/* #[<ENTRY_PORT>]# eControl
 * entry port: eControl
 * signature:  sTWViewControl
 * context:    task
 * #[</ENTRY_PORT>]# */

/* #[<ENTRY_FUNC>]# eControl_getStyle
 * name:         eControl_getStyle
 * global_name:  tTWView_eControl_getStyle
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
TWViewStyle
eControl_getStyle(CELLIDX idx)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);
	return GetViewStyle(p_cellcb);
}

/* #[<ENTRY_FUNC>]# eControl_getBounds
 * name:         eControl_getBounds
 * global_name:  tTWView_eControl_getBounds
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eControl_getBounds(CELLIDX idx, TWRect* outRect)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);
	GetViewBounds(p_cellcb, outRect);
}

/* #[<ENTRY_FUNC>]# eControl_getGlobalBounds
 * name:         eControl_getGlobalBounds
 * global_name:  tTWView_eControl_getGlobalBounds
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eControl_getGlobalBounds(CELLIDX idx, TWRect* outRect)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);
	TWPoint superview_loc;
	cSuperview_getGlobalLocation(&superview_loc);

	TWRect bounds;
	GetViewBounds(p_cellcb, &bounds);

	outRect->x = bounds.x + superview_loc.x;
	outRect->y = bounds.y + superview_loc.y;
	outRect->w = bounds.w;
	outRect->h = bounds.h;
}

/* #[<ENTRY_FUNC>]# eControl_setNeedsUpdate
 * name:         eControl_setNeedsUpdate
 * global_name:  tTWView_eControl_setNeedsUpdate
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eControl_setNeedsUpdate(CELLIDX idx)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);

	if ((GetViewStyle(p_cellcb) & TWViewStyleVisible) == 0) {
		// invisible!
		return;
	}

	TWRect bounds;
	GetViewBounds(p_cellcb, &bounds);
	cSuperview_setNeedsUpdate(&bounds);
}

/* #[<ENTRY_FUNC>]# eControl_setFocus
 * name:         eControl_setFocus
 * global_name:  tTWView_eControl_setFocus
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eControl_setFocus(CELLIDX idx)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);

	// TODO: prevent invisible view from getting a focus?

	cDesktopLink_setKeyboardFocusTarget(GetIdentifier(p_cellcb));
}

/* #[<ENTRY_FUNC>]# eControl_isFocused
 * name:         eControl_isFocused
 * global_name:  tTWView_eControl_isFocused
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
bool
eControl_isFocused(CELLIDX idx)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);
	return cDesktopLink_getKeyboardFocusTarget() == GetIdentifier(p_cellcb);
}

/* #[<ENTRY_FUNC>]# eControl_setMouseCapture
 * name:         eControl_setMouseCapture
 * global_name:  tTWView_eControl_setMouseCapture
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eControl_setMouseCapture(CELLIDX idx)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);

	if (cDesktopLink_getMouseCaptureTarget() != NULL) {
		return;
	}

	cDesktopLink_setMouseCaptureTarget(GetIdentifier(p_cellcb));
}

/* #[<ENTRY_FUNC>]# eControl_releaseMouseCapture
 * name:         eControl_releaseMouseCapture
 * global_name:  tTWView_eControl_releaseMouseCapture
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eControl_releaseMouseCapture(CELLIDX idx)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);

	if (cDesktopLink_getMouseCaptureTarget() != GetIdentifier(p_cellcb)) {
		return;
	}

	cDesktopLink_setMouseCaptureTarget(NULL);
}

/* #[<ENTRY_FUNC>]# eControl_hasMouseCapture
 * name:         eControl_hasMouseCapture
 * global_name:  tTWView_eControl_hasMouseCapture
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
bool
eControl_hasMouseCapture(CELLIDX idx)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);
	return cDesktopLink_getMouseCaptureTarget() == GetIdentifier(p_cellcb);
}

/* #[<ENTRY_PORT>]# eDrawingContext
 * entry port: eDrawingContext
 * signature:  sTWDrawingContext
 * context:    task
 * #[</ENTRY_PORT>]# */

/* #[<ENTRY_FUNC>]# eDrawingContext_fillRect
 * name:         eDrawingContext_fillRect
 * global_name:  tTWView_eDrawingContext_fillRect
 * oneway:       true
 * #[</ENTRY_FUNC>]# */
void
eDrawingContext_fillRect(CELLIDX idx, TWColor color, const TWRect* rect)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);
	cDesktopLink_fillRect(color, rect);
}

/* #[<ENTRY_FUNC>]# eDrawingContext_drawBitmap
 * name:         eDrawingContext_drawBitmap
 * global_name:  tTWView_eDrawingContext_drawBitmap
 * oneway:       true
 * #[</ENTRY_FUNC>]# */
void
eDrawingContext_drawBitmap(CELLIDX idx, const char* data, TWPixelFormat format, const TWSize* bitmapSize, uint32_t numBytes, const TWRect* inRect, const TWPoint* outLoc, TWColor monoColor)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);
	cDesktopLink_drawBitmap(data, format, bitmapSize, numBytes,
		inRect, outLoc, monoColor);	
}

/* #[<ENTRY_PORT>]# eStyleSource
 * entry port: eStyleSource
 * signature:  sTWValueSourceCallback
 * context:    task
 * #[</ENTRY_PORT>]# */

/* #[<ENTRY_FUNC>]# eStyleSource_changing
 * name:         eStyleSource_changing
 * global_name:  tTWView_eStyleSource_changing
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eStyleSource_changing(CELLIDX idx)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);

	eControl_setNeedsUpdate(idx);
}

/* #[<ENTRY_FUNC>]# eStyleSource_changed
 * name:         eStyleSource_changed
 * global_name:  tTWView_eStyleSource_changed
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eStyleSource_changed(CELLIDX idx)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);

	eControl_setNeedsUpdate(idx);
}

/* #[<ENTRY_PORT>]# eBoundsSource
 * entry port: eBoundsSource
 * signature:  sTWValueSourceCallback
 * context:    task
 * #[</ENTRY_PORT>]# */

/* #[<ENTRY_FUNC>]# eBoundsSource_changing
 * name:         eBoundsSource_changing
 * global_name:  tTWView_eBoundsSource_changing
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eBoundsSource_changing(CELLIDX idx)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);

	eControl_setNeedsUpdate(idx);
}

/* #[<ENTRY_FUNC>]# eBoundsSource_changed
 * name:         eBoundsSource_changed
 * global_name:  tTWView_eBoundsSource_changed
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eBoundsSource_changed(CELLIDX idx)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);

	eControl_setNeedsUpdate(idx);
}

/* #[<POSTAMBLE>]#
 *   Put non-entry functions below.
 * #[</POSTAMBLE>]#*/
