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
#include "tTWDeferredDispatch_tecsgen.h"

uint8_t
eDeferredDispatch_start(CELLIDX idx, intptr_t param)
{
    CELLCB  *p_cellcb = GET_CELLCB(idx);
    TWDeferredDispatchDescriptor *dd = &VAR_descriptor;

    dd->cellcb = (void *)p_cellcb

    return cDeferredDispatchManager_registerDeferredDispatch(dd, param);
}

uint8_t
eDeferredDispatch_clear(CELLIDX idx)
{
    CELLCB  *p_cellcb = GET_CELLCB(idx);
    TWDeferredDispatchDescriptor *dd = &VAR_descriptor;

    return cDeferredDispatchManager_unregisterDeferredDispatch(dd);
}

void
TWFireDeferredDispatch(TWDeferredDispatchDescriptor *dd)
{
    CELLCB *p_cellcb = (CELLCB *)dd->cellcb;

    cTarget_main(dd->param);
}