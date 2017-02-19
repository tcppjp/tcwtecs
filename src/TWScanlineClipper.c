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

#include "TWPrivate.h"

static TWScanlineClipperNode *
TWScanlineClipperAllocateNode(TWScanlineClipperState *state)
{
    TWScanlineClipperNode *result;
    if (state->freelist) {
        result = state->freelist;
        state->freelist = state->freelist->next;
        return result;
    }
    if (state->continuousFreeZoneSize) {
        result = state->continuousFreeZoneBegin;
        ++state->continuousFreeZoneBegin;
        --state->continuousFreeZoneSize;
        return result;
    }
    return NULL;
}

static void
TWScanlineClipperFreeNode(TWScanlineClipperState *state, TWScanlineClipperNode *node)
{
    node->freelist.next = state->freelist;
    state->freelist = node;
}

void
TWScanlineClipperInitialize(TWScanlineClipperState *state, TWScanlineClipperNode *heap, uint_fast16_t heapSize)
{
    state->continuousFreeZoneBegin = heap;
    state->freelist = NULL;
    state->continuousFreeZoneSize = heapSize;
}

void
TWScanlineClipperSetClippingRect(TWScanlineClipperState *state, const TWRect *rect)
{
    TWScanlineClipperNode *span = TWScanlineClipperAllocateNode(state);
    span->span.skip = rect->x;
    span->span.wodth = rect->w;
    span->span.nextIndexOffset = 0;

    TWScanlineClipperNode *row = TWScanlineClipperAllocateNode(state);
    row->row.height = rect->h;
    row->row.nextIndexOffset = 0;
    row->row.spanIndexOffset = span - row;

    state->firstRow = row;
    state->top = rect->y;
    state->height = rect->h;
    state->width = rect->x + rect->w;
}

static TWScanlineClipperNode *
TWScanlineClipperCloneRow(TWScanlineClipperState *state, TWScanlineClipperNode *row)
{
    TWScanlineClipperNode *clonedRow = TWScanlineClipperAllocateNode(state);
    if (!clonedRow) {
        // Out of nodes
        return NULL;
    }
    if (row->row.spanIndexOffset) {
        TWScanlineClipperNode *span = row + row->row.spanIndexOffset;
        TWScanlineClipperNode **nextFieldFromPreviousSpan = NULL;
        do {
            TWScanlineClipperNode *clonedSpan = TWScanlineClipperAllocateNode(state);
            if (!clonedRow) {
                // Out of nodes
                return NULL;
            }
            if (nextFieldFromPreviousSpan) {
                *nextFieldFromPreviousSpan = clonedSpan;
            } else {
                clonedRow->row.spanIndexOffset = clonedSpan - clonedRow;
            }
            clonedRow->row.skip = span->row.skip;
            clonedRow->row.width = span->row.width;
            nextFieldFromPreviousSpan = &clonedRow->row.next;
            span = span->next;
        } while (span);
        *nextFieldFromPreviousSpan = NULL;
    } else {
        clonedRow->row.spanIndexOffset = 0;
    }
    return clonedRow;
}

