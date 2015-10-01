zulip-cronmoji
==============

Emojified RC check-in reminders broadcast on [Zulip](https://zulip.org/) :city_sunrise: :computer: :octopus: :clock1030: :rotating_light:

Contact
-------

John J. Workman ([@workmajj](https://twitter.com/workmajj))

I worked on this software during my time as a [Recurse Center](https://www.recurse.com/) facilitator. If you'd like to join a warm community of programmers dedicated to self-improvement, you should consider applying. :-)

Description
-----------

I use this program at RC to announce our regularly scheduled check-ins automatically. Because [sending a message via Zulip](https://zulip.com/api/) is as easy as making a `POST` request, I tried complicating things by working in C, and by implementing a simple template language for the messages.

When `cronmoji` runs, it gets the current time, rounds to the nearest half-hour, and then uses this info to evaluate a random message template. Templates can specify adding the time as a string (`$t`), adding a clock emoji (`$c`), or adding three randomly selected fun/goofy emoji (`$e`). The final message string is published to the Zulip stream and topic specified as command-line args.

Features I won't be adding, because this is a silly project:

* loading template string and emoji arrays from external files
* pinging specific people rather than the whole group
* making the program into an interactive bot that runs in the background

This project will eventually be obsoleted by internal RC tools.

Build & Usage
-------------

1. After cloning the project, open `src/tpl.c` and customize the array of message templates and/or the array of fun emoji (of which `TPL_NUM_EMOJI` are randomly chosen):

        const size_t ZULIP_TPL_SIZE = 8;
        const char *ZULIP_TPL[] = {
            "$c Yay, @**all**, it's time for $t check-ins! $e",
            "$e Get excited, y'@**all**, 'cause it's $t check-in time! $c",
            "$e It's that time again, @**all**! $t is here! $c",
            "$t check-ins are now, @**all**! $c Have fun! $e",
            "@**all** $t check-in time $c has arrived! $e",
            "@**all** Woo woo, it's time $c for $t check-ins! $e",
            "Heads up, @**all**, it's time for $t $c check-ins! $e",
            "Hey @**all**, it's $t check-in time! $c $e"
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

2. Next, build the program from source:

        $ make all

3. Then set up your environment to export the appropriate Zulip API credentials. These can be sourced from your `.bashrc` if you'll be running `cronmoji` repeatedly:

        $ export CRONMOJI_EMAIL=<your-zulip-api-email>
        $ export CRONMOJI_KEY=<your-zulip-api-key>

4. Now you can test `cronmoji` manually (a dedicated Zulip test stream might be good):

        $ ./cronmoji <zulip-test-stream-name> <zulip-topic-name>
        {"msg":"","result":"success","id":12345678}
        @**all** 1:00 check-in time :clock1: has arrived! :octopus: :computer: :smile:

5. Finally, add to `launchd` or `cron` to run at certain intervals. Remember that the time will round to the nearest `TIME_TICK_MINS` (defined in `src/time.c` as `30` by default) when announcing check-ins. So a job that runs at 12:40, for example, will round to `12:30`; but jobs at 12:45 and 12:50 will round to `1:00`. All times are in 12-hour format because of emoji availability.

License
-------

This software is published under the terms of the BSD 3-clause license.

See the [LICENSE](LICENSE.md) file for more info.
