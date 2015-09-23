/* tpl */

// "@**all**" => broadcast to channel
// "[time]" => time as string (e.g., "10:30")
// "[clock]" => emoji corresponding to current time
// "[emoji]" => randomly selected emoji from below

const size_t ZULIP_TPL_SIZE = 4;
const char *ZULIP_TPL[] = {
    "Hey @**all**, it's [time] [clock] check-in time! [emoji]",
    "[clock] Yay, @**all**, it's time for [time] check-ins! [emoji]",
    "@**all** Woo woo, it's time [clock] for [time] check-ins! [emoji]",
    "[time] [clock] check-ins are now, @**all**! Have fun! [emoji]"
};

/* emoji */

// leading/trailing colons added programmatically (e.g., ":octopus:")

const size_t ZULIP_EMOJI_SIZE = 12;
const char *ZULIP_EMOJI[] = {
    "100",
    "computer",
    "heart_decoration",
    "octopus",
    "recursecenter",
    "rocket",
    "rotating_light",
    "smile",
    "smiley",
    "smile_cat",
    "sparkler",
    "thumbsup"
};