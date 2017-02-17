/*
 * Copyright (C) 2017 Tomoaki Kawada
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
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
