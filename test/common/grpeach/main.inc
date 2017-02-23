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

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

// main.cpp
extern uint8_t g_framebuffer[];
extern const uint16_t g_framebufferWidth;
extern const uint16_t g_framebufferHeight;

void MD_StartDeferredDispatch(void);
void MD_Invoke(void *param);
void MD_SetTimeout(uint32_t duration, uintptr_t token);
void MD_ClearTimeout(void);
uint32_t MD_GetTime(void);
void MD_EnterCriticalSection(void);
void MD_LeaveCriticalSection(void);
void MD_UpdateScreen(void);

void MD_PollTouchInput(void);

// tMyTouchInputDriverCore.c
void MD_TouchStart(uint8_t touchId, int16_t x, int16_t y);
void MD_TouchMove(uint8_t touchId, int16_t x, int16_t y);
void MD_TouchEnd(uint8_t touchId, int16_t x, int16_t y);

// tMyDispatcherCore.c
void MD_Initialize(void);
void MD_HandleDeferredDispatch(void);
void MD_HandleInvoke(void *param);
void MD_HandleTimeout(uintptr_t token);

#ifdef __cplusplus
}
#endif

#ifdef EMIT_MAIN_IMPLEMENTATION

#include "mbed.h"
#include "DisplayBace.h"
#include "rtos.h"
#include "LCD_shield_config_4_3inch.h"

#define GRAPHICS_FORMAT                     (DisplayBase::GRAPHICS_FORMAT_RGB565)
#define WR_RD_WRSWA                         (DisplayBase::WR_RD_WRSWA_32_16BIT)
#define FRAME_BUFFER_BYTE_PER_PIXEL         (2)
#define FRAME_BUFFER_STRIDE                 (((LCD_PIXEL_WIDTH * FRAME_BUFFER_BYTE_PER_PIXEL) + 31u) & ~31u)

// TODO: make sure FRAME_BUFFER_STRIDE == LCD_PIXEL_WIDTH * FRAME_BUFFER_BYTE_PER_PIXEL
//       (otherwise tRGB565blahblah cannot handle that)

uint8_t g_framebuffer[FRAME_BUFFER_STRIDE * LCD_PIXEL_HEIGHT]__attribute((aligned(32))); /* 32 bytes aligned */

const uint16_t g_framebufferWidth = LCD_PIXEL_WIDTH;
const uint16_t g_framebufferHeight = LCD_PIXEL_HEIGHT;

namespace {
    DigitalOut  lcd_pwon(P7_15);
    DigitalOut  lcd_blon(P8_1);
    PwmOut      lcd_cntrst(P8_15);
    DisplayBase display;

    TouckKey_LCD_shield touch(P4_0, P2_13, I2C_SDA, I2C_SCL);

    Timer timer;

    Thread main_thread;

    Mutex md_invoke_mutex;
    Semaphore md_invoke_done_semaphore(1); // Mutex may be recursive, so use semaphore
    void *md_pending_invoke = NULL;

    Mutex md_timer_mutex;
    uint32_t md_next_timeout = 0;
    uintptr_t md_next_token = 0;

    void TickHandler(void const *);
    RtosTimer md_timer(&TickHandler);

    void VSyncInterruptHandler(DisplayBase::int_type_t int_type)
    {
        // TODO: vsync?
    }

    void Heartbeat()
    {
        static uint32_t counter = 0;
        ++ counter;
        for (int i = 0; i < 32; ++i) {
            g_framebuffer[i * 2] = g_framebuffer[i * 2 + 1] =
                ((counter >> i) & 1) ? 0xff : 0x00;
        }
        MD_UpdateScreen();
    }

    void
    TickHandler(void const *)
    {
        md_timer_mutex.lock();
        if (md_next_token != 0) {
            if ((int32_t)(MD_GetTime() - md_next_timeout) > 0) {
                uintptr_t token = md_next_token;
                md_next_token = 0;
                MD_HandleTimeout(token);
            }
        }
        md_timer_mutex.unlock();
    }

    void
    RunMainThread()
    {
        puts("main thread starting");

        MD_Initialize();

        while (true) {
            osEvent event = Thread::signal_wait(0);

            if (event.value.signals & 1) {
                // invoke
                void *pending_invoke = md_pending_invoke;
                md_pending_invoke = NULL;

                MD_HandleInvoke(pending_invoke);
                md_invoke_done_semaphore.release();
            }

            if (event.value.signals & 2) {
                MD_HandleDeferredDispatch();
            }
        }
    }

}

