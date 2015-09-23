#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <curl/curl.h>

#include "zconst.h"

#define ENV_VAR_EMAIL "CRONMOJI_EMAIL"
#define ENV_VAR_KEY "CRONMOJI_KEY"

#define MINS_PER_TICK 30 // matches clock emoji

#define POST_URL "https://api.zulip.com/v1/messages"

char *global_api_email;
char *global_api_key;

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

/* curl */

void curl_send(const char *stream, const char *subject, const char *content)
{
    char buf_auth[128] = {0};
    strlcpy(buf_auth, global_api_email, sizeof(buf_auth));
    strlcat(buf_auth, ":", sizeof(buf_auth));
    strlcat(buf_auth, global_api_key, sizeof(buf_auth));

    char buf_post[256] = {0};
    strlcpy(buf_post, "type=stream&to=", sizeof(buf_post));
    strlcat(buf_post, stream, sizeof(buf_post));
    strlcat(buf_post, "&subject=", sizeof(buf_post));
    strlcat(buf_post, subject, sizeof(buf_post));
    strlcat(buf_post, "&content=", sizeof(buf_post));
    strlcat(buf_post, content, sizeof(buf_post));

    CURL *curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();

        fprintf(stderr, "couldn't init curl\n");
        exit(1);
    }

    char *curl_auth = curl_easy_escape(curl, buf_auth, 0);
    char *curl_post = curl_easy_escape(curl, buf_post, 0);

    curl_easy_setopt(curl, CURLOPT_URL, POST_URL);
    curl_easy_setopt(curl, CURLOPT_USERPWD, buf_auth);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, buf_post);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "curl failed with error %s\n", curl_easy_strerror(res));
        exit(1);
    }

    curl_free(curl_auth);
    curl_free(curl_post);

    curl_easy_cleanup(curl);
    curl_global_cleanup();
}

/* main */

int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "usage: %s <stream> <subject>\n", argv[0]);
        exit(1);
    }

    global_api_email = getenv(ENV_VAR_EMAIL);
    if (!global_api_email) {
        fprintf(stderr, "couldn't get e-mail from %s\n", ENV_VAR_EMAIL);
        exit(1);
    }

    global_api_key = getenv(ENV_VAR_KEY);
    if (!global_api_key) {
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

    int idx = random() % ZULIP_TPL_SIZE;
    curl_send(argv[1], argv[2], ZULIP_TPL[idx]);

    return 0;
}
