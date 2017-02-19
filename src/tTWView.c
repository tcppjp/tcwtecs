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
 * call port: cSuperview signature: sTWSuperviewLink context:task optional:true
 *   bool_t     is_cSuperview_joined()                     check if joined
 *   void           cSuperview_setNeedsUpdate( const TWRect* bounds );
 *   void           cSuperview_getGlobalLocation( TWPoint* outLoc );
 * call port: cSubview signature: sTWSubviewLink context:task optional:true
 *   bool_t     is_cSubview_joined(int subscript)        check if joined
 *   void           cSubview_paint( subscript, const TWRect* clipRect, const TWRect* globalBounds );
 *   void           cSubview_subtractClippingRect( subscript, const TWRect* clipRect, const TWRect* globalBounds, uint8_t mode );
 *       subscript:  0...(NCP_cSubview-1)
 * call port: cDesktopLink signature: sTWDesktopLink context:task
 *   void           cDesktopLink_fillRect( TWColor color, const TWRect* rect );
 *   void           cDesktopLink_drawBitmap( const char* data, TWPixelFormat format, const TWSize* bitmapSize, uint32_t numBytes, const TWRect* inRect, const TWPoint* outLoc, TWColor monoColor );
 *   void           cDesktopLink_preparePaint( const TWRect* globalClipRect, const TWPoint* globalLoc );
 *   void           cDesktopLink_subtractClippingRect( const TWRect* globalClipRect );
 * call port: cMouseEvent signature: sTWMouseEvent context:task optional:true
 *   bool_t     is_cMouseEvent_joined()                     check if joined
 *   void           cMouseEvent_mouseDown( TWPoint point, uint8_t button );
 *   void           cMouseEvent_mouseMove( TWPoint point );
 *   void           cMouseEvent_mouseUp( TWPoint point, uint8_t button );
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

	// paint subviews
	for (int_t i = NCP_cSubview - 1; i >= 0; --i) {
		cSubview_subtractClippingRect(i, &new_clip, &self_glob, 1);
		cSubview_paint(i, &new_clip, &self_glob);
	}

	// clip children
	if ((GetViewStyle(p_cellcb) & TWViewStyleNoClipChildren) == 0) {
		for (int_t i = NCP_cSubview - 1; i >= 0; --i) {
			cSubview_subtractClippingRect(i, &new_clip, &self_glob, 0);
		}
	}

	// paint myself
	if (is_cPaintEvent_joined()) {
		TWPoint self_glob_loc = {self_glob.x, self_glob.y};
		cDesktopLink_preparePaint(&new_clip, &self_glob_loc);
		cPaintEvent_paint();
	}
}

/* #[<ENTRY_FUNC>]# eSuperview_subtractClippingRect
 * name:         eSuperview_subtractClippingRect
 * global_name:  tTWView_eSuperview_subtractClippingRect
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eSuperview_subtractClippingRect(CELLIDX idx, const TWRect* clipRect, const TWRect* globalBounds, uint8_t mode)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);

	if (((GetViewStyle(p_cellcb) & TWViewStyleClipSiblings) != 0) == mode) {
		TWRect self_glob;
		GetViewBounds(p_cellcb, &self_glob);
		self_glob.x += globalBounds->x;
		self_glob.y += globalBounds->y;

		TWRect new_clip;
		if (!TWRectIntersect(&self_glob, clipRect, &new_clip)) {
			// culled
			return;
		}

		cDesktopLink_subtractClippingRect(&new_clip);
	}
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
