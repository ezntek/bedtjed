#pragma once

#include "config.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

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
    FILE* filep;
    const char* filename;
    Cursor cursor;

    Line* lines;
    size_t lines_len;
    size_t lines_cap;
} Buffer;

typedef struct {
    Buffer bufs[MAX_BUFS];
    size_t curr_buf_index;
    size_t nbufs;
    Buffer* curr_buf;

} State;

typedef struct tb_event Tb_Event;
typedef void (*EventHandler)(State* state, Tb_Event* evt);

void init(State* state, const char* filename);
void deinit(State* state);
void read_file(State* state);

Buffer Buffer_new(const char* filename);
void Buffer_deinit(Buffer* b);
