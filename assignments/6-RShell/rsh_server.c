
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/un.h>
#include <fcntl.h>

//INCLUDES for extra credit
//#include <signal.h>
//#include <pthread.h>
//-------------------------

#include "dshlib.h"
#include "rshlib.h"


/*
 * start_server(ifaces, port, is_threaded)
 *      ifaces:  a string in ip address format, indicating the interface
 *              where the server will bind.  In almost all cases it will
 *              be the default "0.0.0.0" which binds to all interfaces.
 *              note the constant RDSH_DEF_SVR_INTFACE in rshlib.h
 * 
 *      port:   The port the server will use.  Note the constant 
 *              RDSH_DEF_PORT which is 1234 in rshlib.h.  If you are using
 *              tux you may need to change this to your own default, or even
 *              better use the command line override -s implemented in dsh_cli.c
 *              For example ./dsh -s 0.0.0.0:5678 where 5678 is the new port  
 * 
 *      is_threded:  Used for extra credit to indicate the server should implement
 *                   per thread connections for clients  
 * 
 *      This function basically runs the server by: 
 *          1. Booting up the server
 *          2. Processing client requests until the client requests the
 *             server to stop by running the `stop-server` command
 *          3. Stopping the server. 
 * 
 *      This function is fully implemented for you and should not require
 *      any changes for basic functionality.  
 * 
 *      IF YOU IMPLEMENT THE MULTI-THREADED SERVER FOR EXTRA CREDIT YOU NEED
 *      TO DO SOMETHING WITH THE is_threaded ARGUMENT HOWEVER.  
 */
int start_server(char *ifaces, int port, int is_threaded){
    int svr_socket;
    int rc;

    //
    //TODO:  If you are implementing the extra credit, please add logic
    //       to keep track of is_threaded to handle this feature
    //

    svr_socket = boot_server(ifaces, port);
    
    if (svr_socket < 0){
        int err_code = svr_socket;  //server socket will carry error code
        return err_code;
    }
    //printf("Server Socket: %d\n", svr_socket);

    rc = process_cli_requests(svr_socket);

    stop_server(svr_socket);


    return rc;
}

/*
 * stop_server(svr_socket)
 *      svr_socket: The socket that was created in the boot_server()
 *                  function. 
 * 
 *      This function simply returns the value of close() when closing
 *      the socket.  
 */
int stop_server(int svr_socket){
    return close(svr_socket);
}

/*
 * boot_server(ifaces, port)
 *      ifaces & port:  see start_server for description.  They are passed
 *                      as is to this function.   
 * 
 *      This function "boots" the rsh server.  It is responsible for all
 *      socket operations prior to accepting client connections.  Specifically: 
 * 
 *      1. Create the server socket using the socket() function. 
 *      2. Calling bind to "bind" the server to the interface and port
 *      3. Calling listen to get the server ready to listen for connections.
 * 
 *      after creating the socket and prior to calling bind you might want to 
 *      include the following code:
 * 
 *      int enable=1;
 *      setsockopt(svr_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
 * 
 *      when doing development you often run into issues where you hold onto
 *      the port and then need to wait for linux to detect this issue and free
 *      the port up.  The code above tells linux to force allowing this process
 *      to use the specified port making your life a lot easier.
 * 
 *  Returns:
 * 
 *      server_socket:  Sockets are just file descriptors, if this function is
 *                      successful, it returns the server socket descriptor, 
 *                      which is just an integer.
 * 
 *      ERR_RDSH_COMMUNICATION:  This error code is returned if the socket(),
 *                               bind(), or listen() call fails. 
 * 
 */
