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
 * value            TWViewStyle      ATTR_value      
 *
 * #[</PREAMBLE>]# */

/* Put prototype declaration and/or variale definition here #_PAC_# */
#include "tTWStaticViewStyleSource_tecsgen.h"

#ifndef E_OK
#define	E_OK	0		/* success */
#define	E_ID	(-18)	/* illegal ID */
#endif

/* entry port function #_TEPF_# */
/* #[<ENTRY_PORT>]# eSource
 * entry port: eSource
 * signature:  sTWViewStyleSource
 * context:    task
 * #[</ENTRY_PORT>]# */

/* #[<ENTRY_FUNC>]# eSource_get
 * name:         eSource_get
 * global_name:  tTWStaticViewStyleSource_eSource_get
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
TWViewStyle
eSource_get(CELLIDX idx)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);
	return ATTR_value;
}

/* #[<POSTAMBLE>]#
 *   Put non-entry functions below.
 * #[</POSTAMBLE>]#*/
