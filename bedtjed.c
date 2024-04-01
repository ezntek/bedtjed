#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bedtjed.h"
#include "config.h"

static void Buffer_open_file(Buffer* b, const char* filename);
static void Buffer_read_file_pre(Buffer* b);
static void Buffer_read_file(Buffer* b);

void init(State* state, const char* filename) {
    state->bufs[0] = Buffer_new(filename);
    state->curr_buf_index = 0;
    state->curr_buf = &state->bufs[state->curr_buf_index];
}

Buffer Buffer_new(const char* filename) {
    Buffer res = {0};

    Buffer_open_file(&res, filename);
    Buffer_read_file_pre(&res);
    Buffer_read_file(&res);

    res.filename = filename;
    res.cursor = (Cursor){
        .cursor_pos = (Position){0, 0},
        .char_under_cursor = res.lines[0].line[0],
    };

    return res;
}

void Buffer_open_file(Buffer* b, const char* filename) {
    b->filep = fopen(filename, "r+");

    if (b->filep == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
}

void Buffer_read_file_pre(Buffer* b) {
    b->lines_cap = 8;
    b->lines_len = 0;
    b->lines = calloc(b->lines_cap, sizeof(Line));

    if (b->lines == NULL) {
        perror("calloc");
        Buffer_deinit(b);
        exit(EXIT_FAILURE);
    }
}

void Buffer_read_file(Buffer* b) {
    char* currline = NULL;
    size_t chars_read = 0;

    while ((chars_read = getline(&currline, &(size_t){0}, b->filep)) > 0) {
        if (b->lines_len + 1 > b->lines_cap) {
            b->lines = realloc(b->lines, (b->lines_cap *= 2) * sizeof(Line));

            if (b->lines == NULL) {
                perror("realloc");
                Buffer_deinit(b);
                exit(EXIT_FAILURE);
            }
        }

        size_t line_cap = 0;
        size_t line_len = strlen(currline - 1); // delete \n later
        char* new_line = NULL;

        while (line_len > line_cap)
            line_cap *= 4;

        new_line = calloc(line_cap, 1);

        if (new_line == NULL) {
            perror("malloc");
            Buffer_deinit(b);
            exit(EXIT_FAILURE);
        }

        strcpy(new_line, currline);
        currline[line_len - 1] = '\0'; // get rid of newline entirely

        b->lines[b->lines_len++] = (Line){
            .cap = line_cap,
            .len = line_len,
            .line = new_line,
        };
    }
}

void Buffer_deinit(Buffer* b) {
    if (b->lines != NULL) {
        for (size_t i = 0; i < b->lines_len; i++)
            free(b->lines[i].line);
        free(b->lines);
    }

    if (b->filep != NULL)
        fclose(b->filep);
}
