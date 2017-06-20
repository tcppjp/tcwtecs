/*
 * Copyright (C) 2017 Tomoaki Kawada
 *
 * This file is part of tcwtecs.
 *
 * A part of this source code is based on a code provided by STMicroelectronics
 * and you are required to adhere to the following license terms for
 * redistribution:
 *
 *   Copyright (c) 2016 STMicroelectronics International N.V.
 *   All rights reserved.
 *
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted, provided that the following conditions are met:
 *
 *   1. Redistribution of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of other
 *      contributors to this software may be used to endorse or promote products
 *      derived from this software without specific written permission.
 *   4. This software, including modifications and/or derivative works of this
 *      software, must execute solely and exclusively on microcontroller or
 *      microprocessor devices manufactured by or for STMicroelectronics.
 *   5. Redistribution and use of this software other than as permitted under
 *      this license is void and will automatically terminate your rights under
 *      this license.
 *
 *   THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS"
 *   AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
 *   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 *   PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
 *   RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
 *   SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *   INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 *   OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 *   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 *   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 *   EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */

/* #[<PREAMBLE>]#
 * Don't edit the comments between #[<...>]# and #[</...>]#
 * These comment are used by tecsmerege when merging.
 *
 * call port function #_TCPF_#
 * call port: cOutput signature: sTWGraphicsDeviceOutput context:task
 *   void           cOutput_getScreenSize( TWSize* outSize );
 *   void           cOutput_setClippingRect( const TWRect* rect );
 *   void           cOutput_subtractClippingRect( const TWRect* rect );
 *   void           cOutput_setScissorRect( const TWRect* rect );
 *   void           cOutput_fillRect( TWColor color, const TWRect* rect );
 *   void           cOutput_drawBitmap( const char* data, uint32_t numBytes, const TWBitmapInfo* info, uint16_t infoSize, const TWRect* inRect, const TWPoint* outLoc );
 *   void           cOutput_update( const TWRect* rect );
 * call port: cGraphicsDevice signature: sTWGraphicsDeviceInput context:task
 *   void           cGraphicsDevice_resize( );
 *
 * #[</PREAMBLE>]# */
#include "tMyGraphicsDeviceCore.h"

/* Put prototype declaration and/or variale definition here #_PAC_# */
#include "tMyGraphicsDeviceCore_tecsgen.h"

#ifndef E_OK
#define	E_OK	0		/* success */
#define	E_ID	(-18)	/* illegal ID */
#endif

// The framebuffer is allocated from SDRAM
#define LCD_FRAME_BUFFER    ((int)0xC0000000)
#define LCD_WIDTH           480
#define LCD_HEIGHT          272

LTDC_HandleTypeDef g_hltdc;

/* entry port function #_TEPF_# */
/* #[<ENTRY_PORT>]# eBitmapSource
 * entry port: eBitmapSource
 * signature:  sTWRenderTargetBitmapSource
 * context:    task
 * #[</ENTRY_PORT>]# */

/* #[<ENTRY_FUNC>]# eBitmapSource_get
 * name:         eBitmapSource_get
 * global_name:  tMyGraphicsDeviceCore_eBitmapSource_get
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eBitmapSource_get(void** outData, TWPixelFormat* outPixelFormat, uint16_t* width, uint16_t* height)
{
    if (outData) {
        *outData = (void *)LCD_FRAME_BUFFER;
    }
    if (outPixelFormat) {
        *outPixelFormat = TWPixelFormat32bppRGBX;
    }
    if (width) {
        *width = LCD_WIDTH;
    }
    if (height) {
        *height = LCD_HEIGHT;
    }
}

/* #[<ENTRY_PORT>]# eOutput
 * entry port: eOutput
 * signature:  sTWGraphicsDeviceOutput
 * context:    task
 * #[</ENTRY_PORT>]# */

/* #[<ENTRY_FUNC>]# eOutput_getScreenSize
 * name:         eOutput_getScreenSize
 * global_name:  tMyGraphicsDeviceCore_eOutput_getScreenSize
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eOutput_getScreenSize(TWSize* outSize)
{
    outSize->w = LCD_WIDTH;
    outSize->h = LCD_HEIGHT;
}

/* #[<ENTRY_FUNC>]# eOutput_setClippingRect
 * name:         eOutput_setClippingRect
 * global_name:  tMyGraphicsDeviceCore_eOutput_setClippingRect
 * oneway:       true
 * #[</ENTRY_FUNC>]# */
void
eOutput_setClippingRect(const TWRect* rect)
{
    cOutput_setClippingRect(rect);
}

