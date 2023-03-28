#include "node.h"

int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    fprintf(stderr, "Usage: %s input_file output_file\n", argv[0]);
    return 1;
  }

  FILE *inputFile = fopen(argv[1], "r");
  if (inputFile == NULL)
  {
    fprintf(stderr, "Error: Cannot open input file %s\n", argv[1]);
    return 1;
  }

  FILE *outputFile = fopen(argv[2], "wb");
  if (outputFile == NULL)
  {
    fprintf(stderr, "Error: Cannot open input file %s\n", argv[2]);
    return 1;
  }

  int frequencies[256] = {0};
  count_frequencies(inputFile, frequencies);

  Node *root = build_tree(frequencies);
  printf("%c", root->character);

  assign_codes(root, "");
  fseek(inputFile, 0, SEEK_SET);
  compress_file(inputFile, outputFile, root);
  fclose(inputFile);
  fclose(outputFile);
  free_tree(root);

  return 0;
}