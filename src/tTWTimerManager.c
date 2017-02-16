/*
 * Copyright (C) 2017 Tomoaki Kawada
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#include <assert.h>

#include "tTWTimerManager_tecsgen.h"

static int TWTimerComparer(TWPQNode *a, TWPQNode *b, void *param)
{
    TWTimerDescriptor *timer1 = TWPQNodeToTimerDescriptor(a);
    TWTimerDescriptor *timer2 = TWPQNodeToTimerDescriptor(b);

    assert(timer1->flags & kTWTimerFlagsActive);
    assert(timer2->flags & kTWTimerFlagsActive);
    assert(param);

    uint32_t baseTime = *(const uint32_t *)param;
    uint32_t time1 = timer1->nextTickTime - baseTime;
    uint32_t time2 = timer2->nextTickTime - baseTime;
    if (time1 < time2) return -1;
    if (time1 > time2) return 1;
    return 0;
}

static uint32_t TWTTimerManagerGetBaseTime(CELLCB *p_cellcb)
{
    uint32_t baseTime = cDispatcherLink_getTime();
    if (!TWPQIsEmpty(&VAR_timerQueue)) {
        TWTimerDescriptor *firstTimer = TWPQNodeToTimerDescriptor(TWPQGetTopNode(&VAR_timerQueue));
        uint32_t nextTick = firstTimer->nextTick;
        int32_t diff = (int32_t)(baseTime - nextTick);
        if (diff > 0) {
            baseTime -= diff;
        }
    }
    return baseTime;
}

static void TWTimerManagerUpdateTimeout(CELLCB *p_cellcb)
{
    if (!TWPQIsEmpty(&VAR_timerQueue)) {
        TWTimerDescriptor *firstTimer = TWPQNodeToTimerDescriptor(TWPQGetTopNode(&VAR_timerQueue));
        uint32_t currentTime = cDispatcherLink_getTime();
        uint32_t nextTick = firstTimer->nextTick;
        int32_t diff = (int32_t)(nextTick - currentTime);
        if (diff < 0) {
            diff = 0;
        }
        cDispatcherLink_setTimeout(diff);
    } else {
        cDispatcherLink_clearTimeout();
    }
}

void
eTimerManager_registerTimer(CELLIDX idx, TWTimerDescriptor *timer)
{
    CELLCB  *p_cellcb = GET_CELLCB(idx);

    assert(!(timer->flags & kTWTimerFlagsActive));
    assert(!(timer->flags & kTWTimerFlagsPending));

    // The base time depends on the current time, so if we compute this later
    // bad things would happen
    uint32_t baseTime = TWTTimerManagerGetBaseTime(p_cellcb);

    timer->nextTickTime = cDispatcher_getTime() + timer->interval;

    if (VAR_isProcessingTimers) {
        timer->flags |= kTWTimerFlagsPending;

        TWDLLPushBackNode(&VAR_timerPendingList, &timer->pendingNode);
    } else {
        timer->flags |= kTWTimerFlagsActive;

        TWPQInsertNode(&VAR_timerQueue, &timer->node, &TWTimerComparer, &baseTime);

        // FIXME: would be nice to defer this call; we don't wanna call this so often
        TWTimerManagerUpdateTimeout(p_cellcb);
    }
}

void
eTimerManager_unregisterTimer(CELLIDX idx, TWTimerDescriptor *timer)
{
    CELLCB  *p_cellcb = GET_CELLCB(idx);

    if (timer->flags & kTWTimerFlagsPending) {
        assert(!(timer->flags & kTWTimerFlagsActive)); // exclusive

        TWDLLRemoveNode(&VAR_timerPendingList, &timer->pendingNode);
    } else {
        assert(timer->flags & kTWTimerFlagsActive);

        bool needsToUpdateTimeout = TWPQNodeToTimerDescriptor(TWPQGetTopNode(&VAR_timerQueue)) == timer;

        uint32_t baseTime = TWTTimerManagerGetBaseTime(p_cellcb);
        TWPQRemoveNode(&VAR_timerQueue, &timer->node, &TWTimerComparer, &baseTime);

        if (needsToUpdateTimeout) {
            TWTimerManagerUpdateTimeout(p_cellcb);
        }
    }

    timer->flags &= ~(kTWTimerFlagsPending | kTWTimerFlagsActive);
}


uint8_t
eTimerManager_registerDeferredDispatch(CELLIDX idx, TWDeferredDispatchDescriptor *dd, intptr_t param)
{
    CELLCB  *p_cellcb = GET_CELLCB(idx);
    uint8_t ret = 1;

    cDispatcherLink_enterCriticalSection();
    if (dd->node.prev) {
        // Already active
        ret = 0;
        goto end;
    }
    TWDLLPushBackNode(&VAR_deferredDispatchPendingList, &dd->node);

    cDispatcherLink_startDeferredDispatch();

end:
    cDispatcherLink_leaveCriticalSection();

    return ret;
}

uint8_t
eTimerManager_unregisterDeferredDispatch(CELLIDX idx, TWDeferredDispatchDescriptor *dd)
{
    CELLCB  *p_cellcb = GET_CELLCB(idx);
    uint8_t ret = 1;

    cDispatcherLink_enterCriticalSection();
    if (!dd->node.prev) {
        // Already inactive
        ret = 0;
        goto end;
    }
    TWDLLRemoveNode(&VAR_deferredDispatchPendingList, &dd->node);
    dd->node.prev = NULL;

end:
    cDispatcherLink_leaveCriticalSection();

    return ret;
}

void
eTimerManager_handleTimeout(CELLIDX idx)
{
    CELLCB  *p_cellcb = GET_CELLCB(idx);
    VAR_isProcessingTimers = 1;

    uint32_t currentTime = cDispatcherLink_getTime();
    uint32_t baseTime = TWTTimerManagerGetBaseTime(p_cellcb);

    while (!TWPQIsEmpty(&VAR_timerQueue)) {
        TWTimerDescriptor *timer = TWPQNodeToTimerDescriptor(TWPQGetTopNode(&VAR_timerQueue));
        uint32_t nextTick = timer->nextTick;
        int32_t diff = (int32_t)(nextTick - currentTime);
        if (diff > 0) {
            break;
        }

        TWPQRemoveNode(&VAR_timerQueue, &timer->node, &TWTimerComparer, &baseTime);
        uint8_t flags = timer->flags;
        if (flags & kTWTimerFlagsPeriodic) {
            timer->nextTickTime = currentTime + timer->interval;
            timer->flags = kTWTimerFlagsPeriodic | kTWTimerFlagsPending;
            TWDLLPushBackNode(&VAR_timerPendingList, &timer->pendingNode);
        } else {
            timer->flags = 0;
        }

        TWFireTimer(timer);
    }

    VAR_isProcessingTimers = 0;

    // Add all pending timers to the pending timer queue
    if (VAR_timerPendingList.first) {
        TWDLLNode *first = VAR_timerPendingList.first;
        TWDLLNode *node = first;
        do {
            TWDLLNode *next = node->next;
            TWTimerDescriptor *timer = TWDLLNodeToTimerDescriptor(node);
            timer->flags = (timer->flags & ~kTWTimerFlagsPending) | kTWTimerFlagsActive;
            TWPQInsertNode(&VAR_timerQueue, &timer->node, &TWTimerComparer, &baseTime);
            node = next;
        } while (node != first);
        VAR_timerPendingList.first = NULL;
    }

    TWTimerManagerUpdateTimeout(p_cellcb);
}

void
eTimerManager_handleDeferredDispatch(CELLIDX idx)
{
    CELLCB  *p_cellcb = GET_CELLCB(idx);

    while (true) {
        cDispatcherLink_enterCriticalSection();
        if (TWPQIsEmpty(&VAR_deferredDispatchQueue)) {
            cDispatcherLink_leaveCriticalSection();
            return;
        }
        TWDLLNode *first = VAR_deferredDispatchQueue.first;
        TWDLLRemoveNode(&VAR_deferredDispatchQueue, first);

        // Need to extract the param before leaving CS or it could be soon updated by
        // a pending deferred dispatch request
        intptr_t param = first->param;

        // Make the deferred dispatch inactive (allowing for subsequent request)
        first->node.prev = NULL;

        cDispatcherLink_leaveCriticalSection();

        TWFireDeferredDispatch(TWDLLNodeToDeferredDispatchDescriptor(first), param);
    }
}