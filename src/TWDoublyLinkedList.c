/*
 * Copyright (C) 2017 Tomoaki Kawada
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#include <assert.h>

#include "TWPrivate.h"

void TWDLLPushBackNode(TWPQHeader *header, TWPQNode *node)
{
    assert(header);
    assert(node);

    if (header->first) {
        TWPQNode *first = header->first;
        TWPQNode *last = first->prev;
        node->prev = last;
        node->next = first;
        last->next = node;
        first->prev = node;
    } else {
        header->first = node;
        node->next = node->prev = node;
    }

    assert(node->next->prev == node);
    assert(node->prev->next == node);
}

void TWDLLPushFrontNode(TWPQHeader *header, TWPQNode *node)
{
    assert(header);
    assert(node);

    if (header->first) {
        TWPQNode *first = header->first;
        TWPQNode *last = first->prev;
        node->prev = last;
        node->next = first;
        last->next = node;
        first->prev = node;
        header->first = node;
    } else {
        header->first = node;
        node->next = node->prev = node;
    }

    assert(node->next->prev == node);
    assert(node->prev->next == node);
    assert(header->first == node);
}

void TWDLLInsertNode(TWPQHeader *header, TWPQNode *node, TWPQNode *at)
{
    assert(header);
    assert(node);
    assert(at);
    assert(at != node);

    if (header->first == at) {
        TWDLLPushFrontNode(header, node);
    } else {
        TWPQNode *t = node->prev = at->prev;
        node->next = at;
        t->next = node;
        at->prev = node;
    }

    assert(node->next->prev == node);
    assert(node->prev->next == node);
}

void TWDLLRemoveNode(TWPQHeader *header, TWPQNode *node)
{
    assert(header);
    assert(node);
    assert(node->next->prev == node);
    assert(node->prev->next == node);
    assert(header->first);

    if (header->first == node) {
        if (node->next == node) {
            header->first = NULL;
            return;
        } else {
            header->first = node->next;
        }
    }
    node->next->prev = node->prev;
    node->prev->next = node->next;
}
