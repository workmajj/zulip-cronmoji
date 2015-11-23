#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "cronmoji.h"
#include "tpl.h"

#define TPL_ESC_CHAR '$'
#define TPL_NUM_EMOJI 3

size_t tpl_print_esc_char(char *buf, const size_t size)
{
    assert(buf != NULL);

    char tmp[2] = {TPL_ESC_CHAR, 0};

    strlcat(buf, tmp, size);

    return strlen(tmp);
}

size_t tpl_print_emoji_rand(char *buf, const size_t size)
{
    assert(buf != NULL);

    int size_delta = 0;

    for (int i = 0; i < TPL_NUM_EMOJI; i++) {
        const char *e = ZULIP_EMOJI[random() % ZULIP_EMOJI_SIZE];

        strlcat(buf, e, size);
        size_delta += strlen(e);

        if (i < TPL_NUM_EMOJI - 1) {
            strlcat(buf, " ", size);
            size_delta++;
        }
    }

    return size_delta;
}

size_t tpl_print_emoji_time(char *buf, const size_t size, const TimePair *tp)
{
    assert(buf != NULL);
    assert(tp != NULL);

    assert(tp->h >= 1 && tp->h <= 12);
    assert(tp->m >= 0 && tp->m <= 59);

    char tmp[12] = {0}; // e.g., ":clock1030:\0"

    if (tp->m == 0) {
        snprintf(tmp, sizeof(tmp), ":clock%d:", tp->h);
    }
    else {
        snprintf(tmp, sizeof(tmp), ":clock%d%d:", tp->h, tp->m);
    }

    strlcat(buf, tmp, size);

    return strlen(tmp);
}

size_t tpl_print_str_time(char *buf, const size_t size, const TimePair *tp)
{
    assert(buf != NULL);
    assert(tp != NULL);

    assert(tp->h >= 1 && tp->h <= 12);
    assert(tp->m >= 0 && tp->m <= 59);

    char tmp[6] = {0}; // i.e., "[H]H:MM\0"

    snprintf(tmp, sizeof(tmp), "%d:%02d", tp->h, tp->m);

    strlcat(buf, tmp, size);

    return strlen(tmp);
}

void tpl_build_rand(char *buf, const size_t size)
{
    assert(buf != NULL);

    TimePair tp;
    time_pair_init(&tp);

    srandom(time(NULL));

    const char *tpl = ZULIP_TPL[random() % ZULIP_TPL_SIZE];

    size_t idx_buf = 0;
    size_t idx_tpl = 0;
    size_t size_delta;

    while (tpl[idx_tpl] != 0) {
        if (tpl[idx_tpl] != TPL_ESC_CHAR || tpl[idx_tpl + 1] == 0) {
            if (idx_buf + 1 >= size - 1) { // '\0'
                fprintf(stderr, "template buffer exceeded\n");
                exit(1);
            }

            buf[idx_buf] = tpl[idx_tpl];

            idx_buf++;
            idx_tpl++;

            continue;
        }

        switch (tpl[idx_tpl + 1]) {
        case TPL_ESC_CHAR:
            size_delta = tpl_print_esc_char(buf, size);
            break;
        case 'e':
            size_delta = tpl_print_emoji_rand(buf, size);
            break;
        case 'c':
            size_delta = tpl_print_emoji_time(buf, size, &tp);
            break;
        case 't':
            size_delta = tpl_print_str_time(buf, size, &tp);
            break;
        default:
            fprintf(stderr, "unknown template token %c\n", tpl[idx_tpl + 1]);
            exit(1);
        }

        if (idx_buf + size_delta >= size - 1) { // '\0'
            fprintf(stderr, "template buffer exceeded\n");
            exit(1);
        }

        idx_buf += size_delta;
        idx_tpl += 2;
    }
}
