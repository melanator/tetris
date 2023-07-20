#include "utils.h"

/// Convert seconds to milliseconds
#define SEC_TO_MS(sec) ((sec)*1000000000)

unsigned long get_timestamp(){
    uint64_t milliseconds;
    struct timespec ts;
    int return_code = timespec_get(&ts, TIME_UTC);
    if (return_code == 0)
    {
        printf("Failed to obtain timestamp.\n");
        milliseconds = UINT64_MAX; // use this to indicate error
    }
    else
    {
        // `ts` now contains your timestamp in seconds and nanoseconds! To 
        // convert the whole struct to nanoseconds, do this:
        milliseconds = SEC_TO_MS((uint64_t)ts.tv_sec) + (uint64_t)ts.tv_nsec;
    }

    return milliseconds;
}

void sleep_ms(size_t time){
    static struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = time * 1000 * 1000;
    nanosleep(&ts, NULL);
}