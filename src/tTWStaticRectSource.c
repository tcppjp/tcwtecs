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
 * attr access macro #_CAAM_#
 * x                int16_t          ATTR_x          
 * y                int16_t          ATTR_y          
 * w                int16_t          ATTR_w          
 * h                int16_t          ATTR_h          
 *
 * #[</PREAMBLE>]# */

/* Put prototype declaration and/or variale definition here #_PAC_# */
#include "tTWStaticRectSource_tecsgen.h"

#ifndef E_OK
#define	E_OK	0		/* success */
#define	E_ID	(-18)	/* illegal ID */
#endif

/* entry port function #_TEPF_# */
/* #[<ENTRY_PORT>]# eSource
 * entry port: eSource
 * signature:  sTWRectSource
 * context:    task
 * #[</ENTRY_PORT>]# */

/* #[<ENTRY_FUNC>]# eSource_get
 * name:         eSource_get
 * global_name:  tTWStaticRectSource_eSource_get
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eSource_get(CELLIDX idx, TWRect* outRect)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);
	outRect->x = ATTR_x;
	outRect->y = ATTR_y;
	outRect->w = ATTR_w;
	outRect->h = ATTR_h;
}

/* #[<POSTAMBLE>]#
 *   Put non-entry functions below.
 * #[</POSTAMBLE>]#*/
