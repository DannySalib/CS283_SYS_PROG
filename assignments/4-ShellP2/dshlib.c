#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "dshlib.h"

/*
 * Implement your exec_local_cmd_loop function by building a loop that prompts the 
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.
 * 
 *      while(1){
 *        printf("%s", SH_PROMPT);
 *        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
 *           printf("\n");
 *           break;
 *        }
 *        //remove the trailing \n from cmd_buff
 *        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 * 
 *        //IMPLEMENT THE REST OF THE REQUIREMENTS
 *      }
 * 
 *   Also, use the constants in the dshlib.h in this code.  
 *      SH_CMD_MAX              maximum buffer size for user input
 *      EXIT_CMD                constant that terminates the dsh program
 *      SH_PROMPT               the shell prompt
 *      OK                      the command was parsed properly
 *      WARN_NO_CMDS            the user command was empty
 *      ERR_TOO_MANY_COMMANDS   too many pipes used
 *      ERR_MEMORY              dynamic memory management failure
 * 
 *   errors returned
 *      OK                     No error
 *      ERR_MEMORY             Dynamic memory management failure
 *      WARN_NO_CMDS           No commands parsed
 *      ERR_TOO_MANY_COMMANDS  too many pipes used
 *   
 *   console messages
 *      CMD_WARN_NO_CMD        print on WARN_NO_CMDS
 *      CMD_ERR_PIPE_LIMIT     print on ERR_TOO_MANY_COMMANDS
 *      CMD_ERR_EXECUTE        print on execution failure of external command
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 1+)
 *      malloc(), free(), strlen(), fgets(), strcspn(), printf()
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 2+)
 *      fork(), execvp(), exit(), chdir()
 */
int exec_local_cmd_loop()
{
    char *cmd_buff;
    int rc = 0;
    cmd_buff_t cmd;
    pid_t pid;


    cmd.argc = 0;

    // Allocate memory for fgets 
    cmd._cmd_buffer = malloc(SH_CMD_MAX * sizeof(char));
    if (cmd._cmd_buffer == NULL) {
        return ERR_MEMORY;
    }

    while(1){

        printf("%s", SH_PROMPT);
        if (fgets(cmd._cmd_buffer, ARG_MAX, stdin) == NULL){
            printf("\n");
            break;
        }
        
        //remove the trailing \n from cmd_buff
        cmd._cmd_buffer[strcspn(cmd._cmd_buffer,"\n")] = '\0';

        // Warn user if they provide no command 
        if (cmd._cmd_buffer[0] == '\0') {
            printf(CMD_WARN_NO_CMD);
        }
        
        // TODO IMPLEMENT parsing input to cmd_buff_t *cmd_buff
        char *pipe_token, *pipe_saveptr;

        pipe_token = strtok_r(cmd._cmd_buffer, PIPE_STRING, &pipe_saveptr);

        // Parse each command separated by pipes
        while (pipe_token) {
            char *cmd_ptr = pipe_token;
            
            while (*cmd_ptr) {
                // Skip leading spaces
                while (*cmd_ptr == ' ') cmd_ptr++;

                if (*cmd_ptr == '\0') break; // No more tokens

                // Handle quoted tokens
                if (*cmd_ptr == DOUBLE_QUOTE_CHAR || *cmd_ptr == SINGLE_QUOTE_CHAR) {
                    char quote = *cmd_ptr++;
                    char *start = cmd_ptr;
                    while (*cmd_ptr && *cmd_ptr != quote) cmd_ptr++;

                    if (*cmd_ptr == quote) {
                        // Allocate and store the quoted string, preserving spaces
                        int length = (int)(cmd_ptr - start);
                        char *token = (char *)malloc(length + 1);
                        if (token) {
                            strncpy(token, start, length);
                            token[length] = '\0';
                            cmd.argv[cmd.argc++] = token;
                        }
                        cmd_ptr++; // Move past the closing quote
                    }
                } else {
                    // Handle regular tokens
                    char *start = cmd_ptr;
                    while (*cmd_ptr && *cmd_ptr != SPACE_CHAR && *cmd_ptr != DOUBLE_QUOTE_CHAR && *cmd_ptr != SINGLE_QUOTE_CHAR) cmd_ptr++;

                    int length = (int)(cmd_ptr - start);
                    char *token = (char *)malloc(length + 1);
                    if (token) {
                        strncpy(token, start, length);
                        token[length] = '\0';
                        cmd.argv[cmd.argc++] = token;
                    }
                }
            }

            pipe_token = strtok_r(NULL, PIPE_STRING, &pipe_saveptr);
        }


        // TODO IMPLEMENT if built-in command, execute builtin logic for exit, cd (extra credit: dragon)
        // the cd command should chdir to the provided directory; if no directory is provided, do nothing

        // No command logic 
        //printf("ARGC %d\n", cmd.argc);
        if (cmd.argc == 0) continue;

        // Exit command logic 
        if (strcmp(cmd.argv[0], EXIT_CMD) == 0) break;


        // cd command logic 
        if (strcmp(cmd.argv[0], CD_CMD) == 0) {
            // execute cd with chdir
            if (cmd.argc < 2) {
                //printf("Provide path\n");
            } else if (chdir(cmd.argv[1]) != 0) {
                //printf("Error changing dir\n");
            }

            cmd.argc = 0;
            continue;
        }

        // fork/exec any other commands 
        pid = fork();
        if (pid < 0) {
            return ERR_EXEC_CMD;
        }
        // We can assume # of comand tokens > 0, so terminate our list 
        cmd.argv[cmd.argc] = NULL;  // Terminate the array of pointers
        // execute command if ready, otherwise, wait
        if (pid == 0) {
            if (execvp(cmd.argv[0], cmd.argv) == -1) {
                return ERR_EXEC_CMD;
            }
        } else {
            // In parent process: wait for the child to finish
            int status;
            if (waitpid(pid, &status, 0) < 0) {
                return ERR_EXEC_CMD;
            }
        }
        // TODO IMPLEMENT if not built-in command, fork/exec as an external command
        // for example, if the user input is "ls -l", you would fork/exec the command "ls" with the arg "-l"
        cmd.argc = 0;
    }
    

    return OK;
}
