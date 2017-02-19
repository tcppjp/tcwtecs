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

#pragma once

#include <stddef.h>

#include "tecsui/types.h"

// Private definitions - outsiders should not use them!

/*
 * Helper functions/function-like macros
 */

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define TWGetContainingObject(childPtr, ParentType, childFieldName) \
    (ObjectType *)((char*)(nodePtr) - offsetof(ObjectType, objectNodeFieldName))

/*
 * Priority queue (min-heap; takes O(log N) to extract the minimum element)
 */
struct tagTWPQNode;

typedef struct tagTWPQHeader {
    struct tagTWPQNode *root;
    uint32_t numNodes;
} TWPQHeader;

typedef struct tagTWPQNode {
    struct tagTWPQNode *children[2];
    struct tagTWPQNode *parent;
} TWPQNode;

#define TWPQNodeToObject(nodePtr, ObjectType, objectNodeFieldName) \
    TWGetContainingObject(nodePtr, ObjectType, objectNodeFieldName)

#define TWPQIsEmpty(headerPtr) (!(headerPtr)->root)

#define TWPQGetTopNode(headerPtr) ((headerPtr)->root)

typedef int (*TWPQComparer)(TWPQNode *a, TWPQNode *b, intptr_t param);

void TWPQInsertNode(TWPQHeader *header, TWPQNode *node, TWPQComparer comparer, intptr_t param);
void TWPQRemoveNode(TWPQHeader *header, TWPQNode *node, TWPQComparer comparer, intptr_t param);

/*
 * Doubly linked list
 */
struct tagTWDLLNode;

typedef struct tagTWDLLHeader {
    struct tagTWDLLNode *first;
} TWDLLHeader;

typedef struct tagTWDLLNode {
    // circular
    struct tagTWDLLNode *prev;
    struct tagTWDLLNode *next;
} TWDLLNode;

#define TWDLLNodeToObject(nodePtr, ObjectType, objectNodeFieldName) \
    TWGetContainingObject(nodePtr, ObjectType, objectNodeFieldName)

void TWDLLPushBackNode(TWPQHeader *header, TWPQNode *node);
void TWDLLPushFrontNode(TWPQHeader *header, TWPQNode *node);
void TWDLLInsertNode(TWPQHeader *header, TWPQNode *node, TWPQNode *at);
void TWDLLRemoveNode(TWPQHeader *header, TWPQNode *node);

/*
 * Timer
 */
#define kTWTimerFlagsPeriodic   4

// following two flags are exclusive with each other
#define kTWTimerFlagsActive     1
#define kTWTimerFlagsPending    2

typedef struct tagTWTimerDescriptor {
    union {
        /** Active iff `flags & kTWTimerFlagsActive` */
        TWPQNode node;

        /** Active iff `flags & kTWTimerFlagsPending` */
        TWDLLNode pendingNode;
    };

    /** The value of `p_cellcb` (of type `CELLCB *`) */
    void *cellcb;

    intptr_t param;

    uint8_t flags;
    TWTimePoint nextTickTime;
    TWDuration interval;
} TWTimerDescriptor;

#define TWPQNodeToTimerDescriptor(nodePtr) TWPQNodeToObject(nodePtr, TWTimerDescriptor, node)

extern void TWFireTimer(TWTimerDescriptor *);

/*
 * Deferred dispatch
 */

typedef struct tagTWDeferredDispatchDescriptor {
    TWDLLNode node;

    /** The value of `p_cellcb` (of type `CELLCB *`) */
    void *cellcb;

    intptr_t param;
} TWDeferredDispatchDescriptor;

#define TWDLLNodeToDeferredDispatchDescriptor(nodePtr) TWDLLNodeToObject(nodePtr, TWDeferredDispatchDescriptor, pendingNode)

extern void TWFireDeferredDispatch(TWDeferredDispatchDescriptor *, intptr_t param);

/*
 * Scanline Clipper (C-Buffer implementation)
 */
typedef union tagTWScanlineClipperNode {
    struct {
        uint16_t skip;  // pixels
        uint16_t width; // pixels
        union tagTWScanlineClipperNode *next; // or NULL
    } span;
    struct {
        uint16_t height; // pixels
        int16_t spanIndexOffset; // relative index to the first span, or zero if empty
        union tagTWScanlineClipperNode *next; // or NULL
    } row;
    struct {
        union tagTWScanlineClipperNode *next; // or NULL
    } freelist;
} TWScanlineClipperNode;

typedef struct tagTWScanlineClipperState {
    TWScanlineClipperNode *firstRow;
    TWScanlineClipperNode *continuousFreeZoneBegin;
    TWScanlineClipperNode *freelist;

    uint16_t continuousFreeZoneSize;
    uint16_t top, height;
    uint16_t width;
} TWScanlineClipperState;

typedef struct tagTWScanlineClipperLineScanState {
    uint16_t skip;

    // fields below are private
    uint16_t currentRowY;
    TWScanlineClipperNode *currentRow;
    uint16_t remainingDrawnLines;
    uint16_t currentRowRemainingLines;
} TWScanlineClipperLineScanState;

typedef struct tagTWScanlineClipperSpanScanState {
    uint16_t skip;
    uint16_t width;

    // fields below are private
    uint16_t remainingDrawnPixels;
    TWScanlineClipperNode *currentSpan;
} TWScanlineClipperSpanScanState;

// Clipping/Setup API
extern void TWScanlineClipperInitialize(TWScanlineClipperState *state, TWScanlineClipperNode *heap, uint_fast16_t heapSize);
extern void TWScanlineClipperSetClippingRect(TWScanlineClipperState *state, const TWRect *rect);
extern void TWScanlineClipperSubtractClippingRect(TWScanlineClipperState *state, const TWRect *rect);

// Scanning API
extern void TWScanlineClipperInitializeLineScanner(TWScanlineClipperLineScanState *lineScanState);
extern uint8_t TWScanlineClipperMoveToLine(const TWScanlineClipperState *state, TWScanlineClipperLineScanState *lineScanState, int_fast16_t startY, uint_fast16_t height);
extern uint8_t TWScanlineClipperMoveToNextLine(const TWScanlineClipperState *state, TWScanlineClipperLineScanState *lineScanState);
extern uint8_t TWScanlineClipperStartLine(const TWScanlineClipperState *state, TWScanlineClipperSpanScanState *spanScanState,
    uint_fast16_t inSpanX, uint_fast16_t inSpanWidth, const TWScanlineClipperLineScanState *lineScanState);
extern uint8_t TWScanlineClipperLineAdvance(TWScanlineClipperSpanScanState *spanScanState);



