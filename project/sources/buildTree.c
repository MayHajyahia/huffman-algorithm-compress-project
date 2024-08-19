#include<stdio.h>
#include "headers.h"

tree* buildTree(int **n) {
    tree *root = NULL;

    for (int *min1 = getMin(n); min1 && min1[1] != 0; min1 = getMin(n)) {
        int m1[] = {min1[0], min1[1]};
        n[min1[0]][1] = 0;

        int *min2 = getMin(n);
        if (!min2 || min2[1] == 0) {
            tree *subroot = makeNode(m1[0]);
            int info = root->info;
            root = add(root, subroot);
            root->info = info + m1[1];
            break;
        }

        int m2[] = {min2[0], min2[1]};
        n[min2[0]][1] = 0;

        tree *subroot = makeNode(m1[1] + m2[1]);
        subroot->left = makeNode(m1[0]);
        subroot->right = makeNode(m2[0]);
        root = add(root, subroot);
    }

    return root;
}

