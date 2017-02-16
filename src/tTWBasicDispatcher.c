/*
 * Copyright (C) 2017 Tomoaki Kawada
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#include "tTWBasicDispatcher_tecsgen.h"

void
eDispatcher_invoke(CELLIDX idx, Descriptor(sTWDispatchTarget) target, intptr_t param)
{
    CELLCB  *p_cellcb = GET_CELLCB(idx);

    cTarget_set_descriptor(target);
    cTarget_main(param);
}

void
eDispatcherLink_startDeferredDispatch(void)
{
    CELLCB  *p_cellcb = GET_CELLCB(idx); (void) p_cellcb;
    // TODO: how should I implement this?
}

void
eDispatcherLink_setTimeout(CELLIDX idx, TWDuration duration)
{
    CELLCB  *p_cellcb = GET_CELLCB(idx); (void) p_cellcb;
    // TODO: how should I implement this?
}
void
eDispatcherLink_clearTimeout(CELLIDX idx)
{
    CELLCB  *p_cellcb = GET_CELLCB(idx); (void) p_cellcb;
    // TODO: how should I implement this?
}

TWTimePoint
eDispatcherLink_getTime(CELLIDX idx)
{    CELLCB  *p_cellcb = GET_CELLCB(idx); (void) p_cellcb;
    // TODO: uh-oh. C doens't have std::chrono::stable_clock!!!
    return 0;
}

void
eDispatcherLink_enterCriticalSection(void)
{
    // TODO: how should I implement this?
}

void
eDispatcherLink_leaveCriticalSection(void)
{
    // TODO: how should I implement this?
}

