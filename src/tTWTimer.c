/*
 * Copyright (C) 2017 Tomoaki Kawada
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
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