static uint8_t
TWScanlineClipperSubtractSpan(TWScanlineClipperState *state, TWScanlineClipperNode *row,
    uint_fast16_t inSpanX, uint_fast16_t inSpanWidth)
{
    if (!row->row.spanIndexOffset) {
        // Already empty
        return 1;
    }

    TWScanlineClipperNode *first = row + row->row.spanIndexOffset;

    TWScanlineClipperNode *prev = NULL;
    TWScanlineClipperNode *span = first;

    // Quickly skip spans in the left side of the input span
    while (span) {
        uint_fast16_t rightEdge = span->span.skip + span->span.width;
        if (rightEdge > inSpanX) {
            break;
        }
        //   span:      XXXXX
        //  input:            IIIIIIIII ...
        inSpanX -= rightEdge;
        prev = span;
        span = span->span.next;
    }

    // Clip spans
    while (span) {
        // Now inSpanX is relative to `span`'s left margin (pre-`skip`) edge
        if (span->span.skip > inSpanX) {
            //  span:       XXXXXXXXXXXX
            //  input:  IIIIIII ...
            // Consume the left part of the input span
            uint_fast16_t margin = span->span.skip - inSpanX;
            inSpanWidth -= margin;
            inSpanX = 0;
            // Now:
            //  span:       XXXXXXXXXXXX
            //  input:      IIIIIII ...
        } else {
            //  span:       XXXXXXXXXXXX
            //  input:        IIIIIII ...
            inSpanX -= span->span.skip;
        }

        // Now inSpanX is relative to `span`'s left edge
        uint_fast16_t inSpanRightEdge = inSpanX + inSpanWidth;

        if (inSpanRightEdge >= span->span.width) {
            if (inSpanX > 0) {
                //   span:      Z XXXXX
                //  input:          IIIIIIIII ...
                assert(span->span.width > inSpanX);
                uint_fast16_t overlap = span->span.width - inSpanX;
                span->span.width -= overlap;
                if (span->span.next) {
                    span->span.next->span.skip += overlap;
                }
                inSpanWidth -= overlap;
                prev = span;
                span = span->span.next;
            } else {
                //   span:      Z   XXXXX
                //  input:          IIIIIIIII ...
                if (span->span.next) {
                    span->span.next->span.skip += span->span.width + span->span.skip;
                    inSpanX += span->span.skip;
                }
                if (prev) {
                    prev->span.next = span->span.next;
                } else {
                    if (span->span.next) {
                        row->row.spanIndexOffset = span->span.next - row;
                    } else {
                        row->row.spanIndexOffset = 0;
                    }
                }
                TWScanlineClipperFreeNode(span);
                span = span->span.next;
            }
        } else if (inSpanRightEdge == span->span.width) {
            if (inSpanX > 0) {
                //   span:      Z  XXXXXXX
                //  input:            IIII
                assert(span->span.width > inSpanX);
                span->span.width -= inSpanWidth;
                if (span->span.next) {
                    span->span.next->span.skip += inSpanWidth;
                }
                break;
            } else {
                //   span:      Z  XXXXXXX
                //  input:         IIIIIII
                if (span->span.next) {
                    span->span.next->span.skip += span->span.width + span->span.skip;
                }
                if (prev) {
                    prev->span.next = span->span.next;
                } else {
                    if (span->span.next) {
                        row->row.spanIndexOffset = span->span.next - row;
                    } else {
                        row->row.spanIndexOffset = 0;
                    }
                }
                TWScanlineClipperFreeNode(span);
                break;
            }
        } else {
            if (inSpanX > 0) {
                //   span:      Z  XXXXXXXXX
                //  input:            IIII
                TWScanlineClipperNode *newSpan = TWScanlineClipperAllocateNode(state);
                if (!newSpan) {
                    // Out of nodes
                    return 0;
                }
                newSpan->span.next = span->span.next;
                newSpan->span.width = span->span.width - inSpanRightEdge;
                newSpan->span.skip = inSpanWidth;
                span->span.width = inSpanX;
                span->span.next = newSpan;
                break;
            } else {
                //   span:      Z  XXXXXXXXXX
                //  input:         IIIIIII
                span->span.width -= inSpanWidth;
                span->span.skip += inSpanWidth;
                break;
            }
        }
        // Now inSpanX is relative to `span`'s left margin (pre-`skip`) edge
    }

    return 1;
}

