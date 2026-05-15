#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define GREEN   "\033[0;32m"
#define YELLOW  "\033[0;33m"
#define GRAY    "\033[0;90m"
#define RESET   "\033[0m"

//function to check if the letter exists
bool exists(char user_input, char *random_word) {
    //two ways
    //1. loop through the entire word and see if it exists
    //2. make a hashmap sort of structure that contains the occurence of each letter and decrement everytime (would have to reset every guess)
    //2a. additionally, i could make an array that holds each occurrence and compare through there
    size_t size = strlen(random_word);

    for (size_t i = 0; i < size; i++) {
        if (random_word[i] == user_input) {
            return true;
        }
    }

    return false; 
}

int main() {
    int line_count = 0;

    //read through the file and find the number of items.
    FILE *f = fopen("word.txt", "rb"); //open words.txt for reading only
    if (f == NULL) {
        perror("fopen");
        return -1;
    }
    char *linefeed = NULL;

    size_t size = 0;
    while(getline(&linefeed, &size, f) != -1) { //-1 means EOF
        line_count++;
    }

    //get a random number based on how many words the word.txt file has
    //right now it's 8 because of testing so 8 for now, will change later. probably will add a function above main but not sure yet.
    srand(time(NULL)); //to have a random number every time
    int random_number = rand()%line_count;

    //get the random generated word
    char *random_generated_word = calloc(6, sizeof(char));

    //loop through the file until we random_number th word
    int i = 0;
    linefeed = NULL;
    size = 6;
    f = freopen("words.txt", "rb", f);
    while (i < random_number) {
        i++;
        getline(&linefeed, &size, f);
    }
    fclose(f); //close the file pointer
    //make random_generated_word the linefeed
    strncpy(random_generated_word, linefeed, size-1);
    random_generated_word[size-1] = '\0';
    free(linefeed);

    //print random word for testing right now
    printf("%s\n", random_generated_word);

    char *user_input = NULL;


    while(true) {
    //get the user input
    int count = 0;
    printf("Enter your word > ");
    size = 5;
    getline(&user_input, &size, stdin); //get the userinput
    
    //loop through it multiple times and make the letters colored

        for (size_t i = 0; i < 5; i++) { //words are only 5 letters
            if (user_input[i] == random_generated_word[i]) { //check if green or not
                printf(GREEN "%c" RESET, user_input[i]);
                count++;
            } else { //check if yellow or not. it exists but not in that spot
                count--;
                if (exists(user_input[i], random_generated_word)) {
                    printf(YELLOW "%c" RESET, user_input[i]); 
                } else {
                    printf(GRAY "%c" RESET, user_input[i]);
                }
            }
        }
        puts("");

        //if all the letters are right
        if (count == 5) {
            break;
        }
    }
    free(user_input);
    //free(random_generated_word);
}
