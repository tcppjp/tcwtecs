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
 * currentTimeoutToken     intptr_t        VAR_currentTimeoutToken
 * deferredDispatchPending uint8_t         VAR_deferredDispatchPending
 *
 * call port function #_TCPF_#
 * call port: cTimerManager signature: sTWTimerManagerLink context:task
 *   void           cTimerManager_registerTimer( TWTimerDescriptor* timer );
 *   void           cTimerManager_unregisterTimer( TWTimerDescriptor* timer );
 *   uint8_t        cTimerManager_registerDeferredDispatch( TWDeferredDispatchDescriptor* dd, intptr_t param );
 *   uint8_t        cTimerManager_unregisterDeferredDispatch( TWDeferredDispatchDescriptor* dd );
 *   void           cTimerManager_handleTimeout( );
 *   void           cTimerManager_handleDeferredDispatch( );
 * call port: cDriverControl signature: sTWBasicDispatcherDriverControl context:task optional:true
 *   bool_t     is_cDriverControl_joined()                     check if joined
 *   void           cDriverControl_waitForInterrupt( );
 *   void           cDriverControl_setTimeout( TWDuration duration );
 *   void           cDriverControl_clearTimeout( );
 *   void           cDriverControl_disableInterrupts( );
 *   void           cDriverControl_enableInterrupts( );
 *   TWTimePoint    cDriverControl_getTime( );
 * call port: cTimeoutDispatch signature: sTWDeferredDispatchControl context:task
 *   uint8_t        cTimeoutDispatch_start( intptr_t param );
 *   uint8_t        cTimeoutDispatch_cancel( );
 * call port: cTarget signature: sTWDispatchTarget context:task optional:true
 *   bool_t     is_cTarget_joined()                     check if joined
 *   void           cTarget_main( intptr_t param );
 *   [dynamic, optional]
 *      void           cTarget_set_descriptor( Descriptor( sTWDispatchTarget ) desc );
 *      void           cTarget_unjoin(  );
 *
 * #[</PREAMBLE>]# */

/* Put prototype declaration and/or variale definition here #_PAC_# */
#include "tTWBasicDispatcherCore_tecsgen.h"

#ifndef E_OK
#define	E_OK	0		/* success */
#define	E_ID	(-18)	/* illegal ID */
#endif

/* entry port function #_TEPF_# */
/* #[<ENTRY_PORT>]# eDispatcher
 * entry port: eDispatcher
 * signature:  sTWDispatcherControl
 * context:    task
 * #[</ENTRY_PORT>]# */

/* #[<ENTRY_FUNC>]# eDispatcher_invoke
 * name:         eDispatcher_invoke
 * global_name:  tTWBasicDispatcherCore_eDispatcher_invoke
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eDispatcher_invoke(CELLIDX idx, Descriptor( sTWDispatchTarget ) target, intptr_t param)
{
    CELLCB *p_cellcb = GET_CELLCB(idx);

    cTarget_set_descriptor(target);
    cTarget_main(param);
}

/* #[<ENTRY_FUNC>]# eDispatcher_getTime
 * name:         eDispatcher_getTime
 * global_name:  tTWBasicDispatcherCore_eDispatcher_getTime
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
TWTimePoint
eDispatcher_getTime(CELLIDX idx)
{
    CELLCB *p_cellcb = GET_CELLCB(idx);

    return cDriverControl_getTime();
}

/* #[<ENTRY_PORT>]# eDispatcherLink
 * entry port: eDispatcherLink
 * signature:  sTWDispatcherLink
 * context:    task
 * #[</ENTRY_PORT>]# */

