#pragma once

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

#define NORMAL		"\x1B[0m"
#define RED			"\x1B[31m"
#define GREEN		"\x1B[32m"
#define YELLOW		"\x1B[33m"
#define BLUE		"\x1B[34m"
#define MAGENTA		"\x1B[35m"
#define CYAN		"\x1B[36m"
#define WHITE		"\x1B[37m"

#define DIM			"\x1B[7m"
#define UNDERSCORE	"\x1B[4m"
#define BOLD		"\x1B[1m"

#define LOG_LEVEL_LOG       0
#define LOG_LEVEL_INFO      1
#define LOG_LEVEL_WARNING   2
#define LOG_LEVEL_ERROR     3

#define RENDER_LOG(level, str) {                                              \
    switch(level) {                                                           \
        case LOG_LEVEL_LOG:     fprintf(stderr, RED str NORMAL); break;       \
        case LOG_LEVEL_INFO:    fprintf(stderr, RED str NORMAL); break;       \
        case LOG_LEVEL_WARNING: fprintf(stderr, RED str NORMAL); break;       \
        case LOG_LEVEL_ERROR:   fprintf(stderr, RED str NORMAL); break;       \
    }                                                                         \
}

