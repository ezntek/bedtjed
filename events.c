#include "events.h"
#include "bedtjed.h"
#include <stdio.h>

EVENT(cursor_up) { move_cursor(state, D_UP); }

EVENT(cursor_down) { move_cursor(state, D_DOWN); }

EVENT(cursor_left) { move_cursor(state, D_LEFT); }

EVENT(cursor_right) { move_cursor(state, D_RIGHT); }

EVENT(insult_user) { printf("not implemented"); }

EVENT(quit) { state->should_exit = true; }
