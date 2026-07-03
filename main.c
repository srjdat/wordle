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
// this is for yellow letters
bool exists(char user_input, char *random_word) {
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
    FILE *f = fopen("words.txt", "rb"); //open words.txt for reading only
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
    srand(time(NULL)); //to have a random number every time
    int random_number = rand()%line_count;

    //get the random generated word
    char *random_generated_word = calloc(6, sizeof(char));

    //loop through the file until we random_number nth word
    int i = 0;
    linefeed = NULL;
    size = 6;
    fseek(f, 0, SEEK_SET); // set the file to the top to get the random word
    while (i < random_number) {
        i++;
        getline(&linefeed, &size, f);
    }
    fclose(f); //close the file pointer

    //make random_generated_word the linefeed
    strncpy(random_generated_word, linefeed, size-1);
    random_generated_word[size-1] = '\0';
    free(linefeed);

    // user input
    char *user_input = NULL;

    while(true) {
    //get the user input
    int count = 0;
    printf("Enter your word > ");
    size = 5;
    getline(&user_input, &size, stdin); //get the userinput

    //loop through it multiple times and make the letters colored
        // for (size_t i = 0; i < 5; i++) { //words are only 5 letters
        //     if (user_input[i] == random_generated_word[i]) { //check if green or not
        //         printf(GREEN "%c" RESET, user_input[i]);
        //         count++;
        //     } else { //check if yellow or not. it exists but not in that spot
        //         count--;
        //         if (exists(user_input[i], random_generated_word)) {
        //             printf(YELLOW "%c" RESET, user_input[i]);
        //         } else {
        //             printf(GRAY "%c" RESET, user_input[i]);
        //         }
        //     }
        // }

        // color for each letter
        char word_color_array[5] = "";

        // count for each character
        int char_array_count[26] = {0};
        for (size_t i = 0; i < 5; i++) {
            char_array_count[random_generated_word[i] - 'a']++; // increment each letter in the word
        }

        // first loop check for greens
        for (size_t i = 0; i < 5; i++) {
            if (user_input[i] == random_generated_word[i]) {
                word_color_array[i] = 'G';
                char_array_count[user_input[i] - 'a']--; // decrement each letter that's green
                count++;
            }
        }

        // check for yellows and gray
        for (size_t i = 0; i < 5; i++) {
            // bool yellow = exists(user_input[i], random_generated_word); // if yellow then it exists
            bool yellow = strchr(random_generated_word, user_input[i]);
            if (yellow) {
                // if it exists then we check if we've marked it green or not
                if (char_array_count[user_input[i] - 'a'] > 0) {
                    // exists and there's sitll one left or isn't green
                    if (word_color_array[i] == '\0') {
                        word_color_array[i] = 'Y';
                    }
                } else {
                    // exists but we've already marked it so it's gray
                    if (word_color_array[i] == '\0') {
                        word_color_array[i] = 'R';
                    }
                }
            } else {
                // doesn't exist so gray
                if (word_color_array[i] == '\0') {
                    word_color_array[i] = 'R';
                }
            }
        }

        for (size_t i = 0; i < 5; i++) {
            if (word_color_array[i] == 'G') {
                // if the letter is green
                printf(GREEN "%c" RESET, user_input[i]);
            } else if (word_color_array[i] == 'Y') {
                // if the letter is yellow
                printf(YELLOW "%c" RESET, user_input[i]);
            } else {
                // letter is gray
                printf(GRAY "%c" RESET, user_input[i]);
            }
        }

        puts("");

        //if all the letters are right
        if (count == 5) {
            break;
        }
    }
    free(user_input);
    free(random_generated_word);
}
