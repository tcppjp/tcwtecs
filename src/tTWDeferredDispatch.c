/*
 * Copyright (C) 2017 Tomoaki Kawada
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
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