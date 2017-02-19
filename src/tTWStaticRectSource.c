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
