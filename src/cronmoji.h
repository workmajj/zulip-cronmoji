#ifndef CRONMOJI_H
#define CRONMOJI_H

typedef struct time_pair {
    int h;
    int m;
} TimePair;

void req_build_auth(char *buf, const size_t size,
    const char *email, const char *key);
void req_build_post(char *buf, const size_t size,
    const char *stream, const char *subject, const char *content);
void req_send(char *buf_auth, char *buf_post);

void time_pair_init(TimePair *tp);

void tpl_build_rand(char *buf, const size_t size);

#endif // CRONMOJI_H
