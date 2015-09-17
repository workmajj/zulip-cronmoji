#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "const.h"
#include "pcg_basic.h" // http://www.pcg-random.org/

#define TICK_SIZE_MINS 30 // matches clock emoji

typedef struct time_pair {
    int h;
    int m;
} TimePair;

/* time */

TimePair *time_pair_make()
{
    TimePair *tp = malloc(sizeof(TimePair));
    assert(tp != NULL);

    time_t time_raw = time(NULL);
    struct tm *t = localtime(&time_raw);

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

    printf("%d:%02d\n", tp->h, tp->m); // i.e., "[H]H:MM"
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

/* rand */

int rand_gen(int bound)
{
    const int rounds = 5;

    pcg32_random_t rng;
    pcg32_srandom_r(&rng, time(NULL) ^ (intptr_t)&printf, (intptr_t)&rounds);

    return (int)pcg32_boundedrand_r(&rng, bound); // 0 <= x < bound
}

/* main */

int main(int argc, char *argv[])
{
    TimePair *tp = time_pair_make();

    time_print_digits(tp);
    time_print_emoji(tp);

    free(tp);

    int r = rand_gen(ZULIP_MSG_SIZE);

    printf("%d => %s\n", r, ZULIP_MSG[r]);

    return 0;
}
