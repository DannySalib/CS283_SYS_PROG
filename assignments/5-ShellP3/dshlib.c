#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>

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
    cmd.num_commands = 0;

    // Allocate memory for fgets 
    cmd._cmd_buffer = malloc(SH_CMD_MAX * sizeof(char));
    if (cmd._cmd_buffer == NULL) return ERR_MEMORY;
    
    // Allocate memory for first each argv
    cmd.argv[cmd.num_commands] = malloc(CMD_ARGV_MAX * sizeof(char*));
    if (cmd.argv[cmd.argc] == NULL) return ERR_MEMORY;

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
        
        // parsing input to cmd_buff_t *cmd_buff
        char *pipe_token, *pipe_saveptr;

        pipe_token = strtok_r(cmd._cmd_buffer, PIPE_STRING, &pipe_saveptr);

        // Parse each command separated by pipes
        while (pipe_token) {
            
            if (cmd.num_commands > CMD_MAX)
            {
                printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);
                return ERR_TOO_MANY_COMMANDS;
            } 

            char *cmd_ptr = pipe_token;
            while (*cmd_ptr) {
                
                if (cmd.argc > ARG_MAX) return ERR_CMD_OR_ARGS_TOO_BIG;
                 
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
                            //cmd.argv[cmd.argc++] = token;
                            cmd.argv[cmd.num_commands][cmd.argc++] = token;
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
                        //cmd.argv[cmd.argc++] = token;
                        cmd.argv[cmd.num_commands][cmd.argc++] = token;
                    }
                }
            }
            
            // we have properly parsed and cleaned each token in the command 
            //cmd.argv[cmd.argc] = NULL; 
            cmd.argv[cmd.num_commands][cmd.argc] = NULL; // terminate the argument list
            
            // allocate memory for a new argv[] for the next command
            cmd.argv[++cmd.num_commands] = malloc(CMD_ARGV_MAX * sizeof(char*));
            if (cmd.argv[cmd.num_commands] == NULL) return ERR_MEMORY;

            cmd.argc = 0;

            pipe_token = strtok_r(NULL, PIPE_STRING, &pipe_saveptr);
        }

//        printf("Contents\n");
//        for (int i = 0; i < cmd.num_commands; i++) 
//        {
//            printf("Command %d: ", i);
//            for (int j = 0; cmd.argv[i][j] != NULL; j++) {
//                printf("%s ", cmd.argv[i][j]);
//            }
//            printf("\n");
//        }
//
//        printf("\n");

        // No command logic 
        if (cmd.num_commands == 0) continue;

        // Exit command logic
        if (strcmp(cmd.argv[0][0], EXIT_CMD) == 0) break;

        // cd command logic
        // we assume len(cmd.arhv[0]) > 1 wich can potenitally crash
        if (strcmp(cmd.argv[0][0], CD_CMD) == 0) {
            // execute cd with chdir
            if (chdir(cmd.argv[0][1]) != 0) {
                //printf("Error changing dir\n");
            }
        } else { // pipeline logic 
            execute_pipeline(&cmd);
        }

        cmd.num_commands = 0;
        cmd.argc = 0;
    }
    
    // Free memory for each argv and reset cmd values
    for (int i = 0; i < cmd.num_commands; i++) {
        // Free each argument in the argv array
        for (int j = 0; cmd.argv[i][j] != NULL; j++) {
            free(cmd.argv[i][j]);  // Free the j-th argument of the i-th command
        }
        // Free the argv array itself
        free(cmd.argv[i]);  // Free the array of pointers for the i-th command
    }

    free(cmd._cmd_buffer);
    return OK;
}

void execute_pipeline(cmd_buff_t* cmd)
{
    int pipes[cmd->num_commands - 1][2];  // Array of pipes
    pid_t pids[cmd->num_commands];        // Array to store process IDs
    
    // Create all necessary pipes
    for (int i = 0; i < cmd->num_commands - 1; i++) 
    {
        if (pipe(pipes[i]) == -1) return ERR_PIPE;
    }
    
    // Create processes for each command
    for (int i = 0; i < cmd->num_commands; i++) 
    {
        pids[i] = fork();
        if (pids[i] == -1) return ERR_FORK;
        
        // Child process
        if (pids[i] == 0) 
        {  
            // Set up input pipe for all except first process
            if (i > 0) 
            {
                dup2(pipes[i-1][0], STDIN_FILENO);
                close(pipes[i - 1][0]);
            }

            // Set up output pipe for all except last process
            if (i < cmd->num_commands - 1)
            {
                dup2(pipes[i][1], STDOUT_FILENO);
                close(pipes[i][1]);
            } 
            
            // Close all pipe ends in child
            for (int j = 0; j < cmd->num_commands - 1; j++) {
                if (j != i - 1) close(pipes[j][0]);
                if (j != i) close(pipes[j][1]);
            }

            // Execute command
            execvp(cmd->argv[i][0], cmd->argv[i]);
            exit(ERR_EXEC_CMD); // if execvp returns an error has occured
        }
    }

    // Parent process: close all pipe ends
    for (int i = 0; i < cmd->num_commands - 1; i++) 
    {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    // Wait for all children
    for (int i = 0; i < cmd->num_commands; i++) waitpid(pids[i], NULL, 0);
}