int boot_server(char *ifaces, int port){
    // int socket(int domain, int type, int protocol);
    // 1. Create the server socket using the socket() function. 
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) return ERR_RDSH_COMMUNICATION;

    // tells linux to force allowing this process to use the specified port
    int enable = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));

    // 2. Calling bind to "bind" the server to the interface and port
    // int bind(int socket, const struct sockaddr *address, socklen_t address_len);
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr)); // Clear the structure

    // Set up the server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    // Bind to all interfaces (0.0.0.0) or a specific interface
    if (strcmp(ifaces, RDSH_DEF_SVR_INTFACE) == 0) {
        server_addr.sin_addr.s_addr = INADDR_ANY; // Bind to all interfaces
    } else {
        server_addr.sin_addr.s_addr = inet_addr(ifaces); // Bind to a specific interface
    }

    int bind_result = bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (bind_result < 0) return ERR_RDSH_COMMUNICATION;

    // 3. Calling listen to get the server ready to listen for connections.
    if (listen(server_socket, BACKLOG) < 0) return ERR_RDSH_COMMUNICATION;
    return server_socket;
}

/*
 * process_cli_requests(svr_socket)
 *      svr_socket:  The server socket that was obtained from boot_server()
 *   
 *  This function handles managing client connections.  It does this using
 *  the following logic
 * 
 *      1.  Starts a while(1) loop:
 *  
 *          a. Calls accept() to wait for a client connection. Recall that 
 *             the accept() function returns another socket specifically
 *             bound to a client connection. 
 *          b. Calls exec_client_requests() to handle executing commands
 *             sent by the client. It will use the socket returned from
 *             accept().
 *          c. Loops back to the top (step 2) to accept connecting another
 *             client.  
 * 
 *          note that the exec_client_requests() return code should be
 *          negative if the client requested the server to stop by sending
 *          the `stop-server` command.  If this is the case step 2b breaks
 *          out of the while(1) loop. 
 * 
 *      2.  After we exit the loop, we need to cleanup.  Dont forget to 
 *          free the buffer you allocated in step #1.  Then call stop_server()
 *          to close the server socket. 
 * 
 *  Returns:
 * 
 *      OK_EXIT:  When the client sends the `stop-server` command this function
 *                should return OK_EXIT. 
 * 
 *      ERR_RDSH_COMMUNICATION:  This error code terminates the loop and is
 *                returned from this function in the case of the accept() 
 *                function failing. 
 * 
 *      OTHERS:   See exec_client_requests() for return codes.  Note that positive
 *                values will keep the loop running to accept additional client
 *                connections, and negative values terminate the server. 
 * 
 */
int process_cli_requests(int svr_socket){

    struct sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(client_addr)); // Clear the structure

    socklen_t client_addr_len = sizeof(client_addr);

    int cli_socket, rc; 
    while (1)
    {
        // a. Calls accept() to wait for a client connection. 
        cli_socket = accept(svr_socket, (struct sockaddr *)&client_addr, &client_addr_len);
        if (cli_socket < 0) return ERR_RDSH_COMMUNICATION;
        //printf("Client Socket: %d\n", cli_socket);

        // b. Calls exec_client_requests() to handle executing commands sent by the client. 
        //      It will use the socket returned from accept().
        // c. Loops back to the top (step 2) to accept connecting another client.
        rc = exec_client_requests(cli_socket);

        if (rc == OK_EXIT) {
            // Client sent the `stop-server` command
            printf("Client requested server to stop. Shutting down...\n");
            close(cli_socket); // Close the client socket
            break; // Exit the loop to stop the server
        } else if (rc < 0) {
            // An error occurred in exec_client_requests()
            printf("Error handling client requests. Shutting down...\n");
            close(cli_socket); // Close the client socket
            return rc; // Return the error code
        }

        // Step 1.d: Close the client socket after handling the request
        close(cli_socket);
        //printf("Client disconnected\n");
    }

    return OK_EXIT;
}

