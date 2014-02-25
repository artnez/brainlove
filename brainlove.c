#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MEMORY 30000

typedef struct {
    FILE *fp;  // stream to parse
    char *mem; // memory
    char *ptr; // "the pointer"
    int skd;   // skip depth
} State;

void run(State *state);

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

    run(state);

    free(state->mem);
    free(state);
    fclose(fp);
    return 0;
}

void run(State *state) {
    char tok;
    int pos;

    while ((tok = getc(state->fp)) != EOF) {
        if (state->skd) {
            if (tok == '[') state->skd ++;
            if (tok == ']') state->skd --;
            continue;
        }
        if (tok == '[') {
            if (!*state->ptr) {
                state->skd = 1;
                continue;
            }
            pos = ftell(state->fp);
            do {
                fseek(state->fp, pos, 0);
                run(state);
            } while (*state->ptr);
            continue;
        }
        switch (tok) {
            case '>': state->ptr ++; break;
            case '<': state->ptr --; break;
            case '+': (*state->ptr) ++; break;
            case '-': (*state->ptr) --; break;
            case '.': putchar(*state->ptr); break;
            case ',': *state->ptr = getchar(); break;
            case ']': return;
        }
    }
}
