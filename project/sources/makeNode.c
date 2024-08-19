
#include "headers.h"
#include <stdlib.h>

tree* makeNode(int info) {
    tree *tmp = (tree*)malloc(sizeof(tree));
    tmp->info = info;
    tmp->left = tmp->right = NULL;
    return tmp;
}

