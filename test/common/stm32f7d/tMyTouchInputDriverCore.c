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
#include <assert.h>
#include "tMyTouchInputDriverCore_tecsgen.h"

#include "stm32f7xx_hal.h"
#include "stm32746g_discovery.h"
#include "stm32746g_discovery_ts.h"
#include "stm32746g_discovery_lcd.h"

#ifndef E_OK
#define	E_OK	0		/* success */
#define	E_ID	(-18)	/* illegal ID */
#endif

static uint8_t g_last_touch_valid = 0;
static uint16_t g_last_touch_x;
static uint16_t g_last_touch_y;

/* entry port function #_TEPF_# */
/* #[<ENTRY_PORT>]# eMyTouchInputDriver
 * entry port: eMyTouchInputDriver
 * signature:  sMyTouchInputDriverControl
 * context:    task
 * #[</ENTRY_PORT>]# */

/* #[<ENTRY_FUNC>]# eMyTouchInputDriver_initialize
 * name:         eMyTouchInputDriver_initialize
 * global_name:  tMyTouchInputDriverCore_eMyTouchInputDriver_initialize
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eMyTouchInputDriver_initialize()
{
    uint8_t status;

    status = BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize());
    assert(status == TS_OK);

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

    TS_StateTypeDef touch_state;
    BSP_TS_GetState(&touch_state);

    uint8_t num_touches = touch_state.touchDetected;

    if (num_touches) {
        if (g_last_touch_valid) {
            cDriverEvent_touchMove(0, TWMakePoint(touch_state.touchX[0], touch_state.touchY[0]));
        } else {
            cDriverEvent_touchStart(0, TWMakePoint(touch_state.touchX[0], touch_state.touchY[0]));
        }
        g_last_touch_x = touch_state.touchX[0];
        g_last_touch_y = touch_state.touchY[0];
        g_last_touch_valid = 1;
    } else if (g_last_touch_valid) {
        cDriverEvent_touchEnd(0, TWMakePoint(g_last_touch_x, g_last_touch_y));
        g_last_touch_valid = 0;
    }
}

/* #[<POSTAMBLE>]#
 *   Put non-entry functions below.
 * #[</POSTAMBLE>]#*/
