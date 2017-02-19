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

#include <assert.h>

#include "tecsui/private.h"

void TWDLLPushBackNode(TWDLLHeader *header, TWDLLNode *node)
{
    assert(header);
    assert(node);

    if (header->first) {
        TWDLLNode *first = header->first;
        TWDLLNode *last = first->prev;
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

void TWDLLPushFrontNode(TWDLLHeader *header, TWDLLNode *node)
{
    assert(header);
    assert(node);

    if (header->first) {
        TWDLLNode *first = header->first;
        TWDLLNode *last = first->prev;
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

void TWDLLInsertNode(TWDLLHeader *header, TWDLLNode *node, TWDLLNode *at)
{
    assert(header);
    assert(node);
    assert(at);
    assert(at != node);

    if (header->first == at) {
        TWDLLPushFrontNode(header, node);
    } else {
        TWDLLNode *t = node->prev = at->prev;
        node->next = at;
        t->next = node;
        at->prev = node;
    }

    assert(node->next->prev == node);
    assert(node->prev->next == node);
}

void TWDLLRemoveNode(TWDLLHeader *header, TWDLLNode *node)
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
