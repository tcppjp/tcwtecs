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
 * timerInterval    TWDuration       ATTR_timerInterval
 * displayedTime    TWDuration       VAR_displayedTime
 * baseTime         TWTimePoint      VAR_baseTime
 * flags            uint8_t          VAR_flags
 *
 * call port function #_TCPF_#
 * call port: cText signature: sTWValueSourceCallback context:task optional:true
 *   bool_t     is_cText_joined(int subscript)        check if joined
 *   void           cText_changing( subscript );
 *   void           cText_changed( subscript );
 *       subscript:  0...(NCP_cText-1)
 * call port: cTimer signature: sTWTimerControl context:task
 *   void           cTimer_setTimeout( TWDuration duration, intptr_t param );
 *   void           cTimer_setInterval( TWDuration interval, intptr_t param );
 *   void           cTimer_clear( );
 * call port: cDispatcher signature: sTWDispatcherControl context:task
 *   void           cDispatcher_invoke( Descriptor( sTWDispatchTarget ) target, intptr_t param );
 *   TWTimePoint    cDispatcher_getTime( );
 *
 * #[</PREAMBLE>]# */

/* Put prototype declaration and/or variale definition here #_PAC_# */
#include "tStopwatchCore_tecsgen.h"

#ifndef E_OK
#define	E_OK	0		/* success */
#define	E_ID	(-18)	/* illegal ID */
#endif

#define kStopwatchFlagsRunning (1 << 0)

/* entry port function #_TEPF_# */
/* #[<ENTRY_PORT>]# eText
 * entry port: eText
 * signature:  sStringValueSource
 * context:    task
 * #[</ENTRY_PORT>]# */

/* #[<ENTRY_FUNC>]# eText_get
 * name:         eText_get
 * global_name:  tStopwatchCore_eText_get
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eText_get(CELLIDX idx, char* outString, uint16_t bufferSize)
{
	CELLCB *p_cellcb = GET_CELLCB(idx);

	uint32_t tenMillsecs = VAR_displayedTime / 10;
	uint32_t minutes = tenMillsecs / 6000;
	tenMillsecs -= minutes * 6000;
	uint32_t hours = minutes / 60;
	minutes -= hours * 60;
	hours %= 100;
	// 00:00:00.00

	if (bufferSize < 12) {
		return;
	}

	outString[0] = '0' + (hours / 10);
	outString[1] = '0' + (hours % 10);
	outString[2] =  ':';
	outString[3] = '0' + (minutes / 10);
	outString[4] = '0' + (minutes % 10);
	outString[5] =  ':';
	outString[6] = '0' + (tenMillsecs / 1000);
	outString[7] = '0' + ((tenMillsecs / 100) % 10);
	outString[8] = '.';
	outString[9] = '0' + ((tenMillsecs / 10) % 10);
	outString[10] = '0' + (tenMillsecs % 10);
	outString[11] = 0;
}

/* #[<ENTRY_PORT>]# eStopwatch
 * entry port: eStopwatch
 * signature:  sStopwatchControl
 * context:    task
 * #[</ENTRY_PORT>]# */

/* #[<ENTRY_FUNC>]# eStopwatch_start
 * name:         eStopwatch_start
 * global_name:  tStopwatchCore_eStopwatch_start
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eStopwatch_start(CELLIDX idx)
{
	CELLCB *p_cellcb = GET_CELLCB(idx);

	if (VAR_flags & kStopwatchFlagsRunning) {
		return;
	}

	VAR_baseTime = cDispatcher_getTime() - VAR_displayedTime;
	VAR_flags |= kStopwatchFlagsRunning;

	cTimer_setInterval(ATTR_timerInterval, 0);
}

/* #[<ENTRY_FUNC>]# eStopwatch_stop
 * name:         eStopwatch_stop
 * global_name:  tStopwatchCore_eStopwatch_stop
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eStopwatch_stop(CELLIDX idx)
{
	CELLCB *p_cellcb = GET_CELLCB(idx);

	if ((VAR_flags & kStopwatchFlagsRunning) == 0) {
		return;
	}

	VAR_flags &= ~kStopwatchFlagsRunning;

	cTimer_clear();
}

/* #[<ENTRY_FUNC>]# eStopwatch_reset
 * name:         eStopwatch_reset
 * global_name:  tStopwatchCore_eStopwatch_reset
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eStopwatch_reset(CELLIDX idx)
{
	CELLCB *p_cellcb = GET_CELLCB(idx);


	if (VAR_flags & kStopwatchFlagsRunning) {
		VAR_flags &= ~kStopwatchFlagsRunning;
		cTimer_clear();
	}

	for (int_t i = 0; i < NCP_cText; ++i) {
		cText_changing(i);
	}
	VAR_displayedTime = 0;
	for (int_t i = 0; i < NCP_cText; ++i) {
		cText_changed(i);
	}
}

/* #[<ENTRY_FUNC>]# eStopwatch_isRunning
 * name:         eStopwatch_isRunning
 * global_name:  tStopwatchCore_eStopwatch_isRunning
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
bool
eStopwatch_isRunning(CELLIDX idx)
{
	CELLCB *p_cellcb = GET_CELLCB(idx);
	return (VAR_flags & kStopwatchFlagsRunning) != 0;
}

/* #[<ENTRY_PORT>]# eTimerTick
 * entry port: eTimerTick
 * signature:  sTWDispatchTarget
 * context:    task
 * #[</ENTRY_PORT>]# */

/* #[<ENTRY_FUNC>]# eTimerTick_main
 * name:         eTimerTick_main
 * global_name:  tStopwatchCore_eTimerTick_main
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eTimerTick_main(CELLIDX idx, intptr_t param)
{
	CELLCB *p_cellcb = GET_CELLCB(idx);
	(void) param;

	for (int_t i = 0; i < NCP_cText; ++i) {
		cText_changing(i);
	}
	VAR_displayedTime = cDispatcher_getTime() - VAR_baseTime;
	for (int_t i = 0; i < NCP_cText; ++i) {
		cText_changed(i);
	}
}

/* #[<POSTAMBLE>]#
 *   Put non-entry functions below.
 * #[</POSTAMBLE>]#*/