/* #[<ENTRY_FUNC>]# eOutput_subtractClippingRect
 * name:         eOutput_subtractClippingRect
 * global_name:  tMyGraphicsDeviceCore_eOutput_subtractClippingRect
 * oneway:       true
 * #[</ENTRY_FUNC>]# */
void
eOutput_subtractClippingRect(const TWRect* rect)
{
    cOutput_subtractClippingRect(rect);
}

/* #[<ENTRY_FUNC>]# eOutput_setScissorRect
 * name:         eOutput_setScissorRect
 * global_name:  tMyGraphicsDeviceCore_eOutput_setScissorRect
 * oneway:       true
 * #[</ENTRY_FUNC>]# */
void
eOutput_setScissorRect(const TWRect* rect)
{
    cOutput_setScissorRect(rect);
}

/* #[<ENTRY_FUNC>]# eOutput_fillRect
 * name:         eOutput_fillRect
 * global_name:  tMyGraphicsDeviceCore_eOutput_fillRect
 * oneway:       true
 * #[</ENTRY_FUNC>]# */
void
eOutput_fillRect(TWColor color, const TWRect* rect)
{
    cOutput_fillRect(color, rect);
}

/* #[<ENTRY_FUNC>]# eOutput_drawBitmap
 * name:         eOutput_drawBitmap
 * global_name:  tMyGraphicsDeviceCore_eOutput_drawBitmap
 * oneway:       true
 * #[</ENTRY_FUNC>]# */
void
eOutput_drawBitmap(const char* data, uint32_t numBytes, const TWBitmapInfo* info, uint16_t infoSize, const TWRect* inRect, const TWPoint* outLoc)
{
    cOutput_drawBitmap(data, numBytes, info, infoSize, inRect, outLoc);
}

/* #[<ENTRY_FUNC>]# eOutput_update
 * name:         eOutput_update
 * global_name:  tMyGraphicsDeviceCore_eOutput_update
 * oneway:       true
 * #[</ENTRY_FUNC>]# */
void
eOutput_update(const TWRect* rect)
{
    (void) rect;
    // TODO: maybe we need to flush the SDRAM cache
}

/* #[<ENTRY_PORT>]# eMyGraphicsDevice
 * entry port: eMyGraphicsDevice
 * signature:  sMyGraphicsDeviceControl
 * context:    task
 * #[</ENTRY_PORT>]# */

/* #[<ENTRY_FUNC>]# eMyGraphicsDevice_initialize
 * name:         eMyGraphicsDevice_initialize
 * global_name:  tMyGraphicsDeviceCore_eMyGraphicsDevice_initialize
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eMyGraphicsDevice_initialize()
{
    // Configure LTDC for the target board's integrated display
    g_hltdc.Init.HorizontalSync = 40;
    g_hltdc.Init.VerticalSync = 9;
    g_hltdc.Init.AccumulatedHBP = 53;
    g_hltdc.Init.AccumulatedVBP = 11;
    g_hltdc.Init.AccumulatedActiveH = 283;
    g_hltdc.Init.AccumulatedActiveW = 533;
    g_hltdc.Init.TotalHeigh = 285;
    g_hltdc.Init.TotalWidth = 565;

    g_hltdc.Init.Backcolor.Blue = 255;
    g_hltdc.Init.Backcolor.Green = 0;
    g_hltdc.Init.Backcolor.Red = 0;

    g_hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
    g_hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;
    g_hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;
    g_hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
    g_hltdc.Instance = LTDC;

    HAL_LTDC_Init(&g_hltdc);
    HAL_LTDC_ProgramLineEvent(&g_hltdc, 0);

    HAL_LTDC_EnableDither(&g_hltdc);

    // Enable the display
    HAL_GPIO_WritePin(GPIOI, GPIO_PIN_12, GPIO_PIN_SET);

    // Turn on the backlight
    HAL_GPIO_WritePin(GPIOK, GPIO_PIN_3, GPIO_PIN_SET);

    // Configure a layer
    LTDC_LayerCfgTypeDef layer_cfg;

    layer_cfg.WindowX0 = 0;
    layer_cfg.WindowX1 = LCD_WIDTH;
    layer_cfg.WindowY0 = 0;
    layer_cfg.WindowY1 = LCD_HEIGHT;
    layer_cfg.PixelFormat = LTDC_PIXEL_FORMAT_RGB565;
    layer_cfg.FBStartAdress = ((uint32_t)LCD_FRAME_BUFFER);
    layer_cfg.Alpha = 255;
    layer_cfg.Alpha0 = 0;
    layer_cfg.Backcolor.Blue = 0;
    layer_cfg.Backcolor.Green = 0;
    layer_cfg.Backcolor.Red = 0;
    layer_cfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
    layer_cfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
    layer_cfg.ImageWidth = LCD_WIDTH;
    layer_cfg.ImageHeight = LCD_HEIGHT;

    HAL_LTDC_ConfigLayer(&g_hltdc, &layer_cfg, 0);
}

/* #[<POSTAMBLE>]#
 *   Put non-entry functions below.
 * #[</POSTAMBLE>]#*/