void
TWScanlineClipperSubtractClippingRect(TWScanlineClipperState *state, const TWRect *rect)
{
    uint_fast16_t skippedLines = rect->y;
    uint_fast16_t remainingLines = rect->h;
    if (skippedLines < state->top) {
        uint_fast16_t marginLines = state->top - skip;
        if (marginLines > remainingLines) {
            return;
        }
        remainingLines -= marginLines;
        skippedLines = 0;
    } else {
        skippedLines -= state->top;
    }
    if (skippedLines >= state->height) {
        return;
    }

    // Skip rows above the input rect
    TWScanlineClipperNode *prev = NULL;
    TWScanlineClipperNode *prevprev = NULL;
    TWScanlineClipperNode *row = state->firstRow;
    if (skippedLines) {
        assert(row);
        while (skippedLines > row->row.height) {
            skippedLines -= row->row.height;
            prevprev = prev;
            prev = row;
            row = row->row.next;
            assert(row);
        }
    }

    // Skip a completely clipped row if any (note: there can't be more than one subsequent clipped row)
    assert(row);
    if (!row->row.spanIndexOffset) {
        if (skippedLines + remainingLines < row->row.height) {
            return;
        }
        remainingLines -= row->row.height - skippedLines;
        skippedLines = 0;
        prevprev = prev;
        prev = row;
        row = row->row.next;
        assert(row);
    }

    // Split the row at `y = rect->y` if needed
    if (skippedLines > 0) {
        TWScanlineClipperNode *bottomRow = TWScanlineClipperCloneRow(row);
        if (!bottomRow) {
            // Out of nodes
            return;
        }
        bottomRow->row.height = row->row.height - skippedLines;
        row->row.height = skippedLines;
        bottomRow->row.next = row->row.next;
        row->row.next = bottomRow;
        prevprev = prev;
        prev = row;
        row = bottomRow;
        skippedLines = 0;
    }

    TWScanlineClipperNode *next;

    while (remainingLines && row) {
        if (remainingLines < row->row.height) {
            // That's the last row to subtract and we need to split it
            TWScanlineClipperNode *bottomRow = TWScanlineClipperCloneRow(row);
            if (!bottomRow) {
                // Out of nodes
                return;
            }
            bottomRow->row.height = row->row.height - remainingLines;
            row->row.height = remainingLines;
            bottomRow->row.next = row->row.next;
            row->row.next = bottomRow;
        }

        if (!TWScanlineClipperSubtractSpan(state, row, rect->x, rect->w)) {
            // Out of nodes
            return;
        }
        remainingLines -= row->row.height;

        prevprev = prev;

        next = row->row.next;
        if (!row->row.spanIndexOffset) {
            // Row became empty. If the previous row is also empty, merge these two rows into one.
            if (prev == NULL) {
                // This is the first row
                assert(row == state->firstRow);
                state->firstRow = next;
                state->top += row->row.height;
                state->height -= row->row.height;
            FreeThisAndContinue:
                TWScanlineClipperFreeNode(state, row);
                goto MergeOccured;
            } else if (!prev->row.spanIndexOffset) {
                // Merge with the previous row
                prev->row.height += row->row.height;
                prev->row.next = next;
                goto FreeThisAndContinue;
            }
        }

        prev = row;
    MergeOccured:
        row = next;
    }

    // The last processed row `prev` might have became empty
    if (prev && !prev->row.spanIndexOffset) {
        // ... in which case it might need to be merged with the next row
        assert(row == prev->row.next);
        if (row == NULL) {
            // This is the last row
            if (prevprev) {
                assert(prevprev->row.next == prev);
                prevprev->row.next = NULL;
            } else {
                // It's also the first row
                assert(state->firstRow == row);
                state->firstRow = NULL;
            }
            state->height -= prev->row.height;
            TWScanlineClipperFreeNode(prev);
        } else if (!row->row.spanIndexOffset) {
            prev->row.next = row->row.next;
            prev->row.height += row->row.height;
            TWScanlineClipperFreeNode(row);
        }
    }
}

void
TWScanlineClipperInitializeLineScanner(TWScanlineClipperLineScanState *lineScanState)
{
    lineScanState->currentRow = NULL;
}

uint8_t
TWScanlineClipperMoveToLine(const TWScanlineClipperState *state, TWScanlineClipperLineScanState *lineScanState, int_fast16_t startY, uint_fast16_t height)
{
    uint_fast16_t currentRowY;
    TWScanlineClipperNode *currentRow;

    if (startY + height <= state->top || startY >= state->top + state->height) {
        // There's no row there
        return 0;
    }

    uint_fast16_t skippedLines = 0;
    if (startY < state->top) {
        skippedLines = state->top - startY;
        startY += skippedLines;
        height -= skippedLines;
    }

    if (startY + height > state->top + state->height) {
        height = state->top + state->height - startY;
    }

    if (!state->currentRow || line < state->currentRowY) {
        currentRow = state->firstRow;
        currentRowY = state->top;
    } else {
        currentRow = state->currentRow;
        startY -= state->currentRowY;
        currentRowY = state->currentRowY;
    }

    assert(currentRow);

    while (startY >= currentRow->row.height) {
        startY -= currentRow->row.height;
        currentRowY += currentRow->row.height;
        currentRow = currentRow->row.next;
        assert(currentRow);
    }

    if (!currentRow->row.spanIndexOffset) {
        // Empty row
        if (startY + height <= currentRow->row.height) {
            return 0;
        }

        uint_fast16_t overlap = currentRow->row.height - startY;
        skippedLines += overlap;
        startY = 0; height -= overlap;

        currentRowY += currentRow->row.height;
        currentRow = currentRow->row.next;
        assert(currentRow); // The last row cannot be empty
    }

    state->currentRow = currentRow;
    state->currentRowY = currentRowY;
    state->currentRowRemainingLines = currentRow->row.height - startY;
    state->remainingDrawnLines = height;
    state->skip = skippedLines;

    assert(state->currentRowRemainingLines > 0);

    return 1;
}