/*
 * exec_client_requests(cli_socket)
 *      cli_socket:  The server-side socket that is connected to the client
 *   
 *  This function handles accepting remote client commands. The function will
 *  loop and continue to accept and execute client commands.  There are 2 ways
 *  that this ongoing loop accepting client commands ends:
 * 
 *      1.  When the client executes the `exit` command, this function returns
 *          to process_cli_requests() so that we can accept another client
 *          connection. 
 *      2.  When the client executes the `stop-server` command this function
 *          returns to process_cli_requests() with a return code of OK_EXIT
 *          indicating that the server should stop. 
 * 
 *  Note that this function largely follows the implementation of the
 *  exec_local_cmd_loop() function that you implemented in the last 
 *  shell program deliverable. The main difference is that the command will
 *  arrive over the recv() socket call rather than reading a string from the
 *  keyboard. 
 * 
 *  This function also must send the EOF character after a command is
 *  successfully executed to let the client know that the output from the
 *  command it sent is finished.  Use the send_message_eof() to accomplish 
 *  this. 
 * 
 *  Of final note, this function must allocate a buffer for storage to 
 *  store the data received by the client. For example:
 *     io_buff = malloc(RDSH_COMM_BUFF_SZ);
 *  And since it is allocating storage, it must also properly clean it up
 *  prior to exiting.
 * 
 *  Returns:
 * 
 *      OK:       The client sent the `exit` command.  Get ready to connect
 *                another client. 
 *      OK_EXIT:  The client sent `stop-server` command to terminate the server
 * 
 *      ERR_RDSH_COMMUNICATION:  A catch all for any socket() related send
 *                or receive errors. 
 */
int exec_client_requests(int cli_socket) {
    char *io_buff = malloc(RDSH_COMM_BUFF_SZ); // Buffer for receiving commands
    if (io_buff == NULL) return ERR_MEMORY;

    int rc = OK;
    cmd_buff_t cmd;
    cmd.argc = 0;
    cmd.num_commands = 0;

    // Allocate memory for command buffer
    cmd._cmd_buffer = malloc(SH_CMD_MAX * sizeof(char));
    if (cmd._cmd_buffer == NULL) {
        free(io_buff);
        return ERR_MEMORY;
    }

    // Allocate memory for argv
    cmd.argv[cmd.num_commands] = malloc(CMD_ARGV_MAX * sizeof(char*));
    if (cmd.argv[cmd.num_commands] == NULL) {
        free(io_buff);
        free(cmd._cmd_buffer);
        return ERR_MEMORY;
    }

    while (1)
    {
        // Get command from client
        int recv_size = recv(cli_socket, io_buff, RDSH_COMM_BUFF_SZ, 0);
        if (recv_size < 0)
        {
            rc = ERR_RDSH_COMMUNICATION;
            break;
        } else if (recv_size == 0)
        {
            rc = OK;
            break; // client disconnected 
        }
        
        // Null-terminate the received command
        io_buff[recv_size] = '\0';

        // Remove trailing newline (if any)
        io_buff[strcspn(io_buff, "\n")] = '\0';

        // Check for empty command
        if (io_buff[0] == '\0') {
            printf(CMD_WARN_NO_CMD);
            continue;
        }

        int parse_result = parse_buffer_into_cmd(io_buff, &cmd);
        if (parse_result != OK)
        {
            rc = ERR_EXEC_CMD;
            break;
        }

        // Handle special commands: exit and stop-server
        if (cmd.num_commands > 0 && strcmp(cmd.argv[0][0], EXIT_CMD) == 0) {
            rc = OK; // Exit the loop and return to accept another client
            break;
        } else if (cmd.num_commands > 0 && strcmp(cmd.argv[0][0], STOP_SERVER_CMD) == 0) {
            rc = OK_EXIT; // Exit the loop and stop the server
            break;
        }

        // Execute the command pipeline
        if (cmd.num_commands > 0) {
            rsh_execute_pipeline(cli_socket, &cmd);
        }

        // Send EOF character to indicate end of command output
        if (send_message_eof(cli_socket) != OK) {
            rc = ERR_RDSH_COMMUNICATION;
            break;
        }

        // Reset command buffer for the next iteration
        cmd.num_commands = 0;
        cmd.argc = 0;

    }

    // Free allocated memory
    for (int i = 0; i < cmd.num_commands; i++) {
        for (int j = 0; cmd.argv[i][j] != NULL; j++) {
            free(cmd.argv[i][j]);
        }
        free(cmd.argv[i]);
    }
    free(cmd._cmd_buffer);
    free(io_buff);

    return rc;
}

