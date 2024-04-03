#pragma once

#include <stdbool.h>
#include <stddef.h>

#include "config.h"

#define LEN(arr) sizeof(arr) / sizeof(*arr)
#define BAR_Y    (tb_height() - 1)

typedef enum {
    D_UP,
    D_DOWN,
    D_LEFT,
    D_RIGHT,
} Direction;

typedef struct {
    uint32_t x;
    uint32_t y;
} Position;

typedef struct {
    char* line;
    size_t len;
    size_t cap;
} Line;

typedef struct {
    Position cursor_pos;
    char char_under_cursor;
} Cursor;

typedef struct {
    Line* lines;
    size_t lines_len;
    size_t begin_line;
    size_t end_line;
} Buffer_View;

typedef struct {
    FILE* filep;
    const char* filename;
    Cursor cursor;

    Line* lines;
    size_t lines_len;
    size_t lines_cap;

    Buffer_View viewport;
} Buffer;

typedef struct {
    Buffer bufs[MAX_BUFS];
    size_t curr_buf_index;
    size_t nbufs;
    Buffer* curr_buf;
    bool should_exit;
} State;

typedef void (*EventHandler)(State* state, const struct tb_event* evt);

size_t get_drawable_height(void);
size_t get_drawable_width(void);

void init(State* state, const char* filename);
void deinit(State* state);
void loop(State* state);
void move_cursor(State* state, Direction dr);
void notify(const char* msg);
void clear_bar(void);

Buffer Buffer_new(const char* filename);
void Buffer_deinit(Buffer* b);

Buffer_View Buffer_View_new(Buffer* b);
