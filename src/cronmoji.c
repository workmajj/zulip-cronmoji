#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <curl/curl.h>

#include "req.c"
#include "time.c"
#include "tpl.c"

#define ENV_VAR_EMAIL "CRONMOJI_EMAIL"
#define ENV_VAR_KEY "CRONMOJI_KEY"

#define BUF_SIZE_AUTH 128
#define BUF_SIZE_TPL 256
#define BUF_SIZE_POST 512

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

    char buf_tpl[BUF_SIZE_TPL] = {0};

    tpl_build_rand(buf_tpl, sizeof(buf_tpl));

    char buf_auth[BUF_SIZE_AUTH] = {0};
    char buf_post[BUF_SIZE_POST] = {0};

    req_build_auth(buf_auth, sizeof(buf_auth), api_email, api_key);
    req_build_post(buf_post, sizeof(buf_post), argv[1], argv[2], buf_tpl);

    req_send(buf_auth, buf_post);

    printf("%s\n", buf_tpl);

    return 0;
}
