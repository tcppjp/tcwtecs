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
#include "tTWKeyboardInputManager_tecsgen.h"

void
eControl_setEventReceiver(Descriptor(sTWKeyboardEvent) receiver)
{
    if (is_cReceiver_joined() && cReceiver_refer_to_descriptor() == receiver) {
        return;
    }
    if (is_cReceiver_joined()) {
        cReceiver_leave();
    }
    cReceiver_set_descriptor(receiver);
    cReceiver_enter();
}

void
eControl_clearEventReceiver(void)
{
    if (is_cReceiver_joined()) {
        cReceiver_leave();
    }
    cReceiver_set_unjoin();
}

uint8_t
eConrol_isEventReceiver(Descriptor(sTWKeyboardEvent) receiver)
{
    return is_cReceiver_joined() && cReceiver_refer_to_descriptor() == receiver;
}

void
eControl_notifyKeyDown(uint16_t keyCode)
{
    if (is_cReceiver_joined()) {
        cReceiver_keyDown(keyCode);
    }
}

void
eControl_notifyKeyUp(uint16_t keyCode)
{
    if (is_cReceiver_joined()) {
        cReceiver_keyUp(keyCode);
    }
}
