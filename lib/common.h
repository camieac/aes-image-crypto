#ifndef IC_COMMON_H
#define IC_COMMON_H

#include <string.h>

enum return_codes {
  SUCCESS = 0,
  FAILURE
};

size_t strlcpy(char* dst, const char* src, size_t bufsize);

#endif
