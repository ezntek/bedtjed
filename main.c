#include <stdio.h>
#include <stdlib.h>

#include "bedtjed.h"

int main(int argc, char** argv) {
    if (argc == 1) {
        puts("must supply filename (for now)!");
        printf("usage: %s [filename]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    State state = {0};
    init(&state, argv[1]);
    while (!state.should_exit)
        loop(&state);
    deinit(&state);
    return 0;
}