/*
 * send_message_eof(cli_socket)
 *      cli_socket:  The server-side socket that is connected to the client

 *  Sends the EOF character to the client to indicate that the server is
 *  finished executing the command that it sent. 
 * 
 *  Returns:
 * 
 *      OK:  The EOF character was sent successfully. 
 * 
 *      ERR_RDSH_COMMUNICATION:  The send() socket call returned an error or if
 *           we were unable to send the EOF character. 
 */
int send_message_eof(int cli_socket){
    int bytes_sent = send(cli_socket, &RDSH_EOF_CHAR, 1, 0);
    if (bytes_sent == 1) {
        return OK; // Successfully sent the EOF character
    } else {
        return ERR_RDSH_COMMUNICATION; // Communication error
    }
}

/*
 * send_message_string(cli_socket, char *buff)
 *      cli_socket:  The server-side socket that is connected to the client
 *      buff:        A C string (aka null terminated) of a message we want
 *                   to send to the client. 
 *   
 *  Sends a message to the client.  Note this command executes both a send()
 *  to send the message and a send_message_eof() to send the EOF character to
 *  the client to indicate command execution terminated. 
 * 
 *  Returns:
 * 
 *      OK:  The message in buff followed by the EOF character was 
 *           sent successfully. 
 * 
 *      ERR_RDSH_COMMUNICATION:  The send() socket call returned an error or if
 *           we were unable to send the message followed by the EOF character. 
 */
int send_message_string(int cli_socket, char *buff) {
    // Step 1: Send the message to the client
    int bytes_sent = send(cli_socket, buff, strlen(buff), 0);
    if (bytes_sent < 0) {
        perror("Failed to send message");
        return ERR_RDSH_COMMUNICATION; // Return error if send() fails
    }

    // Step 2: Send the EOF character to indicate the end of the message
    if (send_message_eof(cli_socket) != OK) {
        return ERR_RDSH_COMMUNICATION; // Return error if send_message_eof() fails
    }

    return OK; // Success
}


/*
 * rsh_execute_pipeline(int cli_sock, command_list_t *clist)
 *      cli_sock:    The server-side socket that is connected to the client
 *      clist:       The command_list_t structure that we implemented in
 *                   the last shell. 
 *   
 *  This function executes the command pipeline.  It should basically be a
 *  replica of the execute_pipeline() function from the last deliverable. 
 *  The only thing different is that you will be using the cli_sock as the
 *  main file descriptor on the first executable in the pipeline for STDIN,
 *  and the cli_sock for the file descriptor for STDOUT, and STDERR for the
 *  last executable in the pipeline.  See picture below:  
 * 
 *      
 *┌───────────┐                                                    ┌───────────┐
 *│ cli_sock  │                                                    │ cli_sock  │
 *└─────┬─────┘                                                    └────▲──▲───┘
 *      │   ┌──────────────┐     ┌──────────────┐     ┌──────────────┐  │  │    
 *      │   │   Process 1  │     │   Process 2  │     │   Process N  │  │  │    
 *      │   │              │     │              │     │              │  │  │    
 *      └───▶stdin   stdout├─┬──▶│stdin   stdout├─┬──▶│stdin   stdout├──┘  │    
 *          │              │ │   │              │ │   │              │     │    
 *          │        stderr├─┘   │        stderr├─┘   │        stderr├─────┘    
 *          └──────────────┘     └──────────────┘     └──────────────┘   
 *                                                      WEXITSTATUS()
 *                                                      of this last
 *                                                      process to get
 *                                                      the return code
 *                                                      for this function       
 * 
 *  Returns:
 * 
 *      EXIT_CODE:  This function returns the exit code of the last command
 *                  executed in the pipeline.  If only one command is executed
 *                  that value is returned.  Remember, use the WEXITSTATUS()
 *                  macro that we discussed during our fork/exec lecture to
 *                  get this value. 
 */
