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
 * call port: cSDL signature: sTWSDLGraphicsDeviceControl context:task
 *   void           cSDL_initialize( );
 *   void           cSDL_enterMainLoop( );
 *   void           cSDL_quit( );
 * call port: cDesktop signature: sTWDesktopControl context:task
 *   void           cDesktop_paint( );
 *   void           cDesktop_repaintAll( );
 *
 * #[</PREAMBLE>]# */

/* Put prototype declaration and/or variale definition here #_PAC_# */
#include "tTestApp_tecsgen.h"

#ifndef E_OK
#define	E_OK	0		/* success */
#define	E_ID	(-18)	/* illegal ID */
#endif

/* entry port function #_TEPF_# */
/* #[<ENTRY_PORT>]# eSDL
 * entry port: eSDL
 * signature:  sTWSDLGraphicsDeviceEvent
 * context:    task
 * #[</ENTRY_PORT>]# */

/* #[<ENTRY_FUNC>]# eSDL_idle
 * name:         eSDL_idle
 * global_name:  tTestApp_eSDL_idle
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eSDL_idle()
{
	cDesktop_paint();
}

/* #[<POSTAMBLE>]#
 *   Put non-entry functions below.
 * #[</POSTAMBLE>]#*/

int main(int argc, char **argv) {
	cSDL_initialize();
	cDesktop_repaintAll();
	cSDL_enterMainLoop();
}
