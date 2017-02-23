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
#include "tSDLApp_tecsgen.h"

#ifndef E_OK
#define	E_OK	0		/* success */
#define	E_ID	(-18)	/* illegal ID */
#endif

/* #[<POSTAMBLE>]#
 *   Put non-entry functions below.
 * #[</POSTAMBLE>]#*/

int main(int argc, char **argv) {
    SDL_Init(SDL_INIT_TIMER | SDL_INIT_EVENTS | SDL_INIT_VIDEO);
    cSDLDispatcher_initialize();
    cSDL_initialize();
    cDesktop_repaintAll();
    cSDLDispatcher_enterMainLoop();
}
