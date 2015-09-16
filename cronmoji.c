#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TICK_SIZE_MINS 30 // matched to clock emoji

typedef struct time_pair {
    int h;
    int m;
} TimePair;

TimePair *time_pair_alloc()
{
    time_t raw_time;
    struct tm *t;

    time(&raw_time);
    t = localtime(&raw_time);

    TimePair *tp = malloc(sizeof(TimePair));
    assert(tp != NULL);

    // range is 1-12 (midnight is 12)
    tp->h = (t->tm_hour == 0) ? 12 :
        ((t->tm_hour > 12) ? t->tm_hour - 12 : t->tm_hour);

    // clamped to last tick
    tp->m = t->tm_min - (t->tm_min % TICK_SIZE_MINS);

    return tp;
}

void time_print_digits(TimePair *tp) // FIXME: testing
{
    assert(tp != NULL);

    assert(tp->h >= 1 && tp->h <= 12);
    assert(tp->m >= 0 && tp->m <= 59);

    printf("%d:%02d\n", tp->h, tp->m); // i.e., "HH:MM\0"
}

void time_print_emoji(TimePair *tp) // FIXME: testing
{
    assert(tp != NULL);

    assert(tp->h >= 1 && tp->h <= 12);
    assert(tp->m >= 0 && tp->m <= 59);
    assert(tp->m % TICK_SIZE_MINS == 0);

    if (tp->m == 0) {
        printf(":clock%d:\n", tp->h); // e.g., ":clock8:"
    }
    else {
        printf(":clock%d%d:\n", tp->h, tp->m); // e.g., ":clock330:"
    }
}

int main(int argc, char *argv[])
{
    TimePair *tp = time_pair_alloc();

    time_print_digits(tp);
    time_print_emoji(tp);

    free(tp);

    return 0;
}
