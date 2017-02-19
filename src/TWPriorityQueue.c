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

/**
 * TWULog2(1) = 0, TWULog2(2) = TWULog2(3) = 1, TWULog2(4) = 2, ...
 */
static uint_fast8_t TWULog2(uint32_t value)
{
    uint_fast8_t ret = -1;
    while (value) {
        ++ret; value >>= 1;
    }
    return ret;
}

// WARNING! This code is NOT tested at all!

static void TWPQSwapNodeAndParent(TWPQNode *node, TWPQNode *parent)
{
    assert(node);
    assert(node->parent == parent);
    assert(parent->children[0] == node || parent->children[1] == node);

    TWPQNode *t1, *t2;
    int i = parent->children[1] == node;

    assert(parent->children[i] == node);

    node->parent = parent->parent;
    parent->parent = node;

    t1 = parent->children[1 - i];
    t2 = parent->children[1 - i] = node->children[1 - i];
    node->children[1 - i] = t1;
    if (t1) {
        t1->parent = node;
    }
    if (t2) {
        t2->parent = parent;
    }

    t1 = parent->children[i] = node->children[i];
    node->children[i] = parent;
    if (t1) {
        t1->parent = parent;
    }
}

void TWPQInsertNode(TWPQHeader *header, TWPQNode *node, TWPQComparer comparer, intptr_t param)
{
    assert(header);
    assert(node);
    assert(comparer);

    uint32_t numNodes = header->numNodes;
    node->children[0] = NULL;
    node->children[1] = NULL;

    if (numNodes == 0) {
        assert(!header->root);

        header->root = node;
        node->parent = NULL;
        header->numNodes = 1;
        return;
    }

    ++numNodes;

    // Find out where to insert the new node
    TWPQNode *parent = header->root;
    if (((numNodes - 1) & numNodes) == 0) {
        // Insert as the left child of the leftmost node
        for (uint32_t i = numNodes - 1; (i >>= 1) != 0;) {
            assert(parent->children[0]);
            parent = parent->children[0];
        }
        parent->children[0] = node;
    } else {
        // Insert to the bottom (non-full) level
        uint_fast8_t depth = TWULog2(numNodes);
        uint32_t mask = ((uint32_t)1) << (depth - 1);
        while (mask != 1) {
            if (numNodes & mask) {
                parent = parent->children[1];
            } else {
                parent = parent->children[0];
            }
            assert(parent);
            mask >>= 1;
        }
        parent->children[numNodes & 1] = node;
    }
    node->parent = parent;

    // Restore the heap property
    while ((parent = node->parent) != NULL) {
        if (comparer(node, parent, param) > 0) {
            break;
        }

        // Swap it with its parent node
        TWPQSwapNodeAndParent(node, parent);
    }

    header->numNodes = numNodes;
}

void TWPQRemoveNode(TWPQHeader *header, TWPQNode *node, TWPQComparer comparer, intptr_t param)
{
    assert(header);
    assert(node);
    assert(comparer);

    // Find out the node to swap with
    TWPQNode *last = header->root;
    uint32_t numNodes = header->numNodes;

    assert(last);
    assert(numNodes > 0);

    if (numNodes > 1) {
        uint_fast8_t depth = TWULog2(numNodes);
        uint32_t mask = ((uint32_t)1) << (depth - 1);
        do {
            if (numNodes & mask) {
                last = last->children[1];
            } else {
                last = last->children[0];
            }
            assert(last);
            mask >>= 1;
        } while (mask);
    }

    // Trivial case
    if (last == node) {
        if (node->parent) {
            assert(node->parent->children[0] == node || node->parent->children[1] == node);
            node->parent->children[node->parent->children[1] == node] = NULL;
        }
        header->numNodes = numNodes - 1;
        return;
    }

    // Swap nodes and delete that node
    TWPQNode *t1, *t2, *t3;
    t1 = node->parent;
    t2 = node->parent = last->parent;
    t3 = last->parent = t1;

    t1 = last->children[0] = node->children[0];
    if (t1) {
        t1->parent = last;
    }

    t1 = last->children[1] = node->children[1];
    if (t1) {
        t1->parent = last;
    }

    if (t2) {
        t2->children[t2->children[1] == node] = NULL; // delete
    }
    if (t3) {
        t3->children[t3->children[1] == last] = last;
    }

    // Restore the heap property
    while (true) {
        if (!last->children[0]) {
            break;
        }

        // Swap with the smaller child
        TWPQSwapNodeAndParent(last->children[last->children[1] &&
            comparer(last->children[0], last->children[1], param) > 0], last);
    }
}
