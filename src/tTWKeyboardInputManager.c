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

// tTWKeyboardInputManager_tecsgen.h requires `assert` implicitly :(
#include <assert.h>

#include "tTWKeyboardInputManager_tecsgen.h"

void
eControl_setEventReceiver(CELLIDX idx, Descriptor(sTWKeyboardEvent) receiver)
{
    CELLCB  *p_cellcb = GET_CELLCB(idx);

    if (is_cReceiver_joined() && TWTecsDescriptorEquals(sTWKeyboardEvent, cReceiver_refer_to_descriptor(), receiver)) {
        return;
    }
    if (is_cReceiver_joined()) {
        cReceiver_leave();
    }
    cReceiver_set_descriptor(receiver);
    cReceiver_enter();
}

void
eControl_clearEventReceiver(CELLIDX idx)
{
    CELLCB  *p_cellcb = GET_CELLCB(idx);

    if (is_cReceiver_joined()) {
        cReceiver_leave();
    }
    cReceiver_unjoin();
}

uint8_t
eControl_isEventReceiver(CELLIDX idx, Descriptor(sTWKeyboardEvent) receiver)
{
    CELLCB  *p_cellcb = GET_CELLCB(idx);

    return is_cReceiver_joined() && TWTecsDescriptorEquals(sTWKeyboardEvent, cReceiver_refer_to_descriptor(), receiver);
}

void
eDriverEvent_notifyKeyDown(CELLIDX idx, uint16_t keyCode)
{
    CELLCB  *p_cellcb = GET_CELLCB(idx);

    if (is_cReceiver_joined()) {
        cReceiver_keyDown(keyCode);
    }
}

void
eDriverEvent_notifyKeyUp(CELLIDX idx, uint16_t keyCode)
{
    CELLCB  *p_cellcb = GET_CELLCB(idx);

    if (is_cReceiver_joined()) {
        cReceiver_keyUp(keyCode);
    }
}
