#define TIME_TICK_MINS 30 // matches clock emoji resolution

typedef struct time_pair {
    int h;
    int m;
} TimePair;

void time_pair_init(TimePair *tp)
{
    assert(tp != NULL);

    time_t time_raw = time(NULL);
    struct tm *t = localtime(&time_raw);

    int rem = t->tm_min % TIME_TICK_MINS;

    tp->h = t->tm_hour;
    tp->m = t->tm_min - rem;

    // round to nearest tick
    if (rem >= TIME_TICK_MINS / 2) {
        tp->m += TIME_TICK_MINS;

        if (tp->m == 60) {
            tp->h = (tp->h == 12) ? 1 : tp->h + 1;
            tp->m = 0;
        }
    }

    // hours range from 1 to 12 (midnight is 12)
    tp->h = (tp->h == 0) ? 12 : ((tp->h > 12) ? tp->h - 12 : tp->h);
}
