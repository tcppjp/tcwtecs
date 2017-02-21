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
 * call port: cSubview signature: sTWTouchInputSubviewLink context:task optional:true
 *   bool_t     is_cSubview_joined(int subscript)        check if joined
 *   uint8_t        cSubview_findTouchEventHandler( subscript, const TWPoint* location, Descriptor( sTWTouchInputElementLink )* outLink );
 *       subscript:  0...(NCP_cSubview-1)
 * call port: cSuperview signature: sTWTouchInputSuperviewLink context:task
 *   void           cSuperview_translateFromGlobal( TWPoint* location );
 * call port: cTouchEvent signature: sTWTouchEvent context:task optional:true
 *   bool_t     is_cTouchEvent_joined()                     check if joined
 *   void           cTouchEvent_touchStart( uint8_t touchId, TWPoint location );
 *   void           cTouchEvent_touchMove( uint8_t touchId, TWPoint location );
 *   void           cTouchEvent_touchEnd( uint8_t touchId, TWPoint location );
 *   void           cTouchEvent_touchCancel( uint8_t touchId, TWPoint location );
 * call port: cElementLink signature: sTWTouchInputElementLink context:task
 *   void           cElementLink_touchStart( uint8_t touchId, TWPoint location );
 *   void           cElementLink_touchMove( uint8_t touchId, TWPoint location );
 *   void           cElementLink_touchEnd( uint8_t touchId, TWPoint location );
 *   [ref_desc]
 *      Descriptor( sTWTouchInputElementLink ) cElementLink_refer_to_descriptor();
 *      Descriptor( sTWTouchInputElementLink ) cElementLink_ref_desc()      (same as above; abbreviated version);
 * call port: cStyleSource signature: sTWViewStyleSource context:task
 *   TWViewStyle    cStyleSource_get( );
 * call port: cBoundsSource signature: sTWRectSource context:task
 *   void           cBoundsSource_get( TWRect* outRect );
 *
 * #[</PREAMBLE>]# */

/* Put prototype declaration and/or variale definition here #_PAC_# */
#include "tTWTouchInputElementCore_tecsgen.h"

#ifndef E_OK
#define	E_OK	0		/* success */
#define	E_ID	(-18)	/* illegal ID */
#endif

static TWViewStyle
GetViewStyle(CELLCB *p_cellcb) {
	return cStyleSource_get();
}

static void
GetViewBounds(CELLCB *p_cellcb, TWRect *outRect) {
	cBoundsSource_get(outRect);
}

static void
TranslatePointFromGlobal(CELLCB *p_cellcb, TWPoint *location)
{
	cSuperview_translateFromGlobal(location);

	TWRect self_bounds;
	GetViewBounds(p_cellcb, &self_bounds);

	location->x -= self_bounds.x;
	location->y -= self_bounds.y;
}


/* entry port function #_TEPF_# */
/* #[<ENTRY_PORT>]# eSuperview
 * entry port: eSuperview
 * signature:  sTWTouchInputSubviewLink
 * context:    task
 * #[</ENTRY_PORT>]# */

/* #[<ENTRY_FUNC>]# eSuperview_findTouchEventHandler
 * name:         eSuperview_findTouchEventHandler
 * global_name:  tTWTouchInputElementCore_eSuperview_findTouchEventHandler
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
uint8_t
eSuperview_findTouchEventHandler(CELLIDX idx, const TWPoint* location, Descriptor( sTWTouchInputElementLink )* outLink)
{
	CELLCB *p_cellcb = GET_CELLCB(idx);

	if ((GetViewStyle(p_cellcb) & TWViewStyleVisible) == 0) {
		// invisible!
		return 0;
	}

	TWRect self_bounds;
	GetViewBounds(p_cellcb, &self_bounds);

	if (!TWRectContainsPoint(&self_bounds, location)) {
		return 0;
	}

	TWPoint local_loc = *location;
	local_loc.x -= self_bounds.x;
	local_loc.y -= self_bounds.y;

	for (int_t i = 0; i < NCP_cSubview; ++i) {
		if (cSubview_findTouchEventHandler(i, &local_loc, outLink)) {
			return 1;
		}
	}

	*outLink = cElementLink_refer_to_descriptor();
	return 1;
}

/* #[<ENTRY_PORT>]# eSubview
 * entry port: eSubview
 * signature:  sTWTouchInputSuperviewLink
 * context:    task
 * entry port array size:  NEP_eSubview
 * #[</ENTRY_PORT>]# */

/* #[<ENTRY_FUNC>]# eSubview_translateFromGlobal
 * name:         eSubview_translateFromGlobal
 * global_name:  tTWTouchInputElementCore_eSubview_translateFromGlobal
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eSubview_translateFromGlobal(CELLIDX idx, int_t subscript, TWPoint* location)
{
	CELLCB *p_cellcb = GET_CELLCB(idx);

	(void) subscript;

	TranslatePointFromGlobal(p_cellcb, location);
}

/* #[<ENTRY_PORT>]# eElementLink
 * entry port: eElementLink
 * signature:  sTWTouchInputElementLink
 * context:    task
 * #[</ENTRY_PORT>]# */

/* #[<ENTRY_FUNC>]# eElementLink_touchStart
 * name:         eElementLink_touchStart
 * global_name:  tTWTouchInputElementCore_eElementLink_touchStart
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eElementLink_touchStart(CELLIDX idx, uint8_t touchId, TWPoint location)
{
	CELLCB *p_cellcb = GET_CELLCB(idx);

	if (is_cTouchEvent_joined()) {
		TranslatePointFromGlobal(p_cellcb, &location);

		cTouchEvent_touchStart(touchId, location);
	}
}

/* #[<ENTRY_FUNC>]# eElementLink_touchMove
 * name:         eElementLink_touchMove
 * global_name:  tTWTouchInputElementCore_eElementLink_touchMove
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eElementLink_touchMove(CELLIDX idx, uint8_t touchId, TWPoint location)
{
	CELLCB *p_cellcb = GET_CELLCB(idx);

	if (is_cTouchEvent_joined()) {
		TranslatePointFromGlobal(p_cellcb, &location);

		cTouchEvent_touchMove(touchId, location);
	}
}

/* #[<ENTRY_FUNC>]# eElementLink_touchEnd
 * name:         eElementLink_touchEnd
 * global_name:  tTWTouchInputElementCore_eElementLink_touchEnd
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eElementLink_touchEnd(CELLIDX idx, uint8_t touchId, TWPoint location)
{
	CELLCB *p_cellcb = GET_CELLCB(idx);

	if (is_cTouchEvent_joined()) {
		TranslatePointFromGlobal(p_cellcb, &location);

		cTouchEvent_touchEnd(touchId, location);
	}
}

/* #[<POSTAMBLE>]#
 *   Put non-entry functions below.
 * #[</POSTAMBLE>]#*/
