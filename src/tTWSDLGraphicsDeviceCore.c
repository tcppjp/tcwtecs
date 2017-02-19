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
 * call port: cInput signature: sTWGraphicsDeviceInput context:task
 *   void           cInput_mouseDown( TWPoint point, uint8_t button );
 *   void           cInput_mouseMove( TWPoint point );
 *   void           cInput_mouseUp( TWPoint point, uint8_t button );
 *   void           cInput_keyDown( uint16_t keyCode );
 *   void           cInput_keyUp( uint16_t keyCode );
 *   void           cInput_resize( );
 * call port: cOutput signature: sTWGraphicsDeviceOutput context:task
 *   void           cOutput_getScreenSize( TWSize* outSize );
 *   void           cOutput_setScissorRect( const TWRect* rect );
 *   void           cOutput_fillRect( TWColor color, const TWRect* rect );
 *   void           cOutput_drawBitmap( const char* data, TWPixelFormat format, const TWSize* bitmapSize, uint32_t numBytes, const TWRect* inRect, const TWPoint* outLoc, TWColor monoColor );
 *   void           cOutput_update( const TWRect* rect );
 * call port: cEvent signature: sTWSDLGraphicsDeviceEvent context:task optional:true
 *   bool_t     is_cEvent_joined()                     check if joined
 *   void           cEvent_idle( );
 *
 * #[</PREAMBLE>]# */

#include <string.h>
#include <SDL2/SDL.h>

#include "tTWSDLGraphicsDeviceCore_tecsgen.h"

#ifndef E_OK
#define	E_OK	0		/* success */
#define	E_ID	(-18)	/* illegal ID */
#endif

// TODO: user induced validation event
#define TWSE_VALIDATE 	(SDL_USEREVENT + 0x0001)
#define TWSE_QUIT 		(SDL_USEREVENT + 0x0002)

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

/* #[<ENTRY_FUNC>]# eOutput_setScissorRect
 * name:         eOutput_setScissorRect
 * global_name:  tTWSDLGraphicsDeviceCore_eOutput_setScissorRect
 * oneway:       true
 * #[</ENTRY_FUNC>]# */
void
eOutput_setScissorRect(CELLIDX idx, const TWRect* rect)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);
	cOutput_setScissorRect(rect);
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

/* #[<ENTRY_FUNC>]# eControl_enterMainLoop
 * name:         eControl_enterMainLoop
 * global_name:  tTWSDLGraphicsDeviceCore_eControl_enterMainLoop
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eControl_enterMainLoop(CELLIDX idx)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);

	SDL_Window *wnd = GetSDLWindow(p_cellcb);

	SDL_Event e;
	while (SDL_WaitEvent(&e)) {
		switch (e.type) {
			case SDL_QUIT:
				// TODO: custom handling of SDL_QUIT
				return;
			case SDL_WINDOWEVENT:
				if (e.window.windowID != SDL_GetWindowID(wnd)) {
					break;
				}
				switch (e.window.event) {
					case SDL_WINDOWEVENT_SIZE_CHANGED: {
						int new_w, new_h;
						SDL_GetWindowSize(wnd, &new_w, &new_h);

						SDL_FreeSurface(GetSDLSurface(p_cellcb));

						// FIXME: duplicated code
						SDL_Surface *surf = SDL_CreateRGBSurface(
							0, new_w, new_h, 32, 0xff, 0xff00, 0xff0000, 0);
						VAR_surface = (void *)surf;

						cInput_resize();
						break;
					}
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				cInput_mouseDown(TWMakePoint(e.button.x, e.button.y), e.button.button);
				break;
			case SDL_MOUSEBUTTONUP:
				cInput_mouseUp(TWMakePoint(e.button.x, e.button.y), e.button.button);
				break;
			case SDL_MOUSEMOTION:
				cInput_mouseMove(TWMakePoint(e.motion.x, e.motion.y));
				break;
			case SDL_KEYDOWN:
				cInput_keyDown(e.key.keysym.sym);
				break;
			case SDL_KEYUP:
				cInput_keyUp(e.key.keysym.sym);
				break;
			case TWSE_VALIDATE:
				// TODO: user induced validation event
				break;
			case TWSE_QUIT:
				return;
		}

		if (is_cEvent_joined()) {
			cEvent_idle();
		}
	}
}

/* #[<ENTRY_FUNC>]# eControl_quit
 * name:         eControl_quit
 * global_name:  tTWSDLGraphicsDeviceCore_eControl_quit
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eControl_quit(CELLIDX idx)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);

	SDL_Event ev;
	memset(&ev, 0, sizeof(ev));
	ev.type = TWSE_QUIT;
	SDL_PushEvent(&ev);
}

/* #[<POSTAMBLE>]#
 *   Put non-entry functions below.
 * #[</POSTAMBLE>]#*/
