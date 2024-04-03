#include "events.h"
#include "bedtjed.h"
#include "config.h"
#include <stdio.h>
#include <stdlib.h>

bool is_key_valid_event(uint16_t ch) {
    for (size_t i = 0; i < sizeof(VALID_EVENTS); i++)
        if (ch == VALID_EVENTS[i])
            return true;
    return false;
}

EVENT(cursor_up) { move_cursor(state, D_UP); }

EVENT(cursor_down) { move_cursor(state, D_DOWN); }

EVENT(cursor_left) { move_cursor(state, D_LEFT); }

EVENT(cursor_right) { move_cursor(state, D_RIGHT); }

EVENT(insult_user) {
    if (INSULT_ME) {
        char insult_buf[100];
        size_t insult_i = (size_t)(rand() % LEN(INSULTS));
        snprintf(insult_buf, sizeof(insult_buf), "invalid command (%s)",
                 INSULTS[insult_i]);

        notify(insult_buf);
    } else {
        notify("invalid command");
    }
}

EVENT(quit) { state->should_exit = true; }
