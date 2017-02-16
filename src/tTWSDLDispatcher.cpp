/*
 * Copyright (C) 2017 Tomoaki Kawada
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#include <SDL2/SDL.h>
#include <assert.h>
#include <unordered_map>

#include "tTWSDLDispatcher_tecsgen.h"

using std::unordered_map;

namespace
{
    using TWTimerToken = uintptr_t;

    SDL_threadID g_mainThreadID = 0;
    TWTimerToken g_currentTimeoutToken = 1;
    TWTimerToken g_nextTimeoutToken = 2;
    unordered_map<intrptr_t, SDL_TimerID> g_timers;

    SDL_mutex *g_ddMutex;
    SDL_sem *g_ddWait; // note: SDL's mutex is recursive and cannot be used for this purpose

    SDL_mutex *g_criticalSection;
}

// Custom SDL events
#define TWSE_TIMEOUT                1
#define TWSE_DISPATCH               2
#define TWSE_DEFERRED_DISPATCH      3

static Uint32 SDLTimeoutTimerHandler(Uint32 interval, void *param)
{
    SDL_Event event;
    event.type = SDL_USEREVENT;
    event.user.code = TWSE_TIMEOUT;
    event.user.data1 = param;
    return 1000; // can't let timer removed here; that would cause multithreading problem
}

void
eDispatcher_invoke(Descriptor(sTWDispatchTarget) target, intptr_t param)
{
    if (SDL_ThreadID() == g_mainThreadID) {
        cTarget_set_descriptor(target);
        cTarget_main(param);
    } else {
        SDL_LockMutex(g_ddMutex);

        SDL_Event event;
        event.type = SDL_USEREVENT;
        event.user.code = TWSE_DISPATCH;
        event.user.data1 = &target;
        event.user.data2 = reinterpret_cast<void *>(param);

        SDL_SemWait(g_ddWait);
        SDL_UnlockMutex(g_ddMutex);
    }
}

void
eDispatcherLink_setTimeout(TWDuration duration)
{
    eDispatcherLink_clearTimeout();

    TWTimerToken token = g_nextTimeoutToken++;
    g_currentTimeoutToken = token;

    // Zero is an invalid value for token; just skip it
    if (g_nextTimeoutToken == 0) {
        g_nextTimeoutToken = 1;
    }

    SDL_TimerID timer = SDL_AddTimer(duration, SDLTimeoutTimerHandler,
        reinterpret_cast<void *>(token));

    g_timers.emplace(token, timer);
}
void
eDispatcherLink_clearTimeout(void)
{
    if (g_currentTimeoutToken) {
        auto it = g_timers.find(g_currentTimeoutToken);
        if (it != g_timers.end()) {
            SDL_RemoveTimer(it->second);
            g_timers.erase(it);
        }
        g_currentTimeoutToken = 0;
    }
}

void
eDispatcherLink_startDeferredDispatch(void)
{
    // TODO: deferred dispach
}

TWTimePoint
eDispatcherLink_getTime(void)
{
    return (TWTimePoint) SDL_GetTicks();
}

void
eDispatcherLink_enterCriticalSection(void)
{
    SDL_LockMutex(g_criticalSection);
}

void
eDispatcherLink_leaveCriticalSection(void)
{
    SDL_UnlockMutex(g_criticalSection);
}

void
eSDLDispatcher_initialize(void)
{
    // not sure if this should be here...
    g_mainThreadID = SDL_ThreadID();

    g_ddMutex = SDL_CreateMutex();
    g_ddWait = SDL_CreateSemaphore(0);

    g_criticalSection = SDL_CreateMutex();
}

void
eSDLDispatcher_enterMainLoop(void)
{
    g_mainThreadID = SDL_ThreadID();

    SDL_Event e;
    while (SDL_WaitEvent(&e)) {
        switch (e.type) {
            case SDL_QUIT:
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
            case SDL_USEREVENT:
                switch (e.user.code) {
                    case TWSE_TIMEOUT: {
                        TWTimerToken token = reinterpret_cast<TWTimerToken>(e.user.data1);
                        auto it = g_timers.find(token);
                        if (it != g_timers.end()) {
                            SDL_RemoveTimer(it->second);
                            g_timers.erase(it);
                            if (token == g_currentTimeoutToken) {
                                g_currentTimeoutToken = 0;
                                cTimerManager_handleTimeout();
                            }
                        }
                    } break;
                    case TWSE_DISPATCH: {
                        auto &target = reinterpret_cast<Descriptor(sTWDispatchTarget)&>(e.user.data1);
                        cTarget_set_descriptor(target);
                        cTarget_main(reinterpret_cast<intptr_t>(e.user.data2));

                        SDL_SemPost(g_ddWait);
                    } break;
                }
                break;
        }

        if (is_cEvent_joined()) {
            cEvent_idle();
        }
    }
}
void
eSDLDispatcher_quit(void)
{
}

