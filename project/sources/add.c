#include "headers.h"

tree* add(tree *root, tree *subroot) {
    if (!root)
        return subroot;

    tree *tmp = makeNode(root->info + subroot->info);
    tmp->right = root;
    tmp->left = subroot;
    return tmp;
}

