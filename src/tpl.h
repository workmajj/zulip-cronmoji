// "@**all**" => broadcast to channel
// "$t" => time as string (e.g., "10:30")
// "$c" => emoji corresponding to current time
// "$e" => randomly selected emoji from below

const size_t ZULIP_TPL_SIZE = 12;
const char *ZULIP_TPL[] = {
    "$c Yay, @**all**, it's time for $t check-ins! $e",
    "$e @**all** Yo, it's time $c for $t check-ins!",
    "$e Get excited, y'@**all**, 'cause it's $t check-in time! $c",
    "$e Guess what, @**all**? It's $t check-in time! $c",
    "$e It's that time again, @**all**! $t is here! $c",
    "$e Time for $t $c check-ins, @**all**!",
    "$t check-ins are now, @**all**! $c Have fun! $e",
    "@**all** $t check-in time $c has arrived! $e",
    "@**all** Woo woo, it's time $c for $t check-ins! $e",
    "Heads up, @**all**, it's time for $t $c check-ins! $e",
    "Hey @**all**, it's $t check-in time! $c $e",
    "ZOMG @**all**, it's $t check-in time! $c $e"
};

const size_t ZULIP_EMOJI_SIZE = 48;
const char *ZULIP_EMOJI[] = {
    ":100:",
    ":art:",
    ":clap:",
    ":collision:",
    ":computer:",
    ":confetti_ball:",
    ":cool:",
    ":fire:",
    ":green_book:",
    ":grin:",
    ":grinning:",
    ":heartbeat:",
    ":heart_decoration:",
    ":heart_eyes_cat:",
    ":high_brightness:",
    ":hourglass_flowing_sand:",
    ":laughing:",
    ":mega:",
    ":milky_way:",
    ":new:",
    ":octopus:",
    ":open_hands:",
    ":raised_hands:",
    ":raising_hand:",
    ":recursecenter:",
    ":relaxed:",
    ":rocket:",
    ":rotating_light:",
    ":running:",
    ":slot_machine:",
    ":smile:",
    ":smiley:",
    ":smiley_cat:",
    ":smile_cat:",
    ":sparkler:",
    ":sparkles:",
    ":speaker:",
    ":star2:",
    ":star:",
    ":sunglasses:",
    ":sun_with_face:",
    ":thumbsup:",
    ":tiger2:",
    ":tiger:",
    ":trophy:",
    ":volcano:",
    ":watch:",
    ":zap:"
};
