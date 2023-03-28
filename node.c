#include "node.h"

Node *create_node(char character, int frequency)
{
  Node *newNode = malloc(sizeof(*newNode));
  newNode->character = character;
  newNode->frequency = frequency;
  newNode->left = newNode->right = NULL;
  newNode->code = NULL;
  return newNode;
}

void count_frequencies(FILE *fp, int *frequencies)
{
  int character;
  while ((character = fgetc(fp)) != EOF)
  {
    if (isalpha(character))
    {
      frequencies[tolower(character) - 'a']++;
    }
  }
}

Node *find_min_node(Node **list, int size)
{
  int i;
  int minIndex = 0;

  for (i = 1; i < size; i++)
  {
    if (list[i]->frequency < list[minIndex]->frequency)
      minIndex = i;
  }

  Node *minNode = list[minIndex];

  for (i = minIndex; i < size - 1; i++)
  {
    list[i] = list[i+1];
  }
  list[size-1] = minNode;

  return minNode;
}

Node *build_tree(int *frequencies)
{
  Node **nodes = malloc(26 * sizeof(Node*));
  int i, size = 0;

  for (i = 0; i < 26; i++)
  {
    if (frequencies[i] > 0)
    {
      nodes[size] = create_node(i+'a', frequencies[i]);
      size++;
    }
  }

  while (size > 1)
  {
    Node *left = find_min_node(nodes, size--);
    Node *right = find_min_node(nodes, size--);
    Node *parent = create_node('\0', left->frequency + right->frequency);
    parent->left = left;
    parent->right = right;
    nodes[size] = parent;
    size++;
  }

  Node *root = nodes[0];
  free(nodes);

  return root;
}

void assign_codes(Node *node, char *code)
{
  if (node->left != NULL)
  {
    char *leftCode = malloc(strlen(code) + 2);
    sprintf(leftCode, "%s0", code);
    node->left->code = leftCode;
    assign_codes(node->left, leftCode);
  }

  if (node->right != NULL)
  {
    char *rightCode = malloc(strlen(code) + 2);
    sprintf(rightCode, "%s1", code);
    node->right->code = rightCode;
    assign_codes(node->right, rightCode);
  }
}

void compress_file(FILE *inputFp, FILE *outputFp, Node *root)
{
  int character, i;
  unsigned char byte = 0;
  int bitCount = 0;

  while ((character = fgetc(inputFp)) != EOF)
  {
    if (isalpha(character))
    {
      Node *node = root;
      
      while(node->left != NULL || node->right != NULL)
      {
        if (character < node->character)
          node = node->left;
        else
          node = node->right;
      }

      char *code = node->code;
      while (*code != '\0')
      {
        if (*code == '1')
          byte = (byte << 1) | 0x01;
        else
          byte = byte << 1;
        bitCount++;

        if (bitCount == 8)
        {
          fputc(byte, outputFp);
          byte = 0;
          bitCount = 0;
        }
        code++;
      }
    }
  }

  if (bitCount > 0)
  {
    byte = byte << (8 - bitCount);
    fputc(byte, outputFp);
  }
}

void free_tree(Node *root)
{
  if (root == NULL)
    return;

  free_tree(root->left);
  free_tree(root->right);
  free(root);
}