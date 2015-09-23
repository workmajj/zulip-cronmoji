#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "zconst.h"

#define ENV_EMAIL "CRONMOJI_EMAIL"
#define ENV_KEY "CRONMOJI_KEY"

#define MINS_PER_TICK 30 // matches clock emoji

/* time */

typedef struct time_pair {
    int h;
    int m;
} TimePair;

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
    tp->m = t->tm_min - (t->tm_min % MINS_PER_TICK);

    return tp;
}

void time_print_digits(TimePair *tp)
{
    assert(tp != NULL);

    assert(tp->h >= 1 && tp->h <= 12);
    assert(tp->m >= 0 && tp->m <= 59);

    printf("%d:%02d\n", tp->h, tp->m); // i.e., "[H]H:MM"
}

void time_print_emoji(TimePair *tp)
{
    assert(tp != NULL);

    assert(tp->h >= 1 && tp->h <= 12);
    assert(tp->m >= 0 && tp->m <= 59);
    assert(tp->m % MINS_PER_TICK == 0); // clamped

    if (tp->m == 0) {
        printf(":clock%d:\n", tp->h); // e.g., ":clock8:"
    }
    else {
        printf(":clock%d%d:\n", tp->h, tp->m); // e.g., ":clock330:"
    }
}

/* main */

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "usage: %s <stream_name>\n", argv[0]);
        exit(1);
    }

    char *env_email = getenv(ENV_EMAIL);
    if (!env_email) {
        fprintf(stderr, "couldn't get e-mail address from %s\n", ENV_EMAIL);
        exit(1);
    }
    char *env_key = getenv(ENV_KEY);
    if (!env_key) {
        fprintf(stderr, "couldn't get api key from %s\n", ENV_KEY);
        exit(1);
    }

    printf("e-mail address => %s\n", env_email);
    printf("api key => %s\n", env_key);

    TimePair *tp = time_pair_make();

    time_print_digits(tp);
    time_print_emoji(tp);

    free(tp);

    srandom(time(NULL));

    int r = random() % ZULIP_TPL_SIZE;
    printf("%d => %s\n", r, ZULIP_TPL[r]);

    return 0;
}
