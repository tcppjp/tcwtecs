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
 * maxNumTouches    uint8_t          ATTR_maxNumTouches
 * localTouchId     uint8_t*         VAR_localTouchId
 *
 * call port function #_TCPF_#
 * call port: cSubview signature: sTWTouchInputSubviewLink context:task
 *   uint8_t        cSubview_findTouchEventHandler( const TWPoint* location, Descriptor( sTWTouchInputElementLink )* outLink );
 * call port: cElement signature: sTWTouchInputElementLink context:task
 *   void           cElement_touchStart( subscript, uint8_t touchId, TWPoint location );
 *   void           cElement_touchMove( subscript, uint8_t touchId, TWPoint location );
 *   void           cElement_touchEnd( subscript, uint8_t touchId, TWPoint location );
 *       subscript:  0...(NCP_cElement-1)
 *   [ref_desc]
 *      Descriptor( sTWTouchInputElementLink ) cElement_refer_to_descriptor( int_t subscript );
 *      Descriptor( sTWTouchInputElementLink ) cElement_ref_desc( int_t subscript )      (same as above; abbreviated version);
 *   [dynamic]
 *      void           cElement_set_descriptor( int_t subscript, Descriptor( sTWTouchInputElementLink ) desc );
 * call port: cNullElementLink signature: sTWTouchInputElementLink context:task
 *   void           cNullElementLink_touchStart( uint8_t touchId, TWPoint location );
 *   void           cNullElementLink_touchMove( uint8_t touchId, TWPoint location );
 *   void           cNullElementLink_touchEnd( uint8_t touchId, TWPoint location );
 *   [ref_desc]
 *      Descriptor( sTWTouchInputElementLink ) cNullElementLink_refer_to_descriptor();
 *      Descriptor( sTWTouchInputElementLink ) cNullElementLink_ref_desc()      (same as above; abbreviated version);
 *
 * #[</PREAMBLE>]# */

/* Put prototype declaration and/or variale definition here #_PAC_# */
#include <assert.h>
#include "tTWTouchInputManagerCore_tecsgen.h"
#include "tecsui/private.h"

#ifndef E_OK
#define	E_OK	0		/* success */
#define	E_ID	(-18)	/* illegal ID */
#endif

static bool
IsTouchActive(CELLCB *p_cellcb, uint8_t touchId)
{
	return !TWTecsDescriptorEquals(sTWTouchInputElementLink,
		cElement_refer_to_descriptor(touchId), cNullElementLink_refer_to_descriptor());
}

static int16_t
FindTouchId(CELLCB *p_cellcb, Descriptor(sTWTouchInputElementLink) link, uint8_t localTouchId)
{
	for (int_t i = 0; i < NCP_cElement; ++i) {
		if (TWTecsDescriptorEquals(sTWTouchInputElementLink, cElement_refer_to_descriptor(i), link) &&
			VAR_localTouchId[i] == localTouchId) {
			return (int16_t)i;
		}
	}
	return -1;
}

/* entry port function #_TEPF_# */
/* #[<ENTRY_PORT>]# eSubview
 * entry port: eSubview
 * signature:  sTWTouchInputSuperviewLink
 * context:    task
 * #[</ENTRY_PORT>]# */

/* #[<ENTRY_FUNC>]# eSubview_translateFromGlobal
 * name:         eSubview_translateFromGlobal
 * global_name:  tTWTouchInputManagerCore_eSubview_translateFromGlobal
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eSubview_translateFromGlobal(CELLIDX idx, TWPoint* location)
{
	(void) idx; (void) location;
}

/* #[<ENTRY_PORT>]# eDriverEvent
 * entry port: eDriverEvent
 * signature:  sTWTouchInputDriverEvent
 * context:    task
 * #[</ENTRY_PORT>]# */

/* #[<ENTRY_FUNC>]# eDriverEvent_touchStart
 * name:         eDriverEvent_touchStart
 * global_name:  tTWTouchInputManagerCore_eDriverEvent_touchStart
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eDriverEvent_touchStart(CELLIDX idx, uint8_t touchId, TWPoint location)
{
	CELLCB *p_cellcb = GET_CELLCB(idx);

	if (touchId >= ATTR_maxNumTouches) {
		return;
	}

	assert (!IsTouchActive(p_cellcb, touchId));

	// Find the receiver
	Descriptor(sTWTouchInputElementLink) link = cNullElementLink_refer_to_descriptor();
	cSubview_findTouchEventHandler(&location, &link);

	// Allocate a local touch identifier
	uint8_t localTouchId = 0;
	while (FindTouchId(p_cellcb, link, localTouchId) != -1) {
		++localTouchId;
	}

	// Set the receiver
	cElement_set_descriptor(touchId, link);
	VAR_localTouchId[touchId] = localTouchId;

	// Forward the event
	cElement_touchStart(touchId, localTouchId, location);
}

/* #[<ENTRY_FUNC>]# eDriverEvent_touchMove
 * name:         eDriverEvent_touchMove
 * global_name:  tTWTouchInputManagerCore_eDriverEvent_touchMove
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eDriverEvent_touchMove(CELLIDX idx, uint8_t touchId, TWPoint location)
{
	CELLCB *p_cellcb = GET_CELLCB(idx);

	if (touchId >= ATTR_maxNumTouches) {
		return;
	}

	if (!IsTouchActive(p_cellcb, touchId)) {
		return;
	}

	cElement_touchMove(touchId, VAR_localTouchId[touchId], location);
}

/* #[<ENTRY_FUNC>]# eDriverEvent_touchEnd
 * name:         eDriverEvent_touchEnd
 * global_name:  tTWTouchInputManagerCore_eDriverEvent_touchEnd
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eDriverEvent_touchEnd(CELLIDX idx, uint8_t touchId, TWPoint location)
{
	CELLCB *p_cellcb = GET_CELLCB(idx);

	if (touchId >= ATTR_maxNumTouches) {
		return;
	}

	if (!IsTouchActive(p_cellcb, touchId)) {
		return;
	}

	cElement_touchEnd(touchId, VAR_localTouchId[touchId], location);

	cElement_set_descriptor(touchId, cNullElementLink_refer_to_descriptor());
}

/* #[<ENTRY_PORT>]# eNullElementLink
 * entry port: eNullElementLink
 * signature:  sTWTouchInputElementLink
 * context:    task
 * #[</ENTRY_PORT>]# */

/* #[<ENTRY_FUNC>]# eNullElementLink_touchStart
 * name:         eNullElementLink_touchStart
 * global_name:  tTWTouchInputManagerCore_eNullElementLink_touchStart
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eNullElementLink_touchStart(CELLIDX idx, uint8_t touchId, TWPoint location)
{
	(void) idx, (void) touchId, (void) location;
}

/* #[<ENTRY_FUNC>]# eNullElementLink_touchMove
 * name:         eNullElementLink_touchMove
 * global_name:  tTWTouchInputManagerCore_eNullElementLink_touchMove
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eNullElementLink_touchMove(CELLIDX idx, uint8_t touchId, TWPoint location)
{
	(void) idx, (void) touchId, (void) location;
}

/* #[<ENTRY_FUNC>]# eNullElementLink_touchEnd
 * name:         eNullElementLink_touchEnd
 * global_name:  tTWTouchInputManagerCore_eNullElementLink_touchEnd
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eNullElementLink_touchEnd(CELLIDX idx, uint8_t touchId, TWPoint location)
{
	(void) idx, (void) touchId, (void) location;
}

/* #[<POSTAMBLE>]#
 *   Put non-entry functions below.
 * #[</POSTAMBLE>]#*/
