#ifndef H_NODE
#define H_NODE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct Node {
  char character;
  int frequency;
  char *code;
  struct Node *left;
  struct Node *right;
} Node;

Node *create_node(char character, int frequency);
void count_frequencies(FILE *fp, int *frequencies);
Node *find_min_node(Node **list, int size);
Node *build_tree(int *frequencies);
void compress_file(FILE *inputFp, FILE *outputFp, Node *root);
void assign_codes(Node *node, char *code);
void free_tree(Node *root);

#endif