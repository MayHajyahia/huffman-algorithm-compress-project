#include "headers.h"

int searchPath(tree *root, char c, long (*path)[2], int *countBits, int idx) {
    if (!root)
        return 0;

    if (!root->left && !root->right && root->info == c)
        return 1;

    if (root->left) {
        if (searchPath(root->left, c, path, countBits, *countBits / 64) == 1) {
            (*path)[idx] = ((*path)[idx] << 1) | 0;
            (*countBits)++;
            return 1;
        }
    }

    if (root->right) {
        if (searchPath(root->right, c, path, countBits, *countBits / 64) == 1) {
            (*path)[idx] = ((*path)[idx] << 1) | 1;
            (*countBits)++;
            return 1;
        }
    }

    return 0;
}

