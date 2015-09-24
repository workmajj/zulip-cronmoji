/* tpl */

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

/* emoji */

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
