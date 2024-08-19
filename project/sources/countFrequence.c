
#include "headers.h"
#include <stdio.h>
#include <stdlib.h>

int** countFrequence(int (*a)[256], char* s) {
    FILE *f = fopen(s, "rt");
    char c;
    while ((c = getc(f)) != EOF)
        (*a)[(int)c]++;

    int **new = (int**)malloc(256 * sizeof(int*));
    for (int i = 0; i < 256; i++) {
        new[i] = (int*)malloc(2 * sizeof(int));
        new[i][0] = i;
        new[i][1] = (*a)[i];
    }
    return new;
}

