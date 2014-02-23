#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MEMORY 1048576

typedef struct {
    FILE *fp;  // stream to parse
    char *mem; // memory
    char *ptr; // "the pointer"
    int skd;   // skip depth
} State;

void call(State *state);

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: brainlove SOURCE_FILE\n");
        exit(1);
    }

    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        fprintf(stderr, "%s: %s\n", strerror(errno), argv[1]);
        exit(1);
    }

    State *state = malloc(sizeof(State));
    state->fp = fp;
    state->mem = calloc(MAX_MEMORY, 1);
    state->ptr = state->mem;
    state->skd = 0;

    call(state);

    free(state->mem);
    free(state);
    fclose(fp);
    return 0;
}

void call(State *state) {
    char tok;
    int pos;

    while ((tok = getc(state->fp)) != EOF) {
        // Skip depth is active. Do nothing except nested loop handling.
        if (state->skd) {
            if (tok == '[') state->skd ++;
            if (tok == ']') state->skd --;
            continue;
        }

        // Handle a loop that will cycle at least one time.
        if (tok == '[' && *state->ptr) {
            pos = ftell(state->fp);
            while (*state->ptr) {
                fseek(state->fp, pos, 0);
                call(state);
            }
            continue;
        }

        // Handle a loop that will not cycle. Enables skip depth.
        if (tok == '[') {
            state->skd = 1;
            continue;
        }

        // Ending a loop should return back to the parent call stack.
        if (tok == ']') {
            return;
        }

        // Handle pointer arithmetic and I/O.
        switch (tok) {
            case '>': state->ptr ++; break;
            case '<': state->ptr --; break;
            case '+': (*state->ptr) ++; break;
            case '-': (*state->ptr) --; break;
            case '.': putchar(*state->ptr); break;
            case ',': *state->ptr = getchar(); break;
        }
    }
}
