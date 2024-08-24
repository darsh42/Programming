#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define ERROR(file, func, msg) fprintf(stderr, "[Error]: %s %s => %s\n", file, func, msg)

#endif // COMMON_H_INCLUDED
