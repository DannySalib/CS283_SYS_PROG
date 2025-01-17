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
int reverse_words(char *, int, int, char *);
int print_words_and_their_lengths(char *, int, int);

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
        // 1. It is not a trailing space char 
        // 2. treat tabs as space chars 
        if (*curr_char  == '\t') {
            *curr_char = ' '; // follow logic as if it were a space key
        }
        
        // determine if there is a trailing space key
        if (*curr_char != ' ') {
            is_valid_char = true;
            has_preceeding_space_char = false;
        } else {
            if (!has_preceeding_space_char) {
                has_preceeding_space_char = true;
                is_valid_char = true;
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

    // fill remaining memory in buffer with '.'
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
    if (str_len > len) {
	fprintf(stderr, "Cannot count user string: string length exceeds buffer size");
	return -1;
    }

    bool in_word;
    int i, count; 
    char* curr_buff;

    i = 0;
    count = 0;
    curr_buff = buff;

    while (i < str_len) {

	// if we iterate to a space key, we're no longer in a word
	// if we iterate to a valid char, 
	// 	and last char was a space key, we've found a new word
	if (*curr_buff == ' ') {
		in_word = false;
	} else if (!in_word) { // assume it is a valid char 
		in_word = true;
		count++;
	}	

	// move to next char address 
	i++;
	curr_buff = buff + (i * sizeof(char));
    }
	
    return count;
}

//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS
int reverse_words(char *buff, int len, int str_len, char* reversed_word){
    if (str_len > len) {
        fprintf(stderr, "Cannot count user string: string length exceeds buffer size");
        return -1;
    }

    char *curr_buff, *curr_reversed_word;
    bool new_word;
    int i, j;

    curr_buff = buff;
    curr_reversed_word = reversed_word;
    i = -1;
    j = 0; // represnts the beginning of the current word
    while(++i < str_len) {
        // a new word is found when the current character is a space
        //  or when the next character on the buffer is the end of the string
        new_word = (*curr_buff == ' ') || ((i+1) == str_len);
        
        if (new_word) {

            for(int k = i; k >= j; k--) *curr_reversed_word++ = *curr_buff--;
            
            j = i; // the beginning of the next current word is the current index
            curr_buff = buff + (i * sizeof(char)); // return to our current buffer

            // add space to end of reversed word
            *(curr_reversed_word++) = ' ';
        } 

        curr_buff++;
    }

    return 0; //success! 
}

int print_words_and_their_lengths(char* buff, int len, int str_len) {
    printf("Word Print\n");
    printf("-----------\n");

    if (str_len > len) {
        fprintf(stderr, "Cannot count user string: string length exceeds buffer size");
        return -1;
    }

    char *curr_buff;
    int i, j, word_len;
    bool new_word;
    curr_buff = buff;
    i = -1;
    j = 0; // index of the beginning of the word
        

    while (++i <= str_len) {
        new_word = (*curr_buff == ' ') || (i == str_len);
        if (new_word) {
            word_len = i - j;

            curr_buff = buff + (j * sizeof(char)); // go to the beginning of the word 
            while (j++ < i) printf("%c", *curr_buff++);

            printf(" (%d)\n", word_len);
        }
        curr_buff++;
    }

    return 0;
}

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
    // printf("user str len %d\n", user_str_len);
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
        case 'r':
            // create memory to store the reversed result 
            char* reversed_word;
            reversed_word = (char *)malloc(BUFFER_SZ);
            if (reversed_word == NULL) { // malloc didnt work 
            fprintf(stderr, "ERROR: Memory could not be allocated for reverse_words function.\n");
            exit(99);
            }

            rc = reverse_words(buff, BUFFER_SZ, user_str_len, reversed_word);  //you need to implement
            if (rc < 0){
                printf("Error reversing words, rc = %d", rc);
                exit(2);
            }
            printf("Reversed Words:%s\n", reversed_word);
            free(reversed_word);
            break;
        
        case 'w':
            rc = print_words_and_their_lengths(buff, BUFFER_SZ, user_str_len); 
            if (rc < 0){
                printf("Error printing words/lengths, rc = %d", rc);
                exit(2);
            }
            break;

        default:
            usage(argv[0]);
            exit(1);
    }

    //TODO:  #6 Dont forget to free your buffer before exiting
    print_buff(buff,BUFFER_SZ);
    free(buff);
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?

/*
 * By passing the length explicitly, the helper functions don't need to rely on 
 *    assumptions about the buffer size. Even though buff is defined as 50 bytes, 
 *    the actual data being processed most liekly does not use all 50 bytes. By passing the length, 
 *    you can handle only the valid portion of the buffer without relying on any assumptions.
 *
 * If the buffer size changes in the future or the function is called with a buffer of different sizes, 
 *    the existing helper functions can accommodate these changes without requiring updates 
 *    to the code itsellf. 
 *    
 * By accounting for these factors, we are left with less hardcoded functions, which
 *    allows for a robust program.
*/