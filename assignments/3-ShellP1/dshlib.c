#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "dshlib.h"

/*
 *  build_cmd_list
 *    cmd_line:     the command line from the user
 *    clist *:      pointer to clist structure to be populated
 *
 *  This function builds the command_list_t structure passed by the caller
 *  It does this by first splitting the cmd_line into commands by spltting
 *  the string based on any pipe characters '|'.  It then traverses each
 *  command.  For each command (a substring of cmd_line), it then parses
 *  that command by taking the first token as the executable name, and
 *  then the remaining tokens as the arguments.
 *
 *  NOTE your implementation should be able to handle properly removing
 *  leading and trailing spaces!
 *
 *  errors returned:
 *
 *    OK:                      No Error
 *    ERR_TOO_MANY_COMMANDS:   There is a limit of CMD_MAX (see dshlib.h)
 *                             commands.
 *    ERR_CMD_OR_ARGS_TOO_BIG: One of the commands provided by the user
 *                             was larger than allowed, either the
 *                             executable name, or the arg string.
 *
 *  Standard Library Functions You Might Want To Consider Using
 *      memset(), strcmp(), strcpy(), strtok(), strlen(), strchr()
 */
int build_cmd_list(char *cmd_line, command_list_t *clist)
{
    // function was given null memory 
    if (!cmd_line || !clist) {
        return ERR_CMD_OR_ARGS_TOO_BIG;
    }

    memset(clist, 0, sizeof(command_list_t)); // init to 0s

    bool is_exe; // differentiates exe vs arg
    char *pipe_token, *cmd_token, *pipe_saveptr, *space_saveptr;
    command_t cmd;

    pipe_token = strtok_r(cmd_line, PIPE_STRING, &pipe_saveptr);
    is_exe = true;
    memset(&cmd, 0, sizeof(command_t));

    // parse any pipes in cmd
    while (pipe_token) {

        cmd_token = strtok_r(pipe_token, SPACE_STRING, &space_saveptr);
        // parse command for each pipe 
        while (cmd_token) {
            
            // trim white space
            while (*cmd_token == SPACE_CHAR) cmd_token++;
            char *end = cmd_token + strlen(cmd_token) - 1;
            while (end > cmd_token && *end == SPACE_CHAR) *end-- = '\0';
            
            // store command token appropriately
            if (is_exe) {
                //printf("EXE "); // TODO INIT a command_t
                strncpy(cmd.exe, cmd_token, EXE_MAX - 1); // Set executable name
            } else {
                strncat(cmd.args, cmd_token, ARG_MAX - strlen(cmd.args) - 2); // Append arg
                strncat(cmd.args, " ", ARG_MAX - strlen(cmd.args) - 1); // Space-separate args
            }

            //printf("Command \"%s\"\n", cmd_token);

            is_exe = false;
            cmd_token = strtok_r(NULL, SPACE_STRING, &space_saveptr);
        }
        
        // Append command_t to command_list_t
        if (clist->num > CMD_MAX) {
            return ERR_TOO_MANY_COMMANDS;
        }

        clist->commands[clist->num++] = cmd;

        is_exe = true;
        memset(&cmd, 0, sizeof(command_t));
        pipe_token = strtok_r(NULL, PIPE_STRING, &pipe_saveptr);
    }

    //printf(M_NOT_IMPL);
    return OK;
}