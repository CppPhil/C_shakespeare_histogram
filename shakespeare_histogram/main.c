#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "histogram.h"
#include "is_all_whitespace.h"

int main(void)
{
  Histogram* hist   = histogramCreate();
  FILE*      stream = NULL;

  if (hist == NULL) { goto error; }

  const char filePath[] = "shakespeare.txt";
  stream                = fopen(filePath, "r");

  if (stream == NULL) { goto error; }

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
