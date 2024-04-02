#pragma once

#include "bedtjed.h"

#define EVENT(name) void event_##name(State* state, const struct tb_event* evt)

typedef enum {
    EVENT_CURSOR_DOWN,
    EVENT_CURSOR_UP,
    EVENT_CURSOR_LEFT,
    EVENT_CURSOR_RIGHT,
    EVENT_INSULT_USER,
    EVENT_QUIT,
} EventKind;

EVENT(cursor_up);
EVENT(cursor_down);
EVENT(cursor_left);
EVENT(cursor_right);
EVENT(insult_user);
EVENT(quit);

const EventHandler EVENT_FUNC[] = {
    [EVENT_CURSOR_UP] = &event_cursor_up,
    [EVENT_CURSOR_DOWN] = &event_cursor_down,
    [EVENT_CURSOR_LEFT] = &event_cursor_left,
    [EVENT_CURSOR_RIGHT] = &event_cursor_right,
    [EVENT_INSULT_USER] = &event_insult_user,
    [EVENT_QUIT] = &event_quit,
};

const uint32_t VALID_EVENTS[5] = {
    TB_KEY_ESC,        TB_KEY_ARROW_RIGHT, TB_KEY_ARROW_LEFT,
    TB_KEY_ARROW_DOWN, TB_KEY_ARROW_UP,
};

const EventKind EVENT_KEY[] = {
    [TB_KEY_ESC] = EVENT_QUIT,
    [TB_KEY_ARROW_UP] = EVENT_CURSOR_UP,
    [TB_KEY_ARROW_DOWN] = EVENT_CURSOR_DOWN,
    [TB_KEY_ARROW_LEFT] = EVENT_CURSOR_LEFT,
    [TB_KEY_ARROW_RIGHT] = EVENT_CURSOR_RIGHT,
};
