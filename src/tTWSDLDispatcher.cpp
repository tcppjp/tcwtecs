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
    unordered_map<intptr_t, SDL_TimerID> g_timers;

    SDL_mutex *g_ddMutex;
    SDL_sem *g_ddWait; // note: SDL's mutex is recursive and cannot be used for this purpose

    SDL_mutex *g_criticalSection;
}

// Custom SDL events
#define TWSE_TIMEOUT                1
#define TWSE_DISPATCH               2
#define TWSE_DEFERRED_DISPATCH      3
#define TWSE_QUIT                   4

static Uint32 SDLTimeoutTimerHandler(Uint32 interval, void *param)
{
    SDL_Event event;
    event.type = SDL_USEREVENT;
    event.user.code = TWSE_TIMEOUT;
    event.user.data1 = param;
    SDL_PushEvent(&event);

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

        SDL_PushEvent(&event);

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
    SDL_Event event;
    event.type = SDL_USEREVENT;
    event.user.code = TWSE_DEFERRED_DISPATCH;

    SDL_PushEvent(&event);
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
        for (int_t i = 0; i < NCP_cSDLEvent; ++i) {
            if (cSDLEvent_handle(i, &e)) {
                goto event_handler_end;
            }
        }

        switch (e.type) {
            case SDL_QUIT:
                return;
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
                    case TWSE_DEFERRED_DISPATCH:
                        cTimerManager_handleDeferredDispatch();
                        break;
                    case TWSE_QUIT:
                        return;
                }
                break;
        }

      event_handler_end:;
    }
}
void
eSDLDispatcher_quit(void)
{
    SDL_Event event;
    event.type = SDL_USEREVENT;
    event.user.code = TWSE_QUIT;
    SDL_PushEvent(&event);

}

