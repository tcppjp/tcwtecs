/*
 * Copyright (C) 2016 Tomoaki Kawada
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
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
