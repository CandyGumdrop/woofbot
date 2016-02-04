/*
 * woofbot - Official IRC bot for #CandyGumdrop on Rizon
 * 
 * Written in 2015 by Candy Gumdrop
 * 
 * To the extent possible under law, the author(s) have dedicated all copyright
 * and related and neighboring rights to this software to the public domain
 * worldwide. This software is distributed without any warranty.
 * 
 * You should have received a copy of the CC0 Public Domain Dedication along
 * with this software. If not, see
 * <http://creativecommons.org/publicdomain/zero/1.0/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <libircclient.h>
#include <libirc_rfcnumeric.h>
#include "config.h"

static irc_callbacks_t callbacks;

typedef void command_func(irc_session_t *session, const char *nick,
        const char *channel, const char *input);

static command_func report_in;
static command_func woof;

#define WOOF_COUNT 8

static const char *woofs[WOOF_COUNT] = {
    "Woof!",
    "W-woof!",
    "Woof!  Woof!",
    "Grrrr!",
    "W-woof...  :3c",
    "Rrruff!",
    "Yip!",
    "Arf!"
};

static const char *command_names[] = {
    "bots",
    "woof",
    NULL
};

static command_func *command_funcs[] = {
    report_in,
    woof,
    NULL
};

static void report_in(irc_session_t *session, const char *nick,
        const char *channel, const char *input)
{
    irc_cmd_msg(session, channel, "Reporting in! [C]");
}

static void woof(irc_session_t *session, const char *nick,
        const char *channel, const char *input)
{
    char msg[256];
    char *msgcut;
    const char *woofmsg;
    const char *name;

    if (input) {
        name = input;
    } else {
        name = nick;
    }

    woofmsg = woofs[rand() % WOOF_COUNT];

    if (strlen(name) + strlen(woofmsg) + 2 > 255) {
        return;
    }

    strcpy(msg, name);
    msgcut = strtok(msg, " ");
    if (!msgcut) {
        msgcut = msg;
    }
    strcat(msgcut, ": ");
    strcat(msgcut, woofmsg);

    irc_cmd_msg(session, channel, msg);
}

static void event_connect(irc_session_t *session, const char *event,
        const char *origin, const char **params, unsigned int count)
{
    int i;

    printf("Identifying with NickServ...\n");
    irc_cmd_msg(session, "NickServ", "IDENTIFY " NICKPASS);

    sleep(1);

    for (i = 0; channels[i]; i++) {
        printf("Joining \"%s\"...\n", channels[i]);
        irc_cmd_join(session, channels[i], NULL);
    }

    printf("Joined all channels.\n");
}

static void event_channel(irc_session_t *session, const char *event,
        const char *origin, const char **params, unsigned int count)
{
    if (count >= 2 && params[1][0] == '.') {
        int i = 0, notfound;
        char commandbuf[128];
        char *commandstr;

        strncpy(commandbuf, params[1] + 1, 127);
        commandbuf[127] = '\0';

        if (!(commandstr = strtok(commandbuf, " "))) {
            return;
        }

        while (command_names[i] &&
                (notfound = strcmp(commandstr, command_names[i]))) {
            i++;
        }

        if (!notfound) {
            char *input = strchr(params[1], ' ');

            if (input) {
                input++;
                printf("%s sent command \"%s\" with input \"%s\".\n",
                        origin, commandstr, input);
            } else {
                printf("%s sent command \"%s\" with no input.\n",
                        origin, commandstr);
            }

            command_funcs[i](session, origin, params[0], input);
        }
    }
}

static void event_numeric(irc_session_t *session, unsigned int event,
        const char *origin, const char **params, unsigned int count)
{
}

static void event_ctcp_req(irc_session_t *session, const char *event,
        const char *origin, const char **params, unsigned int count)
{
}

int main(void)
{
    irc_session_t *session;

    srand(time(NULL));

    callbacks.event_connect = event_connect;
    callbacks.event_numeric = event_numeric;
    /* callbacks.event_ctcp_req = event_ctcp_req; */
    callbacks.event_channel = event_channel;

    if (!(session = irc_create_session(&callbacks))) {
        printf("Failed to create IRC session.\n");
        return 1;
    }

    irc_option_set(session, LIBIRC_OPTION_STRIPNICKS);

    if (irc_connect(session, SERVER, PORT, NULL, NICK, USERNAME, REALNAME)) {
        printf("Failed to connect to server.\n");
        return 1;
    }

    if (irc_run(session)) {
        printf("Connection terminated.\n");
        return 1;
    }

    printf("Bot exited.\n");

    return 0;
}