int rsh_execute_pipeline(int cli_sock, cmd_buff_t *cmd) {
    int pipes[cmd->num_commands - 1][2];  // Array of pipes
    pid_t pids[cmd->num_commands];        // Array to store process IDs

    // Create all necessary pipes
    for (int i = 0; i < cmd->num_commands - 1; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            return ERR_PIPE; // Return error if pipe creation fails
        }
    }

    // Create processes for each command
    for (int i = 0; i < cmd->num_commands; i++) {
        pids[i] = fork();
        if (pids[i] == -1) {
            perror("fork");
            return ERR_FORK; // Return error if fork fails
        }

        // Child process
        if (pids[i] == 0) {
            // Set up input redirection for the first process
            if (i == 0 && cli_sock != -1) {
                dup2(cli_sock, STDIN_FILENO); // Redirect stdin to the client socket
            }

            // Set up output redirection for the last process
            if (i == cmd->num_commands - 1 && cli_sock != -1) {
                dup2(cli_sock, STDOUT_FILENO); // Redirect stdout to the client socket
                dup2(cli_sock, STDERR_FILENO); // Redirect stderr to the client socket
            }

            // Set up input pipe for all except the first process
            if (i > 0) {
                dup2(pipes[i - 1][0], STDIN_FILENO);
                close(pipes[i - 1][0]);
            }

            // Set up output pipe for all except the last process
            if (i < cmd->num_commands - 1) {
                dup2(pipes[i][1], STDOUT_FILENO);
                close(pipes[i][1]);
            }

            // Close all pipe ends in the child
            for (int j = 0; j < cmd->num_commands - 1; j++) {
                if (j != i - 1) close(pipes[j][0]);
                if (j != i) close(pipes[j][1]);
            }

            // Execute the command
            execvp(cmd->argv[i][0], cmd->argv[i]);
            perror("execvp"); // If execvp fails
            exit(ERR_EXEC_CMD); // Exit with error code
        }
    }

    // Parent process: close all pipe ends
    for (int i = 0; i < cmd->num_commands - 1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    // Wait for all children and get the exit code of the last process
    int status;
    for (int i = 0; i < cmd->num_commands; i++) {
        waitpid(pids[i], &status, 0);
    }

    // Return the exit code of the last command in the pipeline
    if (WIFEXITED(status)) {
        return WEXITSTATUS(status); // Return the exit code of the last process
    } else {
        return ERR_EXEC_CMD; // Return error if the process did not exit normally
    }
}

/**************   OPTIONAL STUFF  ***************/
/****
 **** NOTE THAT THE FUNCTIONS BELOW ALIGN TO HOW WE CRAFTED THE SOLUTION
 **** TO SEE IF A COMMAND WAS BUILT IN OR NOT.  YOU CAN USE A DIFFERENT
 **** STRATEGY IF YOU WANT.  IF YOU CHOOSE TO DO SO PLEASE REMOVE THESE
 **** FUNCTIONS AND THE PROTOTYPES FROM rshlib.h
 **** 
 */

/*
 * rsh_match_command(const char *input)
 *      cli_socket:  The string command for a built-in command, e.g., dragon,
 *                   cd, exit-server
 *   
 *  This optional function accepts a command string as input and returns
 *  one of the enumerated values from the BuiltInCmds enum as output. For
 *  example:
 * 
 *      Input             Output
 *      exit              BI_CMD_EXIT
 *      dragon            BI_CMD_DRAGON
 * 
 *  This function is entirely optional to implement if you want to handle
 *  processing built-in commands differently in your implementation. 
 * 
 *  Returns:
 * 
 *      BI_CMD_*:   If the command is built-in returns one of the enumeration
 *                  options, for example "cd" returns BI_CMD_CD
 * 
 *      BI_NOT_BI:  If the command is not "built-in" the BI_NOT_BI value is
 *                  returned. 
 */
Built_In_Cmds rsh_match_command(const char *input)
{
    return BI_NOT_IMPLEMENTED;
}

