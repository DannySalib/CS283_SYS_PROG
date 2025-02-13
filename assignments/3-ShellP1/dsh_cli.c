#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "dshlib.h"

/*
 * Implement your main function by building a loop that prompts the
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.  Since we want fgets to also handle
 * end of file so we can run this headless for testing we need to check
 * the return code of fgets.  I have provided an example below of how
 * to do this assuming you are storing user input inside of the cmd_buff
 * variable.
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
 *
 *   Expected output:
 *
 *      CMD_OK_HEADER      if the command parses properly. You will
 *                         follow this by the command details
 *
 *      CMD_WARN_NO_CMD    if the user entered a blank command
 *      CMD_ERR_PIPE_LIMIT if the user entered too many commands using
 *                         the pipe feature, e.g., cmd1 | cmd2 | ... |
 *
 *  See the provided test cases for output expectations.
 */
int main()
{
    char *cmd_buff;
    int rc = 0;
    command_list_t clist;

    cmd_buff = malloc(ARG_MAX);
    int i = 0;
    char curr_exe[EXE_MAX];
    bool exit_requested;
    exit_requested = false;
    
    while(1){
        printf("%s", SH_PROMPT);
        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
            printf("\n");
            break;
        }

        //remove the trailing \n from cmd_buff
        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
        build_cmd_list(cmd_buff, &clist);

        // Check if our number of commands are as expected 
        if (clist.num < 0) {
            fprintf(stderr, "Error: clist.num is out of bounds: %d\n", clist.num);
            exit(-4);
        } 
        
        // iterate through each command from pipe
        while (i < clist.num) {
            
            // something went wrong in build_cmd_list when adding an exe 
            if (clist.commands[i].exe[0] == '\0') {
                fprintf(stderr, "Error: Empty exe at index %d\n", i);
                continue;
            }

            strncpy(curr_exe, clist.commands[i].exe, EXE_MAX - 1); // todo refrence instead of copy
            curr_exe[EXE_MAX - 1] = '\0';

//            printf("i %d\n", i);
//            printf("EXE '%s'\n", curr_exe);
//            printf("Args '%s'\n", clist.commands[i].args);
            
            // process current exe word
            if (strcmp(curr_exe, EXIT_CMD) == 0) {
                //printf("Exiting\n");
                exit_requested = true;
                break;
            }
            
            i++;
        }

        // end program IFF user requested to exit
        if (exit_requested) {
            break;
        }
        
        // printing result accordingly 
        if (clist.num == 0) {
            printf(CMD_WARN_NO_CMD);
        } else if (clist.num > CMD_MAX) {
            printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);
        } else {
            printf(CMD_OK_HEADER, clist.num);

            // print each command user inputed in the specified way:
            for (int x = 0; x < clist.num; x++) {

                printf("<%d> %s", x + 1, clist.commands[x].exe);

                if (clist.commands[x].args[0] == '\0') {
                    printf("\n");
                } else {
                    printf(" [%s]\n", clist.commands[x].args);
                }
            }
        }

        i = 0;
    }

    free(cmd_buff);  // Free allocated memory before exiting
    exit(OK);
}