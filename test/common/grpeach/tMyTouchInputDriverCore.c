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
 * call port: cDriverEvent signature: sTWTouchInputDriverEvent context:task
 *   void           cDriverEvent_touchStart( uint8_t touchId, TWPoint location );
 *   void           cDriverEvent_touchMove( uint8_t touchId, TWPoint location );
 *   void           cDriverEvent_touchEnd( uint8_t touchId, TWPoint location );
 * call port: cTimer signature: sTWTimerControl context:task
 *   void           cTimer_setTimeout( TWDuration duration, intptr_t param );
 *   void           cTimer_setInterval( TWDuration interval, intptr_t param );
 *   void           cTimer_clear( );
 *
 * #[</PREAMBLE>]# */

/* Put prototype declaration and/or variale definition here #_PAC_# */
#include "tMyTouchInputDriverCore_tecsgen.h"

#include "main.h"

#ifndef E_OK
#define	E_OK	0		/* success */
#define	E_ID	(-18)	/* illegal ID */
#endif

void
MD_TouchStart(uint8_t touchId, int16_t x, int16_t y)
{
    cDriverEvent_touchStart(touchId, TWMakePoint(x, y));
}

void
MD_TouchMove(uint8_t touchId, int16_t x, int16_t y)
{
    cDriverEvent_touchMove(touchId, TWMakePoint(x, y));
}

void
MD_TouchEnd(uint8_t touchId, int16_t x, int16_t y)
{
    cDriverEvent_touchEnd(touchId, TWMakePoint(x, y));
}

/* entry port function #_TEPF_# */
/* #[<ENTRY_PORT>]# eInitialize
 * entry port: eInitialize
 * signature:  sTWDispatchTarget
 * context:    task
 * #[</ENTRY_PORT>]# */

/* #[<ENTRY_FUNC>]# eInitialize_main
 * name:         eInitialize_main
 * global_name:  tMyTouchInputDriverCore_eInitialize_main
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eInitialize_main(intptr_t param)
{
    (void) param;
    cTimer_setInterval(10, 0);
}

/* #[<ENTRY_PORT>]# ePoll
 * entry port: ePoll
 * signature:  sTWDispatchTarget
 * context:    task
 * #[</ENTRY_PORT>]# */

/* #[<ENTRY_FUNC>]# ePoll_main
 * name:         ePoll_main
 * global_name:  tMyTouchInputDriverCore_ePoll_main
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
ePoll_main(intptr_t param)
{
    (void) param;
    MD_PollTouchInput();
}

/* #[<POSTAMBLE>]#
 *   Put non-entry functions below.
 * #[</POSTAMBLE>]#*/