/**
  * @brief LTDC MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param hltdc: LTDC handle pointer
  * @retval None
  */
void HAL_LTDC_MspInit(LTDC_HandleTypeDef *hltdc)
{
  GPIO_InitTypeDef gpio_init_structure;
  static RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

  /* Enable the LTDC clocks */
  __HAL_RCC_LTDC_CLK_ENABLE();


  /* LCD clock configuration */
  /* PLLSAI_VCO Input = HSE_VALUE/PLL_M = 1 Mhz */
  /* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAIN = 429 Mhz */
  /* PLLLCDCLK = PLLSAI_VCO Output/PLLSAIR = 429/5 = 85 Mhz */
  /* LTDC clock frequency = PLLLCDCLK / LTDC_PLLSAI_DIVR_2 = 85/4 = 21 Mhz */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
  PeriphClkInitStruct.PLLSAI.PLLSAIN = 192;
  PeriphClkInitStruct.PLLSAI.PLLSAIR = 5;
  PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_4;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);


  /* Enable GPIOs clock */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_GPIOJ_CLK_ENABLE();
  __HAL_RCC_GPIOK_CLK_ENABLE();

  /*** LTDC Pins configuration ***/
  /* GPIOE configuration */
  gpio_init_structure.Pin       = GPIO_PIN_4;
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_NOPULL;
  gpio_init_structure.Speed     = GPIO_SPEED_FAST;
  gpio_init_structure.Alternate = GPIO_AF14_LTDC;
  HAL_GPIO_Init(GPIOE, &gpio_init_structure);

  /* GPIOG configuration */
  gpio_init_structure.Pin       = GPIO_PIN_12;
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Alternate = GPIO_AF9_LTDC;
  HAL_GPIO_Init(GPIOG, &gpio_init_structure);

  /* GPIOI LTDC alternate configuration */
  gpio_init_structure.Pin       = GPIO_PIN_9 | GPIO_PIN_10 | \
                                  GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Alternate = GPIO_AF14_LTDC;
  HAL_GPIO_Init(GPIOI, &gpio_init_structure);

  /* GPIOJ configuration */
  gpio_init_structure.Pin       = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | \
                                  GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | \
                                  GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | \
                                  GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Alternate = GPIO_AF14_LTDC;
  HAL_GPIO_Init(GPIOJ, &gpio_init_structure);

  /* GPIOK configuration */
  gpio_init_structure.Pin       = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_4 | \
                                  GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Alternate = GPIO_AF14_LTDC;
  HAL_GPIO_Init(GPIOK, &gpio_init_structure);

  /* LCD_DISP GPIO configuration */
  gpio_init_structure.Pin       = GPIO_PIN_12;     /* LCD_DISP pin has to be manually controlled */
  gpio_init_structure.Mode      = GPIO_MODE_OUTPUT_PP;
  HAL_GPIO_Init(GPIOI, &gpio_init_structure);

  /* LCD_BL_CTRL GPIO configuration */
  gpio_init_structure.Pin       = GPIO_PIN_3;  /* LCD_BL_CTRL pin has to be manually controlled */
  gpio_init_structure.Mode      = GPIO_MODE_OUTPUT_PP;
  HAL_GPIO_Init(GPIOK, &gpio_init_structure);

  /* Set LTDC Interrupt to the lowest priority */
  HAL_NVIC_SetPriority(LTDC_IRQn, 0xE, 0);

  /* Enable LTDC Interrupt */
  HAL_NVIC_EnableIRQ(LTDC_IRQn);
}

/**
  * @brief LTDC MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  * @param hltdc: LTDC handle pointer
  * @retval None
  */
void HAL_LTDC_MspDeInit(LTDC_HandleTypeDef *hltdc)
{
  /* Reset peripherals */
  /* Enable LTDC reset state */
  __HAL_RCC_LTDC_FORCE_RESET();

  /* Release LTDC from reset state */
  __HAL_RCC_LTDC_RELEASE_RESET();
}