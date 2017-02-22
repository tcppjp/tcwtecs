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
 * dirtyRect        TWRect           VAR_dirtyRect
 * paintOffset      TWPoint          VAR_paintOffset
 *
 * call port function #_TCPF_#
 * call port: cSubview signature: sTWSubviewLink context:task optional:true
 *   bool_t     is_cSubview_joined()                     check if joined
 *   void           cSubview_paint( const TWRect* clipRect, const TWRect* globalBounds );
 *   void           cSubview_subtractClippingRect( const TWRect* clipRect, const TWRect* globalBounds, uint8_t mode );
 * call port: cGraphicsDevice signature: sTWGraphicsDeviceOutput context:task
 *   void           cGraphicsDevice_getScreenSize( TWSize* outSize );
 *   void           cGraphicsDevice_setClippingRect( const TWRect* rect );
 *   void           cGraphicsDevice_subtractClippingRect( const TWRect* rect );
 *   void           cGraphicsDevice_setScissorRect( const TWRect* rect );
 *   void           cGraphicsDevice_fillRect( TWColor color, const TWRect* rect );
 *   void           cGraphicsDevice_drawBitmap( const char* data, uint32_t numBytes, const TWBitmapInfo* info, uint16_t infoSize, const TWRect* inRect, const TWPoint* outLoc );
 *   void           cGraphicsDevice_update( const TWRect* rect );
 * call port: cRepaintDeferredDispatch signature: sTWDeferredDispatchControl context:task
 *   uint8_t        cRepaintDeferredDispatch_start( intptr_t param );
 *   uint8_t        cRepaintDeferredDispatch_cancel( );
 *
 * #[</PREAMBLE>]# */

/* Put prototype declaration and/or variale definition here #_PAC_# */
#include "tTWDesktopCore_tecsgen.h"
#include "tecsui/types.h"
#include "tecsui/geometry.h"

static void
PaintDirtyRegion(CELLCB *p_cellcb)
{
	TWRect dirty_rect = VAR_dirtyRect;
	if (dirty_rect.w <= 0 || dirty_rect.h <= 0) {
		// no region to update
		return;
	}

	// forget the current dirty rect
	VAR_dirtyRect.w = 0;
	VAR_dirtyRect.h = 0;

	TWSize desktop_size;
	cGraphicsDevice_getScreenSize(&desktop_size);
	TWRect desktop_rect = {0, 0, desktop_size.w, desktop_size.h};

	cGraphicsDevice_setClippingRect(&dirty_rect);

	cSubview_paint(&dirty_rect, &desktop_rect);

	cGraphicsDevice_update(&dirty_rect);
}

static void
ScheduleRepaint(CELLCB *p_cellcb)
{
	cRepaintDeferredDispatch_start(0);
}

/* entry port function #_TEPF_# */
/* #[<ENTRY_PORT>]# eDesktop
 * entry port: eDesktop
 * signature:  sTWDesktopControl
 * context:    task
 * #[</ENTRY_PORT>]# */

/* #[<ENTRY_FUNC>]# eDesktop_paint
 * name:         eDesktop_paint
 * global_name:  tTWDesktopCore_eDesktop_paint
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eDesktop_paint(CELLIDX idx)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);

	PaintDirtyRegion(p_cellcb);
}

/* #[<ENTRY_FUNC>]# eDesktop_repaintAll
 * name:         eDesktop_repaintAll
 * global_name:  tTWDesktopCore_eDesktop_repaintAll
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eDesktop_repaintAll(CELLIDX idx)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);

	TWSize desktop_size;
	cGraphicsDevice_getScreenSize(&desktop_size);

	// forget the current dirty rect
	VAR_dirtyRect.w = 0;
	VAR_dirtyRect.h = 0;

	TWRect dr = {0, 0, desktop_size.w, desktop_size.h};

	cGraphicsDevice_setClippingRect(&dr);

	cSubview_paint(&dr, &dr);

	cGraphicsDevice_update(&dr);
}

/* #[<ENTRY_PORT>]# eSubview
 * entry port: eSubview
 * signature:  sTWSuperviewLink
 * context:    task
 * #[</ENTRY_PORT>]# */

