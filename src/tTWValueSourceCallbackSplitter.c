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
 * call port: cCallback signature: sTWValueSourceCallback context:task optional:true
 *   bool_t     is_cCallback_joined(int subscript)        check if joined
 *   void           cCallback_changing( subscript );
 *   void           cCallback_changed( subscript );
 *       subscript:  0...(NCP_cCallback-1)
 *
 * #[</PREAMBLE>]# */

/* Put prototype declaration and/or variale definition here #_PAC_# */
#include "tTWValueSourceCallbackSplitter_tecsgen.h"

#ifndef E_OK
#define	E_OK	0		/* success */
#define	E_ID	(-18)	/* illegal ID */
#endif

/* entry port function #_TEPF_# */
/* #[<ENTRY_PORT>]# eCallback
 * entry port: eCallback
 * signature:  sTWValueSourceCallback
 * context:    task
 * #[</ENTRY_PORT>]# */

/* #[<ENTRY_FUNC>]# eCallback_changing
 * name:         eCallback_changing
 * global_name:  tTWValueSourceCallbackSplitter_eCallback_changing
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eCallback_changing(CELLIDX idx)
{
	CELLCB *p_cellcb = GET_CELLCB(idx);

	for (int_t i = 0; i < NCP_cCallback; ++i) {
		cCallback_changing(i);
	}
}

/* #[<ENTRY_FUNC>]# eCallback_changed
 * name:         eCallback_changed
 * global_name:  tTWValueSourceCallbackSplitter_eCallback_changed
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eCallback_changed(CELLIDX idx)
{
	CELLCB *p_cellcb = GET_CELLCB(idx);

	for (int_t i = 0; i < NCP_cCallback; ++i) {
		cCallback_changed(i);
	}
}

/* #[<POSTAMBLE>]#
 *   Put non-entry functions below.
 * #[</POSTAMBLE>]#*/
