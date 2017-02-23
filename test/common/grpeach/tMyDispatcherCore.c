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
 * call port: cTimerManager signature: sTWTimerManagerLink context:task
 *   void           cTimerManager_registerTimer( TWTimerDescriptor* timer );
 *   void           cTimerManager_unregisterTimer( TWTimerDescriptor* timer );
 *   uint8_t        cTimerManager_registerDeferredDispatch( TWDeferredDispatchDescriptor* dd, intptr_t param );
 *   uint8_t        cTimerManager_unregisterDeferredDispatch( TWDeferredDispatchDescriptor* dd );
 *   void           cTimerManager_handleTimeout( );
 *   void           cTimerManager_handleDeferredDispatch( );
 * call port: cInitializer signature: sTWDispatchTarget context:task optional:true
 *   bool_t     is_cInitializer_joined(int subscript)        check if joined
 *   void           cInitializer_main( subscript, intptr_t param );
 *       subscript:  0...(NCP_cInitializer-1)
 * call port: cTarget signature: sTWDispatchTarget context:task optional:true
 *   bool_t     is_cTarget_joined()                     check if joined
 *   void           cTarget_main( intptr_t param );
 *   [dynamic, optional]
 *      void           cTarget_set_descriptor( Descriptor( sTWDispatchTarget ) desc );
 *      void           cTarget_unjoin(  );
 * call port: cTimerDispatch signature: sTWDeferredDispatchControl context:task
 *   uint8_t        cTimerDispatch_start( intptr_t param );
 *   uint8_t        cTimerDispatch_cancel( );
 *
 * #[</PREAMBLE>]# */

/* Put prototype declaration and/or variale definition here #_PAC_# */
#include <stdio.h>
#include "tMyDispatcherCore_tecsgen.h"

#include "main.h"

#ifndef E_OK
#define	E_OK	0		/* success */
#define	E_ID	(-18)	/* illegal ID */
#endif

static uintptr_t g_currentTimeoutToken = 1;

typedef struct tagInvokeParam {
    Descriptor( sTWDispatchTarget ) target;
    intptr_t param;
} InvokeParam;

void
MD_Initialize(void)
{
    for (int_t i = 0; i < NCP_cInitializer; ++i) {
        cInitializer_main(i, 0);
    }
}

void
MD_HandleDeferredDispatch(void)
{
    cTimerManager_handleDeferredDispatch();
}

void
MD_HandleInvoke(void *param)
{
    const InvokeParam *ip = (const InvokeParam *)param;
    cTarget_set_descriptor(ip->target);
    cTarget_main(ip->param);
}

void
MD_HandleTimeout(uintptr_t token)
{
    // RtosTimer's handler is called in another thread; we cannot call
    // `handleTimeout` directly from here
    cTimerDispatch_start((intptr_t) token);
}

/* entry port function #_TEPF_# */
/* #[<ENTRY_PORT>]# eDispatcher
 * entry port: eDispatcher
 * signature:  sTWDispatcherControl
 * context:    task
 * #[</ENTRY_PORT>]# */

/* #[<ENTRY_FUNC>]# eDispatcher_invoke
 * name:         eDispatcher_invoke
 * global_name:  tMyDispatcherCore_eDispatcher_invoke
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eDispatcher_invoke(Descriptor( sTWDispatchTarget ) target, intptr_t param)
{
    InvokeParam ip = {target, param};
    MD_Invoke(&ip);
}

/* #[<ENTRY_FUNC>]# eDispatcher_getTime
 * name:         eDispatcher_getTime
 * global_name:  tMyDispatcherCore_eDispatcher_getTime
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
TWTimePoint
eDispatcher_getTime()
{
    return MD_GetTime();
}

/* #[<ENTRY_PORT>]# eDispatcherLink
 * entry port: eDispatcherLink
 * signature:  sTWDispatcherLink
 * context:    task
 * #[</ENTRY_PORT>]# */

/* #[<ENTRY_FUNC>]# eDispatcherLink_startDeferredDispatch
 * name:         eDispatcherLink_startDeferredDispatch
 * global_name:  tMyDispatcherCore_eDispatcherLink_startDeferredDispatch
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eDispatcherLink_startDeferredDispatch()
{
    MD_StartDeferredDispatch();
}

/* #[<ENTRY_FUNC>]# eDispatcherLink_setTimeout
 * name:         eDispatcherLink_setTimeout
 * global_name:  tMyDispatcherCore_eDispatcherLink_setTimeout
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eDispatcherLink_setTimeout(TWDuration duration)
{
    ++g_currentTimeoutToken;
    if (g_currentTimeoutToken == 0) {
        // Zero is an invalid value
        g_currentTimeoutToken = 1;
    }
    MD_SetTimeout(duration, g_currentTimeoutToken);
}

/* #[<ENTRY_FUNC>]# eDispatcherLink_clearTimeout
 * name:         eDispatcherLink_clearTimeout
 * global_name:  tMyDispatcherCore_eDispatcherLink_clearTimeout
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eDispatcherLink_clearTimeout()
{
    MD_ClearTimeout();
}

/* #[<ENTRY_FUNC>]# eDispatcherLink_enterCriticalSection
 * name:         eDispatcherLink_enterCriticalSection
 * global_name:  tMyDispatcherCore_eDispatcherLink_enterCriticalSection
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eDispatcherLink_enterCriticalSection()
{
    MD_EnterCriticalSection();
}

/* #[<ENTRY_FUNC>]# eDispatcherLink_leaveCriticalSection
 * name:         eDispatcherLink_leaveCriticalSection
 * global_name:  tMyDispatcherCore_eDispatcherLink_leaveCriticalSection
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eDispatcherLink_leaveCriticalSection()
{
    MD_LeaveCriticalSection();
}

/* #[<ENTRY_FUNC>]# eDispatcherLink_getTime
 * name:         eDispatcherLink_getTime
 * global_name:  tMyDispatcherCore_eDispatcherLink_getTime
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
TWTimePoint
eDispatcherLink_getTime()
{
    return MD_GetTime();
}

/* #[<ENTRY_PORT>]# eTimerDispatch
 * entry port: eTimerDispatch
 * signature:  sTWDispatchTarget
 * context:    task
 * #[</ENTRY_PORT>]# */

/* #[<ENTRY_FUNC>]# eTimerDispatch_main
 * name:         eTimerDispatch_main
 * global_name:  tMyDispatcherCore_eTimerDispatch_main
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eTimerDispatch_main(intptr_t param)
{
    if ((uintptr_t)param != g_currentTimeoutToken) {
        return;
    }
    cTimerManager_handleTimeout();
}

/* #[<POSTAMBLE>]#
 *   Put non-entry functions below.
 * #[</POSTAMBLE>]#*/
