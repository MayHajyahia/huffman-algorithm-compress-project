#include "headers.h"
#include <stdlib.h>

int* getMin(int **new) {
    int* min = NULL;
    for (int i = 0; i < 256; i++)
        if ((min == NULL && (new)[i][1] != 0) || (min && (new)[i][1] <= min[1] && (new)[i][1] != 0))
            min = (new)[i];
    return min;
}

