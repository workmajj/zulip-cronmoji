typedef struct time_pair {
    int h;
    int m;
} TimePair;

void time_pair_init(TimePair *tp)
{
    assert(tp != NULL);

    time_t time_raw = time(NULL);
    struct tm *t = localtime(&time_raw);

    // range is 1-12 (midnight is 12)
    tp->h = (t->tm_hour == 0) ? 12 :
        ((t->tm_hour > 12) ? t->tm_hour - 12 : t->tm_hour);

    tp->m = t->tm_min - (t->tm_min % MINS_PER_TICK); // clamped to last tick
}