/* #[<ENTRY_FUNC>]# eDispatcherLink_startDeferredDispatch
 * name:         eDispatcherLink_startDeferredDispatch
 * global_name:  tTWBasicDispatcherCore_eDispatcherLink_startDeferredDispatch
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eDispatcherLink_startDeferredDispatch(CELLIDX idx)
{
    CELLCB *p_cellcb = GET_CELLCB(idx);
    VAR_deferredDispatchPending = 1;
}

/* #[<ENTRY_FUNC>]# eDispatcherLink_setTimeout
 * name:         eDispatcherLink_setTimeout
 * global_name:  tTWBasicDispatcherCore_eDispatcherLink_setTimeout
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eDispatcherLink_setTimeout(CELLIDX idx, TWDuration duration)
{
	CELLCB *p_cellcb = GET_CELLCB(idx);

	cDriverControl_clearTimeout();
	cTimeoutDispatch_cancel();
	++VAR_currentTimeoutToken;
	cDriverControl_setTimeout(duration);
}

/* #[<ENTRY_FUNC>]# eDispatcherLink_clearTimeout
 * name:         eDispatcherLink_clearTimeout
 * global_name:  tTWBasicDispatcherCore_eDispatcherLink_clearTimeout
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eDispatcherLink_clearTimeout(CELLIDX idx)
{
	CELLCB *p_cellcb = GET_CELLCB(idx);

	cDriverControl_clearTimeout();
	cTimeoutDispatch_cancel();
	++VAR_currentTimeoutToken;
}

/* #[<ENTRY_FUNC>]# eDispatcherLink_enterCriticalSection
 * name:         eDispatcherLink_enterCriticalSection
 * global_name:  tTWBasicDispatcherCore_eDispatcherLink_enterCriticalSection
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eDispatcherLink_enterCriticalSection(CELLIDX idx)
{
	CELLCB *p_cellcb = GET_CELLCB(idx);

	cDriverControl_disableInterrupts();
}

/* #[<ENTRY_FUNC>]# eDispatcherLink_leaveCriticalSection
 * name:         eDispatcherLink_leaveCriticalSection
 * global_name:  tTWBasicDispatcherCore_eDispatcherLink_leaveCriticalSection
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eDispatcherLink_leaveCriticalSection(CELLIDX idx)
{
	CELLCB *p_cellcb = GET_CELLCB(idx);

	cDriverControl_enableInterrupts();
}

/* #[<ENTRY_FUNC>]# eDispatcherLink_getTime
 * name:         eDispatcherLink_getTime
 * global_name:  tTWBasicDispatcherCore_eDispatcherLink_getTime
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
TWTimePoint
eDispatcherLink_getTime(CELLIDX idx)
{
    CELLCB *p_cellcb = GET_CELLCB(idx);

    return cDriverControl_getTime();
}

/* #[<ENTRY_PORT>]# eBasicDispatcher
 * entry port: eBasicDispatcher
 * signature:  sTWBasicDispatcherControl
 * context:    task
 * #[</ENTRY_PORT>]# */

/* #[<ENTRY_FUNC>]# eBasicDispatcher_enterMainLoop
 * name:         eBasicDispatcher_enterMainLoop
 * global_name:  tTWBasicDispatcherCore_eBasicDispatcher_enterMainLoop
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eBasicDispatcher_enterMainLoop(CELLIDX idx)
{
	CELLCB *p_cellcb = GET_CELLCB(idx);

	volatile uint8_t *pending = (volatile uint8_t *)&VAR_deferredDispatchPending;

	while (1) {
		if (*pending) {
			*pending = 0;
			cTimerManager_handleDeferredDispatch();
		}
		// TODO: wait for interrupt?
	}
}

/* #[<ENTRY_PORT>]# eDriverEvent
 * entry port: eDriverEvent
 * signature:  sTWBasicDispatcherDriverEvent
 * context:    task
 * #[</ENTRY_PORT>]# */

/* #[<ENTRY_FUNC>]# eDriverEvent_handleTimeout
 * name:         eDriverEvent_handleTimeout
 * global_name:  tTWBasicDispatcherCore_eDriverEvent_handleTimeout
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eDriverEvent_handleTimeout(CELLIDX idx)
{
	CELLCB *p_cellcb = GET_CELLCB(idx);

	cTimeoutDispatch_start(VAR_currentTimeoutToken);
}

/* #[<ENTRY_PORT>]# eTimeoutDispatch
 * entry port: eTimeoutDispatch
 * signature:  sTWDispatchTarget
 * context:    task
 * #[</ENTRY_PORT>]# */

/* #[<ENTRY_FUNC>]# eTimeoutDispatch_main
 * name:         eTimeoutDispatch_main
 * global_name:  tTWBasicDispatcherCore_eTimeoutDispatch_main
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eTimeoutDispatch_main(CELLIDX idx, intptr_t param)
{
	CELLCB *p_cellcb = GET_CELLCB(idx);

	if (param == VAR_currentTimeoutToken) {
		cTimerManager_handleTimeout();
	}
}

/* #[<POSTAMBLE>]#
 *   Put non-entry functions below.
 * #[</POSTAMBLE>]#*/
