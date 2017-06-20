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
#include <stdatomic.h>

#include "tMyDispatcherCore_tecsgen.h"

#include "tMyDispatcherCore.h"

#ifndef E_OK
#define	E_OK	0		/* success */
#define	E_ID	(-18)	/* illegal ID */
#endif

TIM_HandleTypeDef g_timeoutTimer;
static TIM_HandleTypeDef g_stableClock;

static uint8_t g_hasPendingDeferredDispatch = 0;

static uint8_t MD_CheckPendingDeferredDispatch(void);

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == &g_timeoutTimer) {
        HAL_TIM_Base_Stop_IT(&g_timeoutTimer);

        // We're currently in the Handler mode; we cannot call `handleTimeout`
        // directly from here
        cTimerDispatch_start(0);
    } else {
        // unreachable
        assert(false);
    }
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
    // This works okay since we have no operating systems hence
    // no other threads
    cTarget_set_descriptor(target);
    cTarget_main(param);
}

/* #[<ENTRY_FUNC>]# eDispatcher_getTime
 * name:         eDispatcher_getTime
 * global_name:  tMyDispatcherCore_eDispatcher_getTime
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
TWTimePoint
eDispatcher_getTime()
{
    return __HAL_TIM_GET_COUNTER(&g_stableClock);
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
    // If this was called from the GUI thread, the side effect
    // of this assignment should be visible at the main loop per
    // the C evaluation rules.
    // If this was called from an interrupt handler, it's still
    // safe since this is never read when interrupts are enabled
    g_hasPendingDeferredDispatch = 1;
}

/* #[<ENTRY_FUNC>]# eDispatcherLink_setTimeout
 * name:         eDispatcherLink_setTimeout
 * global_name:  tMyDispatcherCore_eDispatcherLink_setTimeout
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eDispatcherLink_setTimeout(TWDuration duration)
{
    HAL_TIM_Base_Stop_IT(&g_timeoutTimer);

    // FIXME: TIM3 would overflow if duration >= 0x10000
    g_timeoutTimer.Init.Period = duration;
    g_timeoutTimer.Init.Prescaler = (SystemCoreClock / 2 / 1000) - 1; // 1kHz
    g_timeoutTimer.Init.ClockDivision = 0;
    g_timeoutTimer.Init.CounterMode = TIM_COUNTERMODE_UP;

    if(HAL_TIM_Base_Init(&g_timeoutTimer) != HAL_OK) {
        assert(false);
    }

    HAL_TIM_Base_Start_IT(&g_timeoutTimer);
}

/* #[<ENTRY_FUNC>]# eDispatcherLink_clearTimeout
 * name:         eDispatcherLink_clearTimeout
 * global_name:  tMyDispatcherCore_eDispatcherLink_clearTimeout
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eDispatcherLink_clearTimeout()
{
    HAL_TIM_Base_Stop_IT(&g_timeoutTimer);
}

/* #[<ENTRY_FUNC>]# eDispatcherLink_enterCriticalSection
 * name:         eDispatcherLink_enterCriticalSection
 * global_name:  tMyDispatcherCore_eDispatcherLink_enterCriticalSection
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eDispatcherLink_enterCriticalSection()
{
    __disable_irq();
}

/* #[<ENTRY_FUNC>]# eDispatcherLink_leaveCriticalSection
 * name:         eDispatcherLink_leaveCriticalSection
 * global_name:  tMyDispatcherCore_eDispatcherLink_leaveCriticalSection
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eDispatcherLink_leaveCriticalSection()
{
    __enable_irq();
}

/* #[<ENTRY_FUNC>]# eDispatcherLink_getTime
 * name:         eDispatcherLink_getTime
 * global_name:  tMyDispatcherCore_eDispatcherLink_getTime
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
TWTimePoint
eDispatcherLink_getTime()
{
    return __HAL_TIM_GET_COUNTER(&g_stableClock);
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
    cTimerManager_handleTimeout();
}

/* #[<ENTRY_PORT>]# eMyDispatcher
 * entry port: eMyDispatcher
 * signature:  sMyDispatcherControl
 * context:    task
 * #[</ENTRY_PORT>]# */

/* #[<ENTRY_FUNC>]# eMyDispatcher_enterMainLoop
 * name:         eMyDispatcher_enterMainLoop
 * global_name:  tMyDispatcherCore_eMyDispatcher_enterMainLoop
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eMyDispatcher_enterMainLoop()
{
    // Use TIM3 for the timeout event generation
    g_timeoutTimer.Instance = TIM3;

    // Use TIM2 (32-bit timer) for the stable clock
    g_stableClock.Instance = TIM2;
    g_stableClock.Init.Period = 0xffffffff;
    g_stableClock.Init.Prescaler = (SystemCoreClock / 2 / 1000) - 1; // 1kHz
    g_stableClock.Init.ClockDivision = 0;
    g_stableClock.Init.CounterMode = TIM_COUNTERMODE_UP;

    if(HAL_TIM_Base_Init(&g_stableClock) != HAL_OK) {
        assert(false);
    }

    HAL_TIM_Base_Start(&g_stableClock);

    while (true) {
        while (MD_CheckPendingDeferredDispatch()) {
            cTimerManager_handleDeferredDispatch();
        }
        __disable_irq();
        __WFI();
        __enable_irq();
    }
}

/* #[<POSTAMBLE>]#
 *   Put non-entry functions below.
 * #[</POSTAMBLE>]#*/

static uint8_t MD_CheckPendingDeferredDispatch(void)
{
    uint8_t value;
    __disable_irq();
    value = g_hasPendingDeferredDispatch;
    g_hasPendingDeferredDispatch = 0;
    __enable_irq();
    return value;
}
