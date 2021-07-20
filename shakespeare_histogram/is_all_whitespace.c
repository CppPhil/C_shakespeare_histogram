#include <ctype.h>

#include "is_all_whitespace.h"

bool isAllWhitespace(const char* string)
{
  while (*string != '\0') {
    if (!isspace(*string)) { return false; }
    ++string;
  }

  return true;
}
