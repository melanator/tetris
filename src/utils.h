/* Get from https://stackoverflow.com/questions/5833094/get-a-timestamp-in-c-in-microseconds */

#ifndef UTILS_H
#define UTILS_H
#include <stdint.h> // `UINT64_MAX`
#include <stdio.h>  // `printf()`
#include <time.h>   // `timespec_get()`

unsigned long get_timestamp();

#endif // UTILS_H