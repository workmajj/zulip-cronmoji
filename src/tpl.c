#define TPL_ESC_CHAR '$'
#define TPL_NUM_EMOJI 3

// "@**all**" => broadcast to channel
// "$t" => time as string (e.g., "10:30")
// "$c" => emoji corresponding to current time
// "$e" => randomly selected emoji from below

const size_t ZULIP_TPL_SIZE = 4;
const char *ZULIP_TPL[] = {
    "Hey @**all**, it's $t check-in time! $c $e",
    "$c Yay, @**all**, it's time for $t check-ins! $e",
    "@**all** Woo woo, it's time $c for $t check-ins! $e",
    "$t check-ins are now, @**all**! $c Have fun! $e"
};

const size_t ZULIP_EMOJI_SIZE = 12;
const char *ZULIP_EMOJI[] = {
    ":100:",
    ":computer:",
    ":heart_decoration:",
    ":octopus:",
    ":recursecenter:",
    ":rocket:",
    ":rotating_light:",
    ":smile:",
    ":smiley:",
    ":smile_cat:",
    ":sparkler:",
    ":thumbsup:"
};

int tpl_print_emoji_rand(char *buf, const size_t size, const int idx)
{
    assert(buf != NULL);

    int idx_new = idx;

    for (int i = 0; i < TPL_NUM_EMOJI; i++) {
        const char *e = ZULIP_EMOJI[random() % ZULIP_EMOJI_SIZE]; // FIXME

        strlcat(buf, e, size);
        idx_new += strlen(e);

        if (i < TPL_NUM_EMOJI - 1) {
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

    srandom(time(NULL)); // FIXME

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
