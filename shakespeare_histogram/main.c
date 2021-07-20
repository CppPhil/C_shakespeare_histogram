#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "histogram.h"
#include "is_all_whitespace.h"

int main(int argc, char* argv[])
{
  if (argc != 2) {
    fprintf(stderr, "argc was %d, but should have been 2.\n", argc);
    return EXIT_FAILURE;
  }

  Histogram* hist   = histogramCreate();
  FILE*      stream = NULL;

  if (hist == NULL) { goto error; }

  const char* filePath = argv[1];
  stream               = fopen(filePath, "r");

  if (stream == NULL) {
    fprintf(stderr, "Could not open %s\n", filePath);
    goto error;
  }

  const char longestWord[] = "pneumonoultramicroscopicsilicovolcanoconiosis";
  char       wordBuffer[sizeof(longestWord)] = {'\0'};
  size_t     currentIndex                    = 0;
  int        currentChar;
  while ((currentChar = getc(stream)) != EOF) {
    if (isspace(currentChar)) {
      if (!isAllWhitespace(wordBuffer)) {
        if (!histogramAdd(hist, wordBuffer)) { goto error; }
      }

      currentIndex = 0;
      memset(wordBuffer, 0, sizeof(wordBuffer));
    }
    else {
      wordBuffer[currentIndex] = (char)currentChar;
      ++currentIndex;

      if (currentIndex >= (sizeof(wordBuffer) - 1)) {
        fprintf(stderr, "Word is too long!: %s\n", wordBuffer);
        goto error;
      }
    }
  }

  histogramPrint(hist, stdout);

  if (fclose(stream) == EOF) {
    fprintf(stderr, "Could not close file \"%s\"!", filePath);
    goto error;
  }
  histogramFree(hist);
  return EXIT_SUCCESS;

error:
  if (stream != NULL) { fclose(stream); }
  histogramFree(hist);
  return EXIT_FAILURE;
}
