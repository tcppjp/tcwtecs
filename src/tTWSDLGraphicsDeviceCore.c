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
 * width            uint16_t         ATTR_width
 * height           uint16_t         ATTR_height
 * title            const char*      ATTR_title
 * windowFlags      uint32_t         ATTR_windowFlags
 * window           void*            VAR_window
 * surface          void*            VAR_surface
 *
 * call port function #_TCPF_#
 * call port: cOutput signature: sTWGraphicsDeviceOutput context:task
 *   void           cOutput_getScreenSize( TWSize* outSize );
 *   void           cOutput_setClippingRect( const TWRect* rect );
 *   void           cOutput_subtractClippingRect( const TWRect* rect );
 *   void           cOutput_fillRect( TWColor color, const TWRect* rect );
 *   void           cOutput_drawBitmap( const char* data, TWPixelFormat format, const TWSize* bitmapSize, uint32_t numBytes, const TWRect* inRect, const TWPoint* outLoc, TWColor monoColor );
 *   void           cOutput_update( const TWRect* rect );
 * call port: cKeyboardInputDriverEvent signature: sTWKeyboardInputDriverEvent context:task
 *   void           cKeyboardInputDriverEvent_notifyKeyDown( uint16_t keyCode );
 *   void           cKeyboardInputDriverEvent_notifyKeyUp( uint16_t keyCode );
 *
 * #[</PREAMBLE>]# */

#include <string.h>
#include <SDL2/SDL.h>

#include "tTWSDLGraphicsDeviceCore_tecsgen.h"

#ifndef E_OK
#define	E_OK	0		/* success */
#define	E_ID	(-18)	/* illegal ID */
#endif

static inline SDL_Window *GetSDLWindow(CELLCB *p_cellcb) {
	return (SDL_Window *)VAR_window;
}

static inline SDL_Surface *GetSDLSurface(CELLCB *p_cellcb) {
	return (SDL_Surface *)VAR_surface;
}

/* entry port function #_TEPF_# */
/* #[<ENTRY_PORT>]# eBitmapSource
 * entry port: eBitmapSource
 * signature:  sTWRenderTargetBitmapSource
 * context:    task
 * #[</ENTRY_PORT>]# */

/* #[<ENTRY_FUNC>]# eBitmapSource_get
 * name:         eBitmapSource_get
 * global_name:  tTWSDLGraphicsDeviceCore_eBitmapSource_get
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eBitmapSource_get(CELLIDX idx, void** outData, TWPixelFormat* outPixelFormat, uint16_t* width, uint16_t* height)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);

	SDL_Surface *surf = GetSDLSurface(p_cellcb);
	if (outData)
		*outData = surf->pixels;
	if (outPixelFormat)
		*outPixelFormat = TWPixelFormat32bppRGBX;
	if (width)
		*width = (uint16_t) surf->w;
	if (height)
		*height = (uint16_t) surf->h;
}

/* #[<ENTRY_PORT>]# eOutput
 * entry port: eOutput
 * signature:  sTWGraphicsDeviceOutput
 * context:    task
 * #[</ENTRY_PORT>]# */

/* #[<ENTRY_FUNC>]# eOutput_getScreenSize
 * name:         eOutput_getScreenSize
 * global_name:  tTWSDLGraphicsDeviceCore_eOutput_getScreenSize
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eOutput_getScreenSize(CELLIDX idx, TWSize* outSize)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);

	SDL_Surface *surf = GetSDLSurface(p_cellcb);
	outSize->w = surf->w;
	outSize->h = surf->h;
}

/* #[<ENTRY_FUNC>]# eOutput_setClippingRect
 * name:         eOutput_setClippingRect
 * global_name:  tTWSDLGraphicsDeviceCore_eOutput_setClippingRect
 * oneway:       true
 * #[</ENTRY_FUNC>]# */
void
eOutput_setClippingRect(CELLIDX idx, const TWRect* rect)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);
	cOutput_setClippingRect(rect);
}

/* #[<ENTRY_FUNC>]# eOutput_subtractClippingRect
 * name:         eOutput_subtractClippingRect
 * global_name:  tTWSDLGraphicsDeviceCore_eOutput_subtractClippingRect
 * oneway:       true
 * #[</ENTRY_FUNC>]# */
void
eOutput_subtractClippingRect(CELLIDX idx, const TWRect* rect)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);
	cOutput_subtractClippingRect(rect);
}

/* #[<ENTRY_FUNC>]# eOutput_fillRect
 * name:         eOutput_fillRect
 * global_name:  tTWSDLGraphicsDeviceCore_eOutput_fillRect
 * oneway:       true
 * #[</ENTRY_FUNC>]# */
