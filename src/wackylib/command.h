/* File:   command.h
   Author: M. P. Hayes, UCECE
   Date:   15 May 2016
*/

#ifndef COMMAND_H
#define COMMAND_H

#ifdef __cplusplus
extern "C" {
#endif
    

#include "tty.h"


typedef struct command_struct
{
    char command;
    int (*func) (const char *command, char *response, int size);
} command_t;


int
command_do (char *command, char *response, int size);


int
command_tty_poll (tty_t *tty);

void
command_init (command_t *table);


#ifdef __cplusplus
}
#endif    
#endif

