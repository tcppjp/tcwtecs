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
 * data             const char*      ATTR_data
 * dataSize         uint32_t         ATTR_dataSize
 * infoHeader       const TWBitmapInfoHeader*  ATTR_infoHeader
 *
 * #[</PREAMBLE>]# */

/* Put prototype declaration and/or variale definition here #_PAC_# */
#include "tBitmapSource_tecsgen.h"

#ifndef E_OK
#define	E_OK	0		/* success */
#define	E_ID	(-18)	/* illegal ID */
#endif

/* entry port function #_TEPF_# */
/* #[<ENTRY_PORT>]# eImageData
 * entry port: eImageData
 * signature:  sBitmapData
 * context:    task
 * #[</ENTRY_PORT>]# */

/* #[<ENTRY_FUNC>]# eImageData_get
 * name:         eImageData_get
 * global_name:  tBitmapSource_eImageData_get
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eBitmapData_get(CELLIDX idx, const char** data, uint32_t* numBytes, TWBitmapInfoHeader* infoHeader)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);
	*data = ATTR_data;
    *numBytes = ATTR_dataSize;
    *infoHeader = *ATTR_infoHeader;
}

/* #[<POSTAMBLE>]#
 *   Put non-entry functions below.
 * #[</POSTAMBLE>]#*/
