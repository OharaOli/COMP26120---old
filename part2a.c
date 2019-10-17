#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {

    //Variable used to check if there is an argument as expected
    int error = 0;

    //Catch the case where no additional arguments are given
    if(argc == 1) 
    {
        printf("Error: expected some command-line parameters\n");
        error = 1;
    }

    //Store the length of the longest argument and its position in the array
    int longestLength = 0;
    int longestPos = 0;
 
    //Initialise a counter for the loop and a pointer to the word being tested
    int i;
    char* word;

    for(i = 1; i < argc; i++) 
    {
        word = argv[i];
        int length = strlen(word);
        if(length > longestLength) 
        {
            longestLength = length;
            longestPos = i;
        }
    }
 
    //Print the length of the longest argument and what word it relates to
    if(error == 0)
    {
       printf("Max length is %d for string: %s\n", longestLength, argv[longestPos]);
    }
}


