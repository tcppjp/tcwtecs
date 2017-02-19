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
#include "tTWTimer_tecsgen.h"

void
eTimer_setTimeout(CELLIDX idx, TWDuration duration, intptr_t param)
{
    CELLCB  *p_cellcb = GET_CELLCB(idx);
    TWTimerDescriptor *timer = &VAR_descriptor;

    if (timer->flags & (kTWTimerFlagsActive | kTWTimerFlagsPending)) {
        eTimer_clear();
    }

    timer->param = param;
    timer->flags = 0;
    timer->interval = duration;
    timer->cellcb = (void *)p_cellcb;
    cTimerManager_registerTimer(timer);
}

void
eTimer_setInterval(CELLIDX idx, TWDuration interval, intptr_t param)
{
    CELLCB  *p_cellcb = GET_CELLCB(idx);
    TWTimerDescriptor *timer = &VAR_descriptor;

    if (timer->flags & (kTWTimerFlagsActive | kTWTimerFlagsPending)) {
        eTimer_clear();
    }

    timer->param = param;
    timer->flags = kTWTimerFlagsPeriodic;
    timer->interval = interval;
    timer->cellcb = (void *)p_cellcb;
    cTimerManager_registerTimer(timer);
}

void
eTimer_clear(CELLIDX idx)
{
    CELLCB  *p_cellcb = GET_CELLCB(idx);
    TWTimerDescriptor *timer = &VAR_descriptor;

    if (timer->flags & (kTWTimerFlagsActive | kTWTimerFlagsPending)) {
        cTimerManager_unregisterTimer(timer);
    }
}

void
TWFireTimer(TWTimerDescriptor *timer)
{
    CELLCB *p_cellcb = (CELLCB *)timer->cellcb;

    cTarget_main(timer->param);
}