extern "C" void
MD_StartDeferredDispatch(void)
{
    main_thread.signal_set(2);
}

extern "C" void
MD_Invoke(void *param)
{
    md_invoke_mutex.lock();

    md_invoke_done_semaphore.wait();

    md_pending_invoke = param;
    main_thread.signal_set(1);

    // Wait for completion
    md_invoke_done_semaphore.wait();
    md_invoke_done_semaphore.release();

    md_invoke_mutex.unlock();
}

extern "C" void
MD_SetTimeout(uint32_t duration, uintptr_t token)
{
    md_timer_mutex.lock();
    md_next_token = token;
    md_next_timeout = MD_GetTime() + duration;
    md_timer_mutex.unlock();

    md_timer.start(1);
}

extern "C" void
MD_ClearTimeout(void)
{
    md_timer.stop();

    md_timer_mutex.lock();
    md_next_token = 0;
    md_timer_mutex.unlock();
}

extern "C" uint32_t
MD_GetTime(void)
{
    return (uint32_t) timer.read_ms();
}

extern "C" void
MD_EnterCriticalSection(void)
{
    __disable_irq();
}

extern "C" void
MD_LeaveCriticalSection(void)
{
    __enable_irq();
}

extern "C" void
MD_UpdateScreen(void)
{
    __v7_clean_dcache_all();
    display.Graphics_Read_Change(DisplayBase::GRAPHICS_LAYER_0, (void *)g_framebuffer);
}

namespace
{
    TouchKey::touch_pos_t md_last_touch;
    bool md_last_touch_valid = false;
}

extern "C" void
MD_PollTouchInput(void)
{
    TouchKey::touch_pos_t touch_locations[1];
    int num_touches = touch.GetCoordinates(1, touch_locations);

    if (num_touches) {
        if (md_last_touch_valid) {
            MD_TouchMove(0, touch_locations[0].x, touch_locations[0].y);
        } else {
            MD_TouchStart(0, touch_locations[0].x, touch_locations[0].y);
        }
        md_last_touch = touch_locations[0];
        md_last_touch_valid = true;
    } else if (md_last_touch_valid) {
        MD_TouchEnd(0, md_last_touch.x, md_last_touch.y);
        md_last_touch_valid = false;
    }
}

int
main(void)
{
    // Initialize the LCD display
    DisplayBase::graphics_error_t error;
    DisplayBase::lcd_config_t lcd_config = LcdCfgTbl_LCD_shield;
    PinName lvds_pin[8] = {
        /* data pin */
        P5_7, P5_6, P5_5, P5_4, P5_3, P5_2, P5_1, P5_0
    };

    lcd_pwon = 0;
    lcd_blon = 0;
    Thread::wait(100);
    lcd_pwon = 1;
    lcd_blon = 1;

    display.Graphics_Lvds_Port_Init(lvds_pin, 8);

    error = display.Graphics_init(&lcd_config);
    if (error != DisplayBase::GRAPHICS_OK) {
        printf("Line %d, error %d\n", __LINE__, error);
        mbed_die();
    }

    error = display.Graphics_Irq_Handler_Set(DisplayBase::INT_TYPE_S0_LO_VSYNC, 0, VSyncInterruptHandler);
    if (error != DisplayBase::GRAPHICS_OK) {
        printf("Line %d, error %d\n", __LINE__, error);
        mbed_die();
    }

    // Turn on the backlight
    lcd_cntrst.write(1.0);

    // Setup the framebuffer
    DisplayBase::rect_t rect;
    rect.vs = 0;
    rect.vw = LCD_PIXEL_HEIGHT;
    rect.hs = 0;
    rect.hw = LCD_PIXEL_WIDTH;

    display.Graphics_Read_Setting(
        DisplayBase::GRAPHICS_LAYER_0,
        (void *)g_framebuffer,
        FRAME_BUFFER_STRIDE,
        GRAPHICS_FORMAT,
        WR_RD_WRSWA,
        &rect
    );
    display.Graphics_Start(DisplayBase::GRAPHICS_LAYER_0);

    // Setup the touchscreen
    touch.Reset();

    // Disable the touchscreen interrupt --- it freezes OS for some reason
    InterruptIn iin(P2_13);
    iin.disable_irq();

    // Enter the main loop
    timer.start();
    main_thread.start(&RunMainThread);

    main_thread.join();
    puts("main thread killed");

    memset(g_framebuffer, 0x7f, sizeof(g_framebuffer));
    MD_UpdateScreen();

    mbed_die();
}

#endif
