#ifndef IC_LOGGING_H
#define IC_LOGGING_H

#include <stdio.h>
#include <stdlib.h>

#define LOG_DEBUG(...) fprintf(stderr, __VA_ARGS__)
#define LOG_INFO(...) fprintf(stdout, __VA_ARGS__)
#define LOG_WARNING(...) fprintf(stdout, __VA_ARGS__)
#define ERR() { perror("unknown"); fprintf(stderr, "line: %d\n", __LINE__); abort(); }

#endif //ifdef LLOGGING_H
