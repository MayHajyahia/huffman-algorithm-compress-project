
typedef struct tree {
    int info;
    struct tree* left, *right;
} tree;

tree* makeNode(int info);
int** countFrequence(int (*a)[256], char* s);
int* getMin(int **new);
int searchPath(tree *root, char c, long (*path)[2], int *countBits, int idx);
tree* add(tree *root, tree *subroot);
tree* buildTree(int **n);