/*
 * rsh_built_in_cmd(cmd_buff_t *cmd)
 *      cmd:  The cmd_buff_t of the command, remember, this is the 
 *            parsed version fo the command
 *   
 *  This optional function accepts a parsed cmd and then checks to see if
 *  the cmd is built in or not.  It calls rsh_match_command to see if the 
 *  cmd is built in or not.  Note that rsh_match_command returns BI_NOT_BI
 *  if the command is not built in. If the command is built in this function
 *  uses a switch statement to handle execution if appropriate.   
 * 
 *  Again, using this function is entirely optional if you are using a different
 *  strategy to handle built-in commands.  
 * 
 *  Returns:
 * 
 *      BI_NOT_BI:   Indicates that the cmd provided as input is not built
 *                   in so it should be sent to your fork/exec logic
 *      BI_EXECUTED: Indicates that this function handled the direct execution
 *                   of the command and there is nothing else to do, consider
 *                   it executed.  For example the cmd of "cd" gets the value of
 *                   BI_CMD_CD from rsh_match_command().  It then makes the libc
 *                   call to chdir(cmd->argv[1]); and finally returns BI_EXECUTED
 *      BI_CMD_*     Indicates that a built-in command was matched and the caller
 *                   is responsible for executing it.  For example if this function
 *                   returns BI_CMD_STOP_SVR the caller of this function is
 *                   responsible for stopping the server.  If BI_CMD_EXIT is returned
 *                   the caller is responsible for closing the client connection.
 * 
 *   AGAIN - THIS IS TOTALLY OPTIONAL IF YOU HAVE OR WANT TO HANDLE BUILT-IN
 *   COMMANDS DIFFERENTLY. 
 */
Built_In_Cmds rsh_built_in_cmd(cmd_buff_t *cmd)
{
    return BI_NOT_IMPLEMENTED;
}

int parse_buffer_into_cmd(char* buff, cmd_buff_t *cmd)
{
    int rc = OK;
    // Parse the command into cmd_buff_t structure
    char *pipe_token, *pipe_saveptr;
    pipe_token = strtok_r(buff, PIPE_STRING, &pipe_saveptr);

    while (pipe_token) {
        if (cmd->num_commands > CMD_MAX) {
            printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);
            rc = ERR_TOO_MANY_COMMANDS;
            break;
        }

        char *cmd_ptr = pipe_token;
        while (*cmd_ptr) {
            if (cmd->argc > ARG_MAX) {
                rc = ERR_CMD_OR_ARGS_TOO_BIG;
                break;
            }

            // Skip leading spaces
            while (*cmd_ptr == ' ') cmd_ptr++;

            if (*cmd_ptr == '\0') break; // No more tokens

            // Handle quoted tokens
            if (*cmd_ptr == DOUBLE_QUOTE_CHAR || *cmd_ptr == SINGLE_QUOTE_CHAR) {
                char quote = *cmd_ptr++;
                char *start = cmd_ptr;
                while (*cmd_ptr && *cmd_ptr != quote) cmd_ptr++;

                if (*cmd_ptr == quote) {
                    int length = (int)(cmd_ptr - start);
                    char *token = (char *)malloc(length + 1);
                    if (token) {
                        strncpy(token, start, length);
                        token[length] = '\0';
                        cmd->argv[cmd->num_commands][cmd->argc++] = token;
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
                    cmd->argv[cmd->num_commands][cmd->argc++] = token;
                }
            }
        }

        // Terminate the argument list
        cmd->argv[cmd->num_commands][cmd->argc] = NULL;

        // Allocate memory for the next command's argv
        cmd->argv[++cmd->num_commands] = malloc(CMD_ARGV_MAX * sizeof(char*));
        if (cmd->argv[cmd->num_commands] == NULL) {
            rc = ERR_MEMORY;
            break;
        }

        cmd->argc = 0;
        pipe_token = strtok_r(NULL, PIPE_STRING, &pipe_saveptr);
    }

    return rc;
}