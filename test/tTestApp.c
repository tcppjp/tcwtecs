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
 * call port: cSDLDispatcher signature: sTWSDLDispatcherControl context:task
 *   void           cSDLDispatcher_initialize( );
 *   void           cSDLDispatcher_enterMainLoop( );
 *   void           cSDLDispatcher_quit( );
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
/* #[<POSTAMBLE>]#
 *   Put non-entry functions below.
 * #[</POSTAMBLE>]#*/

int main(int argc, char **argv) {
    cSDLDispatcher_initialize();
	cSDL_initialize();
	cDesktop_repaintAll();
	cSDLDispatcher_enterMainLoop();
}
