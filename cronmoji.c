#include <stdio.h>
#include <time.h>

#define TICK_SIZE_MINS 15

void time_str_clamped(char *str)
{
    time_t raw_time;
    struct tm *t;

    time(&raw_time);
    t = localtime(&raw_time);

    int h = (t->tm_hour > 12) ? t->tm_hour - 12 : t->tm_hour;
    int m = t->tm_min - (t->tm_min % TICK_SIZE_MINS); // last tick

    snprintf(str, 6, "%d:%02d", h, m); // "HH:MM\0"
}

int main(int argc, char *argv[])
{
    char time_str[6]; // "HH:MM\0"
    time_str_clamped(time_str);

    printf("%s\n", time_str);

    return 0;
}