void
eOutput_fillRect(CELLIDX idx, TWColor color, const TWRect* rect)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);
	cOutput_fillRect(color, rect);
}

/* #[<ENTRY_FUNC>]# eOutput_drawBitmap
 * name:         eOutput_drawBitmap
 * global_name:  tTWSDLGraphicsDeviceCore_eOutput_drawBitmap
 * oneway:       true
 * #[</ENTRY_FUNC>]# */
void
eOutput_drawBitmap(CELLIDX idx, const char* data, TWPixelFormat format, const TWSize* bitmapSize, uint32_t numBytes, const TWRect* inRect, const TWPoint* outLoc, TWColor monoColor)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);
	cOutput_drawBitmap(data, format, bitmapSize, numBytes, inRect, outLoc, monoColor);
}

/* #[<ENTRY_FUNC>]# eOutput_update
 * name:         eOutput_update
 * global_name:  tTWSDLGraphicsDeviceCore_eOutput_update
 * oneway:       true
 * #[</ENTRY_FUNC>]# */
void
eOutput_update(CELLIDX idx, const TWRect* rect)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);
	cOutput_update(rect);

	// Present the current state of framebuffer to the screen
	SDL_Rect rt = {rect->x, rect->y, rect->w, rect->h};

	SDL_Window *wnd = GetSDLWindow(p_cellcb);
	SDL_Surface *wndSurf = SDL_GetWindowSurface(wnd);
	SDL_Surface *surf = GetSDLSurface(p_cellcb);

	SDL_BlitSurface(surf, &rt, wndSurf, &rt);

	SDL_UpdateWindowSurfaceRects(wnd, &rt, 1);
}

/* #[<ENTRY_PORT>]# eControl
 * entry port: eControl
 * signature:  sTWSDLGraphicsDeviceControl
 * context:    task
 * #[</ENTRY_PORT>]# */

/* #[<ENTRY_FUNC>]# eControl_initialize
 * name:         eControl_initialize
 * global_name:  tTWSDLGraphicsDeviceCore_eControl_initialize
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eControl_initialize(CELLIDX idx)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);
	SDL_Window *wnd = SDL_CreateWindow(ATTR_title,
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		ATTR_width, ATTR_height, ATTR_windowFlags);
	VAR_window = (void *)wnd;

	SDL_Surface *surf = SDL_CreateRGBSurface(
		0, ATTR_width, ATTR_height, 32, 0xff, 0xff00, 0xff0000, 0);
	VAR_surface = (void *)surf;
}

/* #[<ENTRY_PORT>]# eSDLEvent
 * entry port: eSDLEvent
 * signature:  sTWSDLEvent
 * context:    task
 * #[</ENTRY_PORT>]# */

/* #[<ENTRY_FUNC>]# eSDLEvent_handle
 * name:         eSDLEvent_handle
 * global_name:  tTWSDLGraphicsDeviceCore_eSDLEvent_handle
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
uint8_t
eSDLEvent_handle(CELLIDX idx, SDL_Event* event)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);

	SDL_Window *wnd = GetSDLWindow(p_cellcb);

	switch (event->type) {
		case SDL_WINDOWEVENT:
			if (event->window.windowID != SDL_GetWindowID(wnd)) {
				return 0;
			}
			// (nothing to handle so far...
			//  maybe SDL_WINDOWEVENT_SIZE_CHANGED?)
			return 1;
		case SDL_MOUSEBUTTONDOWN:
			if (event->button.windowID != SDL_GetWindowID(wnd)) {
				return 0;
			}
			// TODO: emulate touch event
			return 1;
		case SDL_MOUSEBUTTONUP:
			if (event->button.windowID != SDL_GetWindowID(wnd)) {
				return 0;
			}
			// TODO: emulate touch event
			return 1;
		case SDL_MOUSEMOTION:
			if (event->button.windowID != SDL_GetWindowID(wnd)) {
				return 0;
			}
			return 1;
		case SDL_KEYDOWN:
			if (event->key.windowID != SDL_GetWindowID(wnd)) {
				return 0;
			}
			cKeyboardInputDriverEvent_notifyKeyDown(event->key.keysym.sym);
			return 1;
		case SDL_KEYUP:
			if (event->key.windowID != SDL_GetWindowID(wnd)) {
				return 0;
			}
			cKeyboardInputDriverEvent_notifyKeyUp(event->key.keysym.sym);
			return 1;
	}

	return 0;
}

/* #[<POSTAMBLE>]#
 *   Put non-entry functions below.
 * #[</POSTAMBLE>]#*/
