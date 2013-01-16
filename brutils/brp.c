#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#include "brutils.h"

void showusage() {
  fprintf(stderr, "usage: brp input column-index num-groups output-format\n");
  fprintf(stderr, "       can specify '-' for input, to read from stdin\n");
  exit(1);
}

int main(int argc, char * argv[])
{
  line_t line;
  int i, j, col_index;
  int first_line = 1;

  if (argc < 5)
    showusage();
  if (strcmp(argv[1], "-") != 0)
    line.pin = try_open(argv[1], "rb");
  else
    line.pin = stdin;
  col_index = atoi(argv[2]);
  int pouts_len = atoi(argv[3]);
  const char* output_format = argv[4];

  FILE ** pouts = (FILE **) malloc( pouts_len * sizeof(FILE *) );
  char out_path[256];
  for (i = 0; i != pouts_len; ++i)
  {
    sprintf(out_path, output_format, i);
    pouts[i] = try_open(out_path, "wb");
  }

  while (fgets(line.buf, sizeof(line.buf), line.pin)) {
    if (first_line) {
      for (j = 0; j != pouts_len; ++j)
      {
        fputs(line.buf, pouts[j]); // write header to all the files
        first_line = 0;
      }
    }
    else if ( find_col(col_index, &line) ) // if this string has the requisite number of columns
      fputs(line.buf, pouts[fnv_hash(line.col_beg, line.col_end) % pouts_len]); // write it to the correct file
  }

  if (line.pin != stdin)
    fclose(line.pin);

  for (i = 0; i != pouts_len; ++i)
    fclose(pouts[i]);

  return 0;
}

