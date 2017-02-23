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

/* Put prototype declaration and/or variale definition here #_PAC_# */
#include "tMyGraphicsDeviceCore_tecsgen.h"

#include "main.h"

#ifndef E_OK
#define	E_OK	0		/* success */
#define	E_ID	(-18)	/* illegal ID */
#endif

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
        *outData = g_framebuffer;
    }
    if (outPixelFormat) {
        *outPixelFormat = TWPixelFormat16bppRGB565;
    }
    if (width) {
        *width = g_framebufferWidth;
    }
    if (height) {
        *height = g_framebufferHeight;
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
    outSize->w = g_framebufferWidth;
    outSize->h = g_framebufferHeight;
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
    MD_UpdateScreen();
}

/* #[<POSTAMBLE>]#
 *   Put non-entry functions below.
 * #[</POSTAMBLE>]#*/