/* #[<ENTRY_FUNC>]# eSubview_setNeedsUpdate
 * name:         eSubview_setNeedsUpdate
 * global_name:  tTWDesktopCore_eSubview_setNeedsUpdate
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eSubview_setNeedsUpdate(CELLIDX idx, const TWRect* bounds)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);

	TWRect *dr = &VAR_dirtyRect;

	// clip to desktop
	TWSize desktop_size;
	cGraphicsDevice_getScreenSize(&desktop_size);

	TWRect desktop = {0, 0, desktop_size.w, desktop_size.h};
	TWRect clipped_bounds;

	// clip by desktop bounds
	if (!TWRectIntersect(bounds, &desktop, &clipped_bounds)) {
		// outside the screen
		return;
	}

	if (dr->w <= 0 || dr->h <= 0) {
		*dr = clipped_bounds;
	} else {
		TWRectUnion(dr, &clipped_bounds, dr);
	}

	ScheduleRepaint(p_cellcb);
}

/* #[<ENTRY_FUNC>]# eSubview_getGlobalLocation
 * name:         eSubview_getGlobalLocation
 * global_name:  tTWDesktopCore_eSubview_getGlobalLocation
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eSubview_getGlobalLocation(CELLIDX idx, TWPoint* outLoc)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);
	(void) p_cellcb;

	outLoc->x = 0;
	outLoc->y = 0;
}

/* #[<ENTRY_PORT>]# eDesktopLink
 * entry port: eDesktopLink
 * signature:  sTWDesktopLink
 * context:    task
 * #[</ENTRY_PORT>]# */

/* #[<ENTRY_FUNC>]# eDesktopLink_fillRect
 * name:         eDesktopLink_fillRect
 * global_name:  tTWDesktopCore_eDesktopLink_fillRect
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eDesktopLink_fillRect(CELLIDX idx, TWColor color, const TWRect* rect)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);
	TWRect globRect = *rect;
	globRect.x += VAR_paintOffset.x;
	globRect.y += VAR_paintOffset.y;
	cGraphicsDevice_fillRect(color, &globRect);
}

/* #[<ENTRY_FUNC>]# eDesktopLink_drawBitmap
 * name:         eDesktopLink_drawBitmap
 * global_name:  tTWDesktopCore_eDesktopLink_drawBitmap
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eDesktopLink_drawBitmap(CELLIDX idx, const char* data, uint32_t numBytes, const TWBitmapInfo* info, uint16_t infoSize, const TWRect* inRect, const TWPoint* outLoc)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);
	TWPoint globLoc = *outLoc;
	globLoc.x += VAR_paintOffset.x;
	globLoc.y += VAR_paintOffset.y;
	cGraphicsDevice_drawBitmap(data, numBytes, info, infoSize,
		inRect, &globLoc);
}

/* #[<ENTRY_FUNC>]# eDesktopLink_preparePaint
 * name:         eDesktopLink_preparePaint
 * global_name:  tTWDesktopCore_eDesktopLink_preparePaint
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eDesktopLink_preparePaint(CELLIDX idx, const TWRect* globalClipRect, const TWPoint* globalLoc)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);

	VAR_paintOffset = *globalLoc;
	cGraphicsDevice_setScissorRect(globalClipRect);
}

/* #[<ENTRY_FUNC>]# eDesktopLink_subtractClippingRect
 * name:         eDesktopLink_subtractClippingRect
 * global_name:  tTWDesktopCore_eDesktopLink_subtractClippingRect
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eDesktopLink_subtractClippingRect(CELLIDX idx, const TWRect* globalClipRect)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);

	cGraphicsDevice_subtractClippingRect(globalClipRect);
}

/* #[<ENTRY_PORT>]# eGraphicsDevice
 * entry port: eGraphicsDevice
 * signature:  sTWGraphicsDeviceInput
 * context:    task
 * #[</ENTRY_PORT>]# */

/* #[<ENTRY_FUNC>]# eGraphicsDevice_resize
 * name:         eGraphicsDevice_resize
 * global_name:  tTWDesktopCore_eGraphicsDevice_resize
 * oneway:       true
 * #[</ENTRY_FUNC>]# */
void
eGraphicsDevice_resize(CELLIDX idx)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);

	// mark entire the screen as dirty
	TWSize desktop_size;
	cGraphicsDevice_getScreenSize(&desktop_size);

	VAR_dirtyRect.x = 0;
	VAR_dirtyRect.y = 0;
	VAR_dirtyRect.w = desktop_size.w;
	VAR_dirtyRect.h = desktop_size.h;

	ScheduleRepaint(p_cellcb);
}

/* #[<ENTRY_PORT>]# eRepaintDeferredDispatch
 * entry port: eRepaintDeferredDispatch
 * signature:  sTWDispatchTarget
 * context:    task
 * #[</ENTRY_PORT>]# */

/* #[<ENTRY_FUNC>]# eRepaintDeferredDispatch_main
 * name:         eRepaintDeferredDispatch_main
 * global_name:  tTWDesktopCore_eRepaintDeferredDispatch_main
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eRepaintDeferredDispatch_main(CELLIDX idx, intptr_t param)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);

	PaintDirtyRegion(p_cellcb);
}

/* #[<POSTAMBLE>]#
 *   Put non-entry functions below.
 * #[</POSTAMBLE>]#*/
