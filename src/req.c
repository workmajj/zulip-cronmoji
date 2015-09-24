#define REQ_POST_URL "https://api.zulip.com/v1/messages"

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

    curl_easy_setopt(ch, CURLOPT_URL, REQ_POST_URL);
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