uint8_t
TWScanlineClipperMoveToNextLine(const TWScanlineClipperState *state, TWScanlineClipperLineScanState *lineScanState)
{
    assert(state->currentRow);
    assert(state->remainingDrawnLines);
    if ((--state->remainingDrawnLines) == 0) {
        return 0;
    }
    if ((--state->currentRowRemainingLines) != 0) {
        state->skip = 0;
        return 1;
    }

    TWScanlineClipperNode *currentRow = state->currentRow;

    // If this is the last row, it should be impossible to reach here since
    // that means `state->remainingDrawnLines` is zero
    assert(currentRow->row.next);

    state->currentRowY += currentRow->row.height;
    currentRow = currentRow->row.next;

    if (!currentRow->row.spanIndexOffset) {
        // Empty row
        if (state->remainingDrawnLines <= currentRow->row.height) {
            return 0;
        }

        state->skip = currentRow->row.height;
        state->remainingDrawnLines -= currentRow->row.height;

        state->currentRowY += currentRow->row.height;
        currentRow = currentRow->row.next;
        assert(currentRow); // The last row cannot be empty
    } else {
        state->skip = 0;
    }

    state->currentRowRemainingLines = currentRow->row.height;
    state->currentRow = currentRow;

    return 1;
}

uint8_t
TWScanlineClipperStartLine(const TWScanlineClipperState *state, TWScanlineClipperSpanScanState *spanScanState,
    int_fast16_t inSpanX, uint_fast16_t inSpanWidth, const TWScanlineClipperLineScanState *lineScanState)
{
    if (inSpanX >= state->width || inSpanX + (int_fast16_t)inSpanWidth <= 0) {
        // Out of range
        goto NothingToDraw;
    }

    uint_fast16_t skipBecauseOfOOR = 0;
    if (inSpanX < 0) {
        skipBecauseOfOOR = -inSpanX;
        inSpanX = 0;
        inSpanWidth -= skipBecauseOfOOR;
    }

    TWScanlineClipperNode *row = lineScanState->currentRow;
    if (!row) {
        // No current row
        goto NothingToDraw;
    }

    if (!row->row.spanIndexOffset) {
        // The row is empty
        goto NothingToDraw;
    }
    TWScanlineClipperNode *span = row + row->row.spanIndexOffset;

    // Quickly skip spans in the left side of the input span
    while (span) {
        uint_fast16_t rightEdge = span->span.skip + span->span.width;
        if (rightEdge > inSpanX) {
            break;
        }
        //   span:      XXXXX
        //  input:            IIIIIIIII ...
        inSpanX -= rightEdge;
        prev = span;
        span = span->span.next;
    }

    if (!span) {
        // Nothing to draw
        goto NothingToDraw;
    }

    if (inSpanX > span->span.skip) {
        //   span:      XXXXX
        //  input:        IIIIIIIII ...
        spanScanState->skip = skipBecauseOfOOR;
        uint_fast16_t rightEdge = span->span.skip + span->span.width;
        uint_fast16_t inSpanRightEdge = inSpanX + inSpanWidth;
        uint_fast16_t overlap = MIN(rightEdge, inSpanRightEdge) - inSpanX;
        spanScanState->width = overlap;
        spanScanState->remainingDrawnPixels = inSpanWidth - overlap;
    } else {
        //   span:      XXXXX
        //  input:     IIIIIIIII ...
        spanScanState->skip = span->span.skip - inSpanX + skipBecauseOfOOR;
        uint_fast16_t rightEdge = span->span.skip + span->span.width;
        uint_fast16_t inSpanRightEdge = inSpanX + inSpanWidth;
        uint_fast16_t overlap = MIN(rightEdge, inSpanRightEdge) - span->span.skip;
        spanScanState->width = overlap;
        spanScanState->remainingDrawnPixels = inSpanRightEdge > rightEdge ? inSpanRightEdge - rightEdge : 0;
    }

    spanScanState->currentSpan = span->span.next;

    return 1;
NothingToDraw:
    return 0;
}

uint8_t
TWScanlineClipperLineAdvance(TWScanlineClipperSpanScanState *spanScanState)
{
    TWScanlineClipperNode *span = spanScanState->currentSpan;
    if (!span) {
        return 0;
    }

    uint_fast16_t remainingDrawnPixels = spanScanState->remainingDrawnPixels;
    if (span->span.skip > remainingDrawnPixels) {
        return 0;
    }

    spanScanState->skip = span->span.skip;
    remainingDrawnPixels -= span->span.skip;
    if (span->span.width > remainingDrawnPixels) {
        spanScanState->width = remainingDrawnPixels;
        spanScanState->currentSpan = NULL;
    } else {
        remainingDrawnPixels -= span->span.width;
        spanScanState->currentSpan = span->span.next;
        spanScanState->remainingDrawnPixels = remainingDrawnPixels;
    }
    return 1;
}

