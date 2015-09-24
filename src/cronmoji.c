#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <curl/curl.h>

#include "const.h"

#define API_URL "https://api.zulip.com/v1/messages"

#define BUF_SIZE_AUTH 128
#define BUF_SIZE_TPL  256
#define BUF_SIZE_POST 512

#define ENV_VAR_EMAIL "CRONMOJI_EMAIL"
#define ENV_VAR_KEY   "CRONMOJI_KEY"

#define MINS_PER_TICK 30 // matches clock emoji resolution

#define NUM_EMOJI 3

#define TPL_ESC_CHAR '$'

/* time */

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

/* tpl */

int tpl_print_emoji_rand(char *buf, const size_t size, const int idx)
{
    assert(buf != NULL);

    int idx_new = idx;

    for (int i = 0; i < NUM_EMOJI; i++) {
        const char *e = ZULIP_EMOJI[random() % ZULIP_EMOJI_SIZE]; // FIXME

        strlcat(buf, e, size);
        idx_new += strlen(e);

        if (i < NUM_EMOJI - 1) {
            strlcat(buf, " ", size);
            idx_new++;
        }
    }

    return idx_new;
}

int tpl_print_emoji_time(char *buf, const size_t size, const int idx,
    const TimePair *tp)
{
    assert(buf != NULL);
    assert(tp != NULL);

    assert(tp->h >= 1 && tp->h <= 12);
    assert(tp->m >= 0 && tp->m <= 59);
    assert(tp->m % MINS_PER_TICK == 0);

    int idx_new = idx;

    char tmp[12] = {0}; // e.g., ":clock1030:\0"

    if (tp->m == 0) {
        snprintf(tmp, sizeof(tmp), ":clock%d:", tp->h);
    }
    else {
        snprintf(tmp, sizeof(tmp), ":clock%d%d:", tp->h, tp->m);
    }

    strlcat(buf, tmp, size);
    idx_new += strlen(tmp);

    return idx_new;
}

int tpl_print_str_time(char *buf, const size_t size, const int idx,
    const TimePair *tp)
{
    assert(buf != NULL);
    assert(tp != NULL);

    assert(tp->h >= 1 && tp->h <= 12);
    assert(tp->m >= 0 && tp->m <= 59);

    int idx_new = idx;

    char tmp[6] = {0}; // i.e., "[H]H:MM\0"

    snprintf(tmp, sizeof(tmp), "%d:%02d", tp->h, tp->m);
    strlcat(buf, tmp, size);
    idx_new += strlen(tmp);

    return idx_new;
}

void tpl_build_rand(char *buf, const size_t size)
{
    assert(buf != NULL);

    TimePair tp;
    time_pair_init(&tp);

    const char *tpl = ZULIP_TPL[random() % ZULIP_TPL_SIZE]; // FIXME

    for (int buf_idx = 0, i = 0; tpl[i] != 0; i++) {
        if (tpl[i] == TPL_ESC_CHAR && tpl[i + 1] != 0) {
            switch (tpl[i + 1]) {
            case 'e':
                buf_idx = tpl_print_emoji_rand(buf, size, buf_idx);
                break;
            case 'c':
                buf_idx = tpl_print_emoji_time(buf, size, buf_idx, &tp);
                break;
            case 't':
                buf_idx = tpl_print_str_time(buf, size, buf_idx, &tp);
                break;
            default:
                fprintf(stderr, "unknown template token $%c\n", tpl[i + 1]);
                exit(1);
            }

            i++; // two chars total (e.g., "$e")
        }
        else {
            if (buf_idx + 1 >= size) {
                fprintf(stderr, "template buffer exceeded\n");
                exit(1);
            }

            buf[buf_idx] = tpl[i];
            buf_idx++;
        }
    }
}

/* req */

void req_build_auth(char *buf, const size_t size,
    const char *email, const char *key)
{
    assert(buf != NULL);
    assert(email != NULL);
    assert(key != NULL);

    strlcpy(buf, email, size);
    strlcat(buf, ":", size);
    strlcat(buf, key, size);
}

void req_build_post(char *buf, const size_t size,
    const char *stream, const char *subject, const char *content)
{
    assert(buf != NULL);
    assert(stream != NULL);
    assert(subject != NULL);
    assert(content != NULL);

    strlcpy(buf, "type=stream&to=", size);
    strlcat(buf, stream, size);
    strlcat(buf, "&subject=", size);
    strlcat(buf, subject, size);
    strlcat(buf, "&content=", size);
    strlcat(buf, content, size);
}

void req_send(char *buf_auth, char *buf_post)
{
    assert(buf_auth != NULL);
    assert(buf_post != NULL);

    CURL *ch = curl_easy_init();
    if (!ch) {
        curl_global_cleanup();

        fprintf(stderr, "couldn't init curl\n");
        exit(1);
    }

    // TODO: curl_easy_escape?
    // TODO: silence response?

    curl_easy_setopt(ch, CURLOPT_URL, API_URL);
    curl_easy_setopt(ch, CURLOPT_USERPWD, buf_auth);
    curl_easy_setopt(ch, CURLOPT_POSTFIELDS, buf_post);

    CURLcode resp = curl_easy_perform(ch);
    if (resp != CURLE_OK) {
        fprintf(stderr, "curl failed with error %s\n",
            curl_easy_strerror(resp));

        curl_easy_cleanup(ch);
        curl_global_cleanup();

        exit(1);
    }

    curl_easy_cleanup(ch);
    curl_global_cleanup();
}

/* main */

int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "usage: %s <stream> <subject>\n", argv[0]);
        exit(1);
    }

    char *api_email = getenv(ENV_VAR_EMAIL);
    if (!api_email) {
        fprintf(stderr, "couldn't get e-mail from %s\n", ENV_VAR_EMAIL);
        exit(1);
    }

    char *api_key = getenv(ENV_VAR_KEY);
    if (!api_key) {
        fprintf(stderr, "couldn't get api key from %s\n", ENV_VAR_KEY);
        exit(1);
    }

    srandom(time(NULL)); // FIXME

    char buf_tpl[BUF_SIZE_TPL] = {0};

    tpl_build_rand(buf_tpl, sizeof(buf_tpl));

    char buf_auth[BUF_SIZE_AUTH] = {0};
    char buf_post[BUF_SIZE_POST] = {0};

    req_build_auth(buf_auth, sizeof(buf_auth), api_email, api_key);
    req_build_post(buf_post, sizeof(buf_post), argv[1], argv[2], buf_tpl);

    // req_send(buf_auth, buf_post);

    printf("%s\n", buf_tpl); // FIXME: testing

    return 0;
}
