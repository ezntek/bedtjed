#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "bedtjed.h"
#include "config.h"
#include "events.h"

static void Buffer_open_file(Buffer* b, const char* filename);
static void Buffer_read_file_pre(Buffer* b);
static void Buffer_read_file(Buffer* b);
static void draw_line(State* state, Line* line, size_t rel_lineno);
static void handle_event(State* state, const struct tb_event* evt);

void init(State* state, const char* filename) {
    tb_init();
    srand(time(NULL));
    state->bufs[0] = Buffer_new(filename);
    state->curr_buf_index = 0;
    state->curr_buf = &state->bufs[state->curr_buf_index];
    state->curr_buf->viewport = Buffer_View_new(state->curr_buf);
}

void deinit(State* state) {
    for (size_t i = 0; i < state->nbufs; i++)
        Buffer_deinit(&state->bufs[i]);
    tb_shutdown();

    puts("\033[1mBye\033[0m");
}

void loop(State* state) {
    struct tb_event evt = {0};
    for (size_t currline = 0; currline < state->curr_buf->viewport.lines_len;
         currline++) {
        Line* linep = &state->curr_buf->viewport.lines[currline];
        draw_line(state, linep, currline);
    }

    tb_present();
    tb_poll_event(&evt);
    handle_event(state, &evt);
}

void clear_bar(void) {
    for (size_t i = 0; i < tb_width(); i++)
        tb_set_cell(i, BAR_Y, ' ', TB_DEFAULT, TB_DEFAULT);
}

void notify(const char* msg) {
    clear_bar();
    tb_print(0, BAR_Y, TB_DEFAULT, TB_DEFAULT, msg);
}

void move_cursor(State* state, Direction dr) {
    // TODO: implement
}

size_t get_drawable_height(void) { return (size_t)(tb_height() - 2); }

size_t get_drawable_width(void) {
    return tb_width();
} // FIXME: when h-scrolling is added

static void handle_event(State* state, const struct tb_event* evt) {
    // these are for "keys" (special keys)
    if (!is_key_valid_event(evt->key)) {
        event_insult_user(state, NULL);
    } else {
        clear_bar();
        EVENT_FUNC[EVENT_KEY[evt->key]](state, evt);
    }
}

static void draw_line(State* state, Line* line, size_t rel_lineno) {
    size_t col = 0;
    char currch;

    while ((currch = line->line[col]) != '\n' && col < line->len) {
        if (currch == '\t') {
            col += TAB_SPACES;
            continue;
        }

        if (currch == ' ') {
            col++;
            continue;
        }

        tb_set_cell(col, rel_lineno, currch, TB_DEFAULT, TB_DEFAULT);

        col++;
    }
}

Buffer Buffer_new(const char* filename) {
    Buffer res = {0};

    Buffer_open_file(&res, filename);
    Buffer_read_file(&res);

    res.filename = filename;
    res.cursor = (Cursor){
        .cursor_pos = (Position){0, 0},
        .char_under_cursor = res.lines[0].line[0],
    };

    return res;
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

static void Buffer_open_file(Buffer* b, const char* filename) {
    b->filep = fopen(filename, "r+");

    if (b->filep == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
}

static void Buffer_read_file_pre(Buffer* b) {
    b->lines_cap = 8;
    b->lines_len = 0;
    b->lines = calloc(b->lines_cap, sizeof(Line));

    if (b->lines == NULL) {
        perror("calloc");
        Buffer_deinit(b);
        exit(EXIT_FAILURE);
    }
}

static void Buffer_read_file(Buffer* b) {
    char* currline = NULL;
    size_t chars_read = 0;

    Buffer_read_file_pre(b);

    while ((chars_read = getline(&currline, &(size_t){0}, b->filep)) != -1) {
        if (b->lines_len + 1 > b->lines_cap) {
            b->lines = realloc(b->lines, (b->lines_cap *= 2) * sizeof(Line));

            if (b->lines == NULL) {
                perror("realloc");
                Buffer_deinit(b);
                exit(EXIT_FAILURE);
            }
        }

        size_t line_cap = 1;
        size_t line_len = chars_read;
        char* new_line = NULL;

        if (line_len == 0)
            break;

        while (line_cap < line_len)
            line_cap *= 4;

        new_line = malloc(line_cap);

        if (new_line == NULL) {
            perror("malloc");
            Buffer_deinit(b);
            exit(EXIT_FAILURE);
        }

        strncpy(new_line, currline, line_len);

        b->lines[b->lines_len++] = (Line){
            .cap = line_cap,
            .len = line_len,
            .line = new_line,
        };
    }
}

Buffer_View Buffer_View_new(Buffer* b) {
    size_t h = get_drawable_height();
    size_t end_line = b->lines_len;
    if (b->lines_len > h)
        end_line = h;

    // (h = get_drawable_height()) ? h < b->lines_len : b->lines_len;

    return (Buffer_View){
        .begin_line = 0,
        .end_line = end_line,
        .lines = b->lines,
        .lines_len = end_line,
    };
}
