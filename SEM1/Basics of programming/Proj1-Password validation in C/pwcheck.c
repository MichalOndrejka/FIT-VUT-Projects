//Michal Ondrejka xondre15
#include <stdio.h>
#include <stdlib.h>

#define true 1
#define false 0

int str_len(char str[]) {
    //Counts the length of string
    int i = 0;
    while (str[i] != '\0' && str[i] != '\n') {
        i++;
    }
    return i;
}

int substr_compare(char substring1[], char substring2[], int parameter) {
    //Checks the sequence of characters based on the parameter
    for (int i = 0; i < parameter; i++) {
        if (substring1[i] != substring2[i]) {
            return false;
        }
    }
    return true;
}

int char_in_array(char character, char array[]) {
    //Checks if there is a chracter in array
    for (int i = 0; array[i] != '\0'; i++) {
        if (array[i] == character) {
            return true;
        }
    }
    return false;
}

int security_1 (char psw[]) {
    int lower_case = false;
    int upper_case = false;
    //Checks for lower and upper case
    for (int i = 0; psw[i] != '\0'; i++) {
        if ('a' <= psw[i] && psw[i] <= 'z') {
            lower_case = true;
        }
        else if ('A' <= psw[i] && psw[i] <= 'Z') {
            upper_case = true;
        }
    }
    return lower_case == true && upper_case == true;
}

int security_2 (char psw[], int parameter) {
    int lower_case = false;
    int upper_case = false;
    int number = false;
    int special_character = false;
    if (parameter > 4) {
        parameter = 4;
    }
    //Checks and counts different groups of characters
    for (int i = 0; psw[i] != '\0'; i++) {
        if ('a' <= psw[i] && psw[i] <='z') {
            lower_case = true;
        }
        else if ('A' <= psw[i] && psw[i] <='Z') {
            upper_case = true;
        }
        else if ('0' <= psw[i] && psw[i] <='9') {
            number = true;
        }
        else if (' ' <= psw[i] && psw[i] <= '~') {
            special_character = true;
        }
    }
    //If there are more groups of characters then parameter, it continues to security 1
    if (lower_case + upper_case + number + special_character >= parameter) {
        return security_1(psw);
    }
    else {
        return false;
    }
}

int security_3 (char psw[], int parameter) {
    int sequence = 1;
    int max_sequence = 1;
    //Counts the sequence of letters, if letters aren't same it resets to 1
    for (int i = 1; psw[i] != '\0'; i++) {
        if (psw[i] == psw[i-1]) {
            sequence++;
            if (sequence >= max_sequence) {
                max_sequence = sequence;
            }
        }
        else {
            sequence = 1;
        }
    }
    //If the max sequence is smaller then the parameter, it continues to security 2
    if (max_sequence < parameter) {
        return security_2(psw, parameter);
    }
    else {
        return false;
    }
}

int security_4 (char password[], int parameter) {
    //If the parameter si larger then the password there is no need to check substrings
    if (parameter > str_len(password)) {
        return security_3(password, parameter);
    }
    //Comparing first characters of substrings, a funtion is being called if they match
    for (int i = 0; password[i + parameter - 1] != '\0'; i++) {
        for (int j = i + 1; password[j + parameter - 1] != '\0'; j++) {
            if (password[i] == password[j]) {
                if (substr_compare(&password[i], &password[j], parameter)) {
                    return false;
                }
            }
        }
    }
    return security_3(password, parameter);
}

int main (int argc, char *argv[]) {
    //Checks if the stats are required and written correctly
    int stats = false;
    char stats_example[] = "--stats";
    if (argc == 4) {
        //If compares the parameter with the stats_example
        if (substr_compare(argv[3], stats_example, str_len(stats_example)) && str_len(argv[3]) == str_len(stats_example)) {
            stats = true;
        } else {
            fprintf(stderr,"Incorrect stats parameter!\n");
            return 1;
        }
    //Checks if there is corrrecnt number of parameters
    } else if (argc < 3 || argc > 4) {
        fprintf(stderr,"Incorrect number of parameters!\n");
        return 1;
    }
    //Create integer from string and save the rest of the string
    char *parameter_rest;
    int parameter = strtol(argv[2], &parameter_rest, 10);
    char *security_rest;
    int security = strtol(argv[1], &security_rest, 10);
    //If there is rest of the string, it means it is incorrect parameter
    if (str_len(parameter_rest) > 0 || str_len(security_rest) > 0) {
        fprintf(stderr,"Incorrect security or stats parameter!\n");
        return 1;
    }
    int max_psw_length = 102;
    char password[max_psw_length];
    //Variables for stats
    int arr_diff_characters[127] = {false}; //Array of booleans for ascii table characters
    int count_diff_characters = 0;
    int min = max_psw_length;
    double character_count = 0;
    double password_count = 0;
    //While loop gets max_psw_length of characters to string, it discrads the rest
    while (fgets(password, max_psw_length, stdin) != NULL) {
        //If there is no \n in the string, it means it cut the string and therefore the password was longer then allowed
        if (!(char_in_array('\n', password))) {
            fprintf(stderr,"Incorrect password length!");
            return 1;
        }
        //Parameter has to be intiger > 0
        if (parameter < 1) { 
            fprintf(stderr,"Incorrect security parameter!");
            return 1;
        }
        //Updates stats variables
        password_count++;
        for (int i = 0; password[i] != '\0' && password[i] != '\n'; i++) {
            character_count++;
            int index = password[i]; //Creates index (ASCII ord) from character automatically
            if (arr_diff_characters[index] == false) { //If there is false in array of booleans, it hasn't been counted yet.
                arr_diff_characters[index] = true;
                count_diff_characters++;
            }
        }
        //Updates min length
        if (str_len(password) < min) {
            min = str_len(password);
        }
        /*Calls the security function. If the password fulfill the requirements, 
        funcitons return one security level lower function, until security 1, which returns true*/
        switch (security) {
        case 1:
            if (security_1(password)) {
                printf("%s", password);
            }
            break;
        case 2:
            if (security_2(password, parameter)) {
                printf("%s", password);
            }
            break;
        case 3:
            if (security_3(password, parameter)) {
                printf("%s", password);
            }
            break;
        case 4:
            if (security_4(password, parameter)) {
                printf("%s", password);
            }
            break;
        default:
            fprintf(stderr,"Incorrect security level!\n");
            return 1;
        }
    }
    double average = character_count/password_count;
    //Prints stats
    if (stats == true) {
        printf("Statistika:\nRuznych znaku: %d\nMinimalni delka: %d\nPrumerna delka: %.1f\n", count_diff_characters, min, average);
    }
    return 0;
}