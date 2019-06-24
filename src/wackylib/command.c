/* File:   command.c
   Author: M. P. Hayes, UCECE
   Date:   15 May 2016
*/

#include "command.h"
#include <ctype.h>
#include <string.h>

static command_t *command_table;

#define MESSAGE_SIZE_MAX 16

int
command_do (char *str, char *response, int size)
{
    int command;
    int i;

    if (response)
        response[0] = 0;

    if (str[0] == 0)
        return 1;

    command = toupper ((int) str[0]);

    for (i = 0; command_table[i].command; i++)
    {
        if (command_table[i].command == command)
        {
            return command_table[i].func (str, response, MESSAGE_SIZE_MAX);
        }
    }
    return 0;
}


int
command_tty_poll (tty_t *tty)
{
    char response[MESSAGE_SIZE_MAX];
    char buffer[32];
    char *str;
    int size;
    int ret;
            
    /* Check if connection dropped.  */
    if (! tty_poll (tty))
        return 0;

    /* Grab a string if a line is ready otherwise return 0.  */
    str = tty_gets (tty, buffer, sizeof (buffer));
    if (!str)
        return 1;

    size = strlen (str);
    str[size - 1] = '\0';

    ret = command_do (str, response, sizeof (response));

    if (ret <= 0)
    {
        tty_printf (tty, "Error %d\n", ret);
    }
    else if (response[0])
    {
        tty_printf (tty, response);
        tty_printf (tty, "\n");
    }

    /* Send newline at end of prompt to simplify parsing for
       communicating program.  */
    tty_printf (tty, "> \n");
    return 1;
}


void
command_init (command_t *table)
{
    command_table = table;
}
