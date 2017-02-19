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

