#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "headers.h"
#define UNIX
void Compress(char *s) {
    FILE *f = fopen(s, "rt");
    if (!f) {
        perror("ok");
        return;
    }
    char *ss = (char*)malloc(sizeof(char) * (strlen(s) + 5));
    strcpy(ss, s);
    strcat(ss, ".huf");
    FILE *new = fopen(ss, "wb");
    if (!new) {
        fclose(f);
        perror("cannot create the file ");
        return;
    }
    printf("compressing ...\n");
    fputc((char)67, new); // 0
    fputc((char)147, new); // 1

    #ifdef UNIX
        fputc((char)0, new);
    #endif
    #ifdef WINDOWS
        fputc(1, new);
    #endif
    #ifdef MACOS
        fputc(2, new);
    #endif
    #ifdef OTHER
        fputc(255, new);
    #endif

    fseek(f, 0, SEEK_END);
    int fs = ftell(f);
    fwrite(&fs, sizeof(fs), 1, new); // 3-6

    int a[256] = {0};
    int** n = countFrequence(&a, s);
    if (!n) {
        fclose(f);
        fclose(new);
        free(ss);
        return;
    }
    short chc=0;
    for(int i=0;i<256;i++)
      if(n[i][1]!=0)
        chc++;
    fseek(new,11,SEEK_SET);
    fwrite(&chc,sizeof(chc),1,new);//11-12
    for(int i=0;i<256;i++)
      if(n[i][1]!=0){
        fwrite(&n[i][0],sizeof(char),1,new);
        fwrite(&n[i][1],sizeof(int),1,new);
      }
    tree *root = buildTree(n);
    char content;
    int i;
    char cb = 0;
    fseek(f, 0, SEEK_SET);
    for (char c = fgetc(f); c != EOF; c = fgetc(f)) {
        long path[2] = {0};
        int countBits = 0;
        searchPath(root, c, &path, &countBits, 0);
        for (i = 0; i < countBits; i++) {
            if (cb == 8) {
                fwrite(&content, sizeof(char), 1, new);
                cb = content = 0;
            }
            content = (content << 1) | (path[(i + 1) / 64] & 1);
            path[(i + 1) / 64] >>= 1;
            cb++;
        }
    }
    if (cb > 0) {
        content = content << (8 - cb);
        fwrite(&content, sizeof(char), 1, new);
    }
    fseek(new, 0, SEEK_END);
    int ns = ftell(new);
    fseek(new, 7, SEEK_SET); // 7-10
    fwrite(&ns, sizeof(ns), 1, new);
    fclose(f);
    fclose(new);
}

void unCompress(char *s) {
    char *ss = (char *)malloc(sizeof(char) * (strlen(s) - 4));
    int p;
    for (p = 0; p < strlen(s) - 4; p++)
        ss[p] = s[p];
    ss[p] = '\0';
    FILE *f = fopen(s, "rb");
    if (!f) {
        perror("unreachable file");
        free(ss);
        return;
    }
    if (fgetc(f) != 67 || fgetc(f) != 147) {
        fprintf(stderr, "The file type is not Huffman file");
        fclose(f);
        free(ss);
        return;
    }
    FILE *new = fopen(ss, "wt");
    if (!new) {
        perror("cannot create the file");
        fclose(f);
        free(ss);
        return;
    }
    printf("uncompressing ...\n");
   
    fseek(f, 3, SEEK_SET);
    int size;
    fread(&size, sizeof(size), 1, f);
    short chc=0;
    fseek(f,11,SEEK_SET);
    fread(&chc,sizeof(chc),1,f);
    int **n=(int **)malloc(256*sizeof(int *));
    for(int i=0;i<256;i++){
       n[i]=(int*)malloc(2 * sizeof(int));
       n[i][0]=0;
       n[i][1]=0;
    }
    for(int i=0;i<chc;i++){
       char c=0;
       int occur=0;
       fread(&c,sizeof(c),1,f);
       fread(&occur,sizeof(occur),1,f);
       n[c][0]=(int)c;
       n[c][1]=occur;
    }
    tree *root=buildTree(n);
    char c;
    fread(&c, sizeof(c), 1, f);
    int bitIndex = 0;
    for (int i = 0; i < size; i++) {
        tree *rt = root;
        while (rt->left && rt->right) {
            if ((c & 0x80) == 0)
                rt = rt->left;
            else
                rt = rt->right;
            c <<= 1;
            bitIndex++;

            if (bitIndex % 8 == 0)
                fread(&c, sizeof(c), 1, f);
        }
        if (!rt->left && !rt->right)
            fputc((char)(rt->info), new);
    }
    fclose(f);
    fclose(new);
    free(ss);
}
void fileInfo(char *s) {
    FILE *f = fopen(s, "rb");  
    if (!f) {
        perror("Cannot open file");
        return;
    }
    if (fgetc(f) != 67 || fgetc(f) != 147) {
        printf("The file type is not a Huffman file\n");
        fclose(f);
        return;
    }

    printf("About %s:\n", s);
    int fs = 0;
    fseek(f, 3, SEEK_SET);
    fread(&fs, sizeof(int), 1, f);
    printf("Original file size: %d bytes\n", fs);
    int ns = 0;
    fseek(f, 7, SEEK_SET);
    fread(&ns, sizeof(int), 1, f);
    printf("File size after compression: %d bytes\n", ns);
    char os;
    fseek(f, 2, SEEK_SET); 
    fread(&os, sizeof(char), 1, f);

    const char *os_str;
    switch ((int)os) {
        case 0:
            os_str = "Unix";
            break;
        case 1:
            os_str = "Windows";
            break;
        case 2:
            os_str = "Mac";
            break;
        default:
            os_str = "Other (not Unix, Windows, or Mac)";
            break;
    }
    printf("Operating system the file was created on: %s\n", os_str);

    fclose(f);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <-d|-c> <filename>\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "-d") == 0)
        unCompress(argv[2]);
    else
        if (strcmp(argv[1], "-c") == 0)
          Compress(argv[2]);
    else
        if (strcmp(argv[1], "-i") == 0)
          fileInfo(argv[2]);
    else {
        fprintf(stderr, "Invalid option <%s>\n", argv[1]);
        return 1;
    }

    return 0;
}

