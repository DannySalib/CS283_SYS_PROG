#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *, int, int);
//add additional prototypes here

int setup_buff(char *buff, char *user_str, int len){
    //TODO: #4:  Implement the setup buff as per the directions
   	
    int curr_buff_len = 0;
    int i = 0;    

    bool has_preceeding_space_char = false;
    bool is_valid_char = false;

    char* curr_char = user_str;
    char* curr_buff_char = buff;

    while (*curr_char != '\0') {
	if (curr_buff_len > len) {
		// we have exceeded max buffer size 
		return -1; // The user supplied string is too large
	}

	// A char is valid if it meets the following conditions 
	// 1. It is not a tab char 
	// 2. It is not a trailing space char 
	if (*curr_char != '\t') {
		if (*curr_char != ' ') {
			is_valid_char = true;
			has_preceeding_space_char = false;
		} else {
			if (!has_preceeding_space_char) {
				has_preceeding_space_char = true;
				is_valid_char = true;
			}
		}
	}

	if (is_valid_char) {
		// update the buffer @ index with the validated current char 
		*curr_buff_char = *curr_char;
		curr_buff_len++;
	}
	
	// update for next iteration 
	i++;
	curr_char = user_str + (i * sizeof(char));
	curr_buff_char = buff + (i * sizeof(char));
    }

    // fill remainding memory in buffer with '.'
    while (i < len) {
	*curr_buff_char = '.';

	i++;
	curr_buff_char = buff + (i * sizeof(char));
    }

    return curr_buff_len;
}

void print_buff(char *buff, int len){
    printf("Buffer:  ");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    putchar('\n');
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

int count_words(char *buff, int len, int str_len){
    //YOU MUST IMPLEMENT
    return 0;
}

//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS

int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    /* There are 2 possible cases in this condition
     * 1. argc < 2 : In an OR statement, Compiler skips next condition thus no errors 
     * 2. argc >= 2 : Compiler considers next condition. This condition will always compute when len(argv) > 2 thus no errors 
    */
    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO:  #2 Document the purpose of the if statement below
    /*
     * At this point this condition MUST be true 
     * 1. argc >= 2 & *argv[1] == '-'
     *
     * We can consider 2 possible values of argc
     * 2. argc == 2
     * 3. argc > 2 ==> argc in {3, 4, ..., n}
     *
     * We can now infer our code with the following 
     * IF argc < 3 THEN argc MUST BE 2
     * 	- We do not have a 3rd argument: our word that needs to processed
     *	- Therefore we print usage error and exit
     *
     * Now we can continue with our script by storing 3rd arg -> input_string and process it
     */
    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string

    //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 99
    // CODE GOES HERE FOR #3
    
    buff = (char *)malloc(BUFFER_SZ * sizeof(char));
    if (buff == NULL) { // malloc didnt work 
	fprintf(stderr, "ERROR: Memory could not be allocated to buffer.\n");
	exit(99);
    }


    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos
    printf("user str len %d\n", user_str_len);
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d", user_str_len);
        exit(2);
    }

    switch (opt){
        case 'c':
            rc = count_words(buff, BUFFER_SZ, user_str_len);  //you need to implement
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;

        //TODO:  #5 Implement the other cases for 'r' and 'w' by extending
        //       the case statement options
        default:
            usage(argv[0]);
            exit(1);
    }

    //TODO:  #6 Dont forget to free your buffer before exiting
    print_buff(buff,BUFFER_SZ);
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
//          PLACE YOUR ANSWER HERE
