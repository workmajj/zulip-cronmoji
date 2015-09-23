#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <curl/curl.h>

#include "zconst.h"

#define API_POST_URL "https://api.zulip.com/v1/messages"

#define BUF_SIZE_AUTH 128
#define BUF_SIZE_POST 512

#define ENV_VAR_EMAIL "CRONMOJI_EMAIL"
#define ENV_VAR_KEY "CRONMOJI_KEY"

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

/* req */

void req_build_auth(char *buf, const size_t size,
    const char *email, const char *key)
{
    assert(buf != NULL);
    assert(email != NULL);
    assert(key != NULL);

    memset(buf, 0, size);

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

    memset(buf, 0, size);

    strlcpy(buf, "type=stream&to=", size);
    strlcat(buf, stream, size);
    strlcat(buf, "&subject=", size);
    strlcat(buf, subject, size);
    strlcat(buf, "&content=", size);
    strlcat(buf, content, size);
}

void req_send(char *buf_auth, char *buf_post)
{
    CURL *ch = curl_easy_init();
    if (!ch) {
        curl_global_cleanup();

        fprintf(stderr, "couldn't init curl\n");
        exit(1);
    }

    // TODO: curl_easy_escape?
    // TODO: silence response?

    curl_easy_setopt(ch, CURLOPT_URL, API_POST_URL);
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

    /*
    printf("e-mail address => %s\n", env_email);
    printf("api key => %s\n", env_key);

    TimePair *tp = time_pair_make();

    time_print_digits(tp);
    time_print_emoji(tp);

    free(tp);
    */

    srandom(time(NULL));

    int r = random() % ZULIP_TPL_SIZE;

    char buf_auth[BUF_SIZE_AUTH];
    char buf_post[BUF_SIZE_POST];

    req_build_auth(buf_auth, sizeof(buf_auth), api_email, api_key);
    req_build_post(buf_post, sizeof(buf_post), argv[1], argv[2], ZULIP_TPL[r]);

    req_send(buf_auth, buf_post);

    return 0;
}
