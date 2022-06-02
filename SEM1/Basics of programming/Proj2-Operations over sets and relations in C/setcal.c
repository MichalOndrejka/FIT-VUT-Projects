// Dominik Truchly          xtruch01
// Michal Ondrejka          xondre15
// Jakub Brnak              xbrnak01
// Lucia Sirova             xsirov01
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
// Number of invalid words in universum
#define NUM_INVALID_WORDS 25

// Declaring structures for universum 
typedef struct {
    int count;
    char **items;

} uni_t;

// Declaring structures for sets and relations 
typedef struct {
    int count;
    int *items;

} set_t;

// List of invalid words
char invalid_words[NUM_INVALID_WORDS][20] = {
    "true",       "false",     "U",
    "R",          "S",         "C",
    "empty",      "card",      "complement",
    "union",      "intersect", "minus",
    "subseteq",   "subset",    "equals",
    "reflexive",  "symmetric", "antisymmetric",
    "transitive", "function",  "domain",
    "codomain",   "injective", "surjective",
    "bijective"};

// Reading line from given file
char *readLine(FILE *file)
{
    char *string = (char *)malloc(sizeof(char));
    if (string == NULL) {
        return NULL;
    }
    char character = fgetc(file);

    int char_count = 0;
    while (character != '\n' && character != EOF) {
        char_count += 1;
        if (character == ' ') {
            char_count = 0;
        }
        // Returning error if word is longer than 30 characters
        if (char_count > 30) {
            fprintf(stderr, "Too many characters in a word");
            exit(1);
        }
        string = realloc(string, strlen(string) + sizeof(char) + 1);
        if (string == NULL) {
            free(string);
            string = NULL;
            return NULL;
        }
        strncat(string, &character, 1);
        character = fgetc(file);
        if (character == EOF) {
            free(string);
            string = NULL;
            return NULL;
        }

    }
    if (character == EOF) {
        free(string);
        string = NULL;
        return NULL;
    }
    return string;
}
// Convert words from universum to indexes and save them to set structures
int choosefrom(uni_t *universum, char *word)
{
    for (int i = 0; i < universum->count; i++) {
        if (!strcmp(universum->items[i], word)) {
            return i;
        }
    }
    fprintf(stderr, "Item not in universum");
    exit(1);
}
// Loading line from file into universum structure
void uni_ctor(char line[], uni_t *universum)
{
    universum->items = malloc(0);
    universum->count = 0;
    char *word = strtok(line, " ");
    if (strlen(word) != 1) {
        fprintf(stderr, "Wrong input file");
        exit(1);
    }
    word = strtok(NULL, " ");

    while (word != NULL) {
        universum->count += 1;

        universum->items =
            realloc(universum->items, sizeof(char *) * universum->count);
        universum->items[universum->count - 1] =
            malloc(strlen(word) * sizeof(char));

        if (universum->items == NULL ||
            universum->items[universum->count - 1] == NULL) {
            fprintf(stderr, "Memory error");
            exit(1);
        }
        strcpy(universum->items[universum->count - 1], word);
        word = strtok(NULL, " ");
    }
}
// Loading line from file into set structure
void set_ctor(char line[], set_t *set, uni_t *universum)
{
    set->items = (int *)malloc(0);
    set->count = 0;
    char *word = strtok(line, " ");
    if (strlen(word) != 1) {
        fprintf(stderr, "Wrong input file");
        exit(1);
    }
    word = strtok(NULL, " ");

    while (word != NULL) {
        set->count += 1;
        set->items = realloc(set->items, sizeof(int) * set->count);
        if (set->items == NULL) {
            fprintf(stderr, "Memory error");
            exit(1);
        }
        set->items[set->count - 1] = choosefrom(universum, word);
        word = strtok(NULL, " ");
    }
}
// Checks for duplicate element in universum
int duplicate_elements_in_universum(uni_t *universum)
{
    for (int i = 0; i < universum->count; i++) {
        for (int j = 0; j < universum->count; j++) {
            if (strcmp(universum->items[i], universum->items[j]) == 0 &&
                i != j) {
                return 1;
            }
        }
    }
    return 0;
}
// Checks for duplicate elements in relations
int duplicate_elements_in_relation(set_t *arr)
{
    for (int i = 1; i < arr->count; i += 2) {
        int x = arr->items[i - 1];
        int y = arr->items[i];
        for (int j = 1; j < arr->count; j += 2) {
            if (((arr->items[j] == y) && (arr->items[j - 1] == x)) &&
                (i != j)) {
                return 1;
            }
        }
    }
    return 0;
}
// Checks for duplicate elements in sets
int duplicate_elements_in_set(set_t *arr)
{
    for (int i = 0; i < arr->count; i++) {
        for (int j = 0; j < arr->count; j++) {
            if ((arr->items[i] == arr->items[j]) && (i != j)) {
                return 1;
            }
        }
    }
    return 0;
}
// Checks for invalid words in universum
int invalid_words_in_universum(uni_t *universum)
{
    for (int i = 0; i < universum->count; i++) {
        for (int j = 0; j < NUM_INVALID_WORDS; j++) {
            if (!(strcmp(universum->items[i], invalid_words[j])))
                return 1;
        }
    }
    return 0;
}
// Checks for non alphabetic characters in line
int non_alphabetic_character(char line[])
{
    for (int i = 0; line[i] != '\0'; i++) {
        if (!((line[i] >= 'A' && line[i] <= 'Z') ||
              (line[i] >= 'a' && line[i] <= 'z') || line[i] == ' ')) {
            return 1;
        }
    }
    return 0;
}
// Replace parentheses for blank spaces
void removeprts(char *line)
{
    int count = 0;
    int length = strlen(line);
    for (int i = 0; i < length; i++) {
        if (line[i] == '(' || line[i] == ')') {
            count++;
            line[i] = ' ';
        }
    }
}

// Commands over sets
void empty(set_t set)
{
    if (set.count > 0)
        printf("false\n");
    else
        printf("true\n");
}

void card(set_t set) { printf("%d\n", set.count); }

void complement(set_t set, uni_t universum)
{
    printf("S ");
    if (set.count == 0) {
        for (int i = 0; i < universum.count; i++) {
            printf("%s ", universum.items[i]);
        }
    }
    else {
        bool flag = true;
        for (int i = 0; i < universum.count; i++) {
            for (int j = 0; j < set.count; j++) {
                if (set.items[j] == i) {
                    flag = false;
                    break;
                }
            }
            if (flag == true) {
                printf("%s ", universum.items[i]);
            }
            flag = true;
        }
    }
    printf("\n");
}

void union_sets(set_t set1, set_t set2, uni_t universum)
{
    printf("S ");
    for (int i = 0; i < set1.count; i++) {
        printf("%s ", universum.items[set1.items[i]]);
    }
    bool match = true;
    for (int i = 0; i < set2.count; i++) {
        match = false;
        for (int j = 0; j < set1.count; j++) {
            if (set2.items[i] == set1.items[j]) {
                match = true;
            }
        }
        if (match == false) {
            printf("%s ", universum.items[set2.items[i]]);
        }
    }
}

void intersect(set_t *sets, int set1, int set2, uni_t universum)
{
    printf("S ");
    int *answr = (int *)malloc(0);
    int count = 0;
    for (int i = 0; i < sets[set1].count; i++) {
        for (int j = 0; j < sets[set2].count; j++) {
            if (sets[set1].items[i] == sets[set2].items[j]) {
                count++;
                answr = realloc(answr, count * (sizeof(int)));
                answr[count - 1] = sets[set1].items[i];
            }
        }
    }
    for (int u = 0; u < count; u++) {
        if (u == count - 1)
            printf("%s\n", universum.items[answr[u]]);
        else
            printf("%s ", universum.items[answr[u]]);
    }
    free(answr);
    answr = NULL;
}

void minus(set_t set1, set_t set2, uni_t universum)
{
    printf("S ");
    int match;
    for (int i = 0; i < set1.count; i++) {
        match = false;
        for (int j = 0; j < set2.count; j++) {
            if (set1.items[i] == set2.items[j]) {
                match = true;
            }
        }
        if (match == false) {
            printf("%s ", universum.items[set1.items[i]]);
        }
    }
    printf("\n");
}

void subsetq(set_t setA, set_t setB)
{
    int countInB = 0;
    for (int i = 0; i < setA.count; i++) {
        for (int j = 0; j < setB.count; j++) {
            if (setA.items[i] == setB.items[j]) {
                countInB += 1;
                break;
            }
        }
    }
    if (countInB == setA.count)
        printf("true\n");
    else
        printf("false\n");
}

void subset(set_t set1, set_t set2)
{
    if (set1.count > set2.count) {
        printf("false");
        return;
    }
    int *arr = malloc(sizeof(int) * set1.count);
    for (int i = 0; i < set1.count; i++) {
        arr[i] = 0;
    }
    for (int i = 0; i < set1.count; i++) {
        for (int k = 0; k < set2.count + 1; k++) {
            if (set1.items[i] == set2.items[k]) {
                arr[i] = 1;
            }
            if (k == set2.count && arr[i] == 0) {
                printf("false");
                return;
            }
        }
    }
    int count = 0;
    for (int i = 0; i < set1.count; i++) {
        if (arr[i] == 1) {
            count += 1;
        }
    }
    if (count < set2.count) {
        printf("true");
        return;
    }
    printf("false");
    free(arr);
    arr = NULL;
}

void equals(set_t *sets, int set1, int set2)
{
    int count = 0;
    if (sets[set1].count != sets[set2].count) {
        printf("false\n");
        return;
    }
    for (int i = 0; i < sets[set1].count; i++) {
        for (int j = 0; j < sets[set2].count; j++) {
            if (sets[set1].items[i] == sets[set2].items[j]) {
                count++;
            }
        }
    }

    if (count == sets[set1].count) {
        printf("true\n");
        return;
    }
    else {
        printf("false\n");
        return;
    }
}

// Commands over relations

void reflexive(set_t set, uni_t universum)
{
    int *arr = malloc(universum.count * sizeof(int));
    for (int i = 0; i < set.count; i += 2) {
        if (set.items[i] == set.items[i + 1]) {
            arr[set.items[i]] = true;
        }
    }
    for (int i = 0; i < universum.count; i++) {
        if (!arr[i]) {
            printf("false\n");
            return;
        }
    }
    free(arr);
    arr = NULL;
    printf("true\n");
}

void symmetric(set_t relationA)
{
    int x, y, count = 0;
    for (int i = 1; i < relationA.count; i += 2) {
        x = relationA.items[i - 1];
        y = relationA.items[i];
        if (x == y)
            count++;
        else {
            for (int j = 1; j < relationA.count; j += 2) {
                if (x == relationA.items[j] && y == relationA.items[j - 1]) {
                    count++;
                }
            }
        }
    }
    if (count == relationA.count / 2)
        printf("true\n");
    else
        printf("false\n");
}

void antisymetric(set_t set)
{
    for (int i = 0; i < set.count; i += 2) {
        int a = set.items[i];
        int b = set.items[i + 1];
        if (a == b)
            continue;
        for (int j = 0; j < set.count; j += 2) {
            int c = set.items[j];
            int d = set.items[j + 1];
            if (a == d && b == c) {
                printf("false\n");
                return;
            }
        }
    }
    printf("true\n");
}

void functionabc(set_t set, uni_t universum)
{
    int *arr = malloc(universum.count * sizeof(int));
    for (int i = 0; i < universum.count; i++) {
        arr[i] = false;
    }
    for (int i = 0; i < set.count; i += 2) {
        if (arr[set.items[i]] == true) {
            printf("false\n");
            return;
        }
        arr[set.items[i]] = true;
    }
    printf("true\n");
    free(arr);
    arr = NULL;
}

void domain(set_t relationA, uni_t universum)
{
    bool flag = true;
    printf("S ");
    for (int i = 0; i < relationA.count; i += 2) {
        for (int j = 0; j < i; j += 2) {
            if (relationA.items[j] == relationA.items[i])
                flag = false;
        }
        if (flag)
            printf("%s ", universum.items[relationA.items[i]]);
        flag = true;
    }
    printf("\n");
}

void codomain(set_t relationA, uni_t universum)
{
    bool flag = true;
    printf("S ");
    for (int i = 1; i < relationA.count; i += 2) {
        for (int j = 1; j < i; j += 2) {
            if (relationA.items[j] == relationA.items[i])
                flag = false;
        }
        if (flag)
            printf("%s ", universum.items[relationA.items[i]]);
        flag = true;
    }
    printf("\n");
}

int findIndex(int number, set_t set)
{
    for (int i = 0; i < set.count; i++) {
        if (set.items[i] == number) {
            return i;
        }
    }
    return -1;
}

void transitive(set_t set)
{
    bool flag = false;
    for (int i = 0; i < set.count; i += 2) {
        for (int j = 0; j < set.count; j += 2) {
            if ((set.items[i + 1] == set.items[j]) && (i != j)) {
                flag = false;
                for (int u = 0; u < set.count; u += 2) {
                    if (set.items[u] == set.items[i] &&
                        set.items[u + 1] == set.items[j + 1]) {
                        flag = true;
                    }
                }
                if (flag == false) {
                    printf("false\n");
                    return;
                }
            }
        }
    }
    printf("true\n");
    return;
}

bool surjective(set_t relation, set_t set1, set_t set2)
{
    if (relation.count == 0) {
        return true;
    }
    int *arr = malloc(set2.count * sizeof(int));
    int index;
    for (int i = 0; i < set2.count; i++) {
        arr[i] = false;
    }
    for (int i = 0; i < relation.count; i += 2) {
        index = findIndex(relation.items[i], set1);
        if (index == -1) {
            return false;
        }
    }
    for (int i = 0; i < relation.count; i += 2) {
        index = findIndex(relation.items[i + 1], set2);
        if (index == -1) {
            return false;
        }
        if (index == -1) {
            fprintf(stderr, "Invalid relation\n");
            free(arr);
            arr = NULL;
            exit(1);
        }
        if (arr[index] == false) {
            arr[index] = true;
        }
    }

    for (int i = 0; i < set2.count; i++) {
        if (arr[i] == false) {
            free(arr);
            arr = NULL;
            return false;
        }
    }
    free(arr);
    arr = NULL;
    return true;
}

bool injective(set_t relation, set_t set1, set_t set2)
{
    if (relation.count == 0) {
        return true;
    }
    int *arr = malloc(set2.count * sizeof(int));
    int index;
    for (int i = 0; i < set2.count; i++) {
        arr[i] = false;
    }
    for (int i = 0; i < relation.count; i += 2) {
        index = findIndex(relation.items[i], set1);
        if (index == -1) {
            return false;
        }
    }
    for (int i = 0; i < relation.count; i += 2) {
        index = findIndex(relation.items[i + 1], set2);
        if (index == -1) {
            fprintf(stderr, "Invalid relation\n");
            free(arr);
            arr = NULL;
            exit(1);
        }
        if (arr[index] == true) {
            free(arr);
            arr = NULL;
            return false;
        }
        else if (arr[index] == false) {
            arr[index] = true;
        }
    }
    free(arr);
    arr = NULL;
    return true;
}

void bijective(set_t relation, set_t set1, set_t set2)
{
    if (injective(relation, set1, set2) && surjective(relation, set1, set2)) {
        printf("true\n");
    }
    else {
        printf("false\n");
    }
}

// Detects given command and arguments from a line and executes them
void command_detecter(char line[], uni_t universum, set_t *sets, int arr[])
{
    uni_t command;
    uni_ctor(line, &command);
    for (int i = 1; i < command.count; i++) {
        if (atoi(command.items[i]) < 2) {
            fprintf(stderr, "Command over universum\n");
            exit(1);
        }
    }
    if (command.count == 2) {
        int number = atoi(command.items[1]) - 2;
        if (arr[number] == 0) {
            if (!strcmp(command.items[0], "empty")) {
                empty(sets[number]);
            }
            else if (!strcmp(command.items[0], "card")) {
                card(sets[number]);
            }
            else if (!strcmp(command.items[0], "complement")) {
                complement(sets[number], universum);
            }
            else {
                fprintf(stderr, "Set command over relation\n");
                exit(1);
            }
        }
        if (arr[number] == 1) {
            if (!strcmp(command.items[0], "reflexive")) {
                reflexive(sets[number], universum);
            }
            else if (!strcmp(command.items[0], "symmetric")) {
                symmetric(sets[number]);
            }
            else if (!strcmp(command.items[0], "antisymmetric")) {
                antisymetric(sets[number]);
            }
            else if (!strcmp(command.items[0], "transitive")) {
                transitive(sets[number]);
            }
            else if (!strcmp(command.items[0], "function")) {
                functionabc(sets[number], universum);
            }
            else if (!strcmp(command.items[0], "domain")) {
                domain(sets[number], universum);
            }
            else if (!strcmp(command.items[0], "codomain")) {
                codomain(sets[number], universum);
            }
            else {
                fprintf(stderr, "Relation command over over\n");
                exit(1);
            }
        }
    }
    else if (command.count == 3) {
        int number1 = atoi(command.items[1]) - 2;
        int number2 = atoi(command.items[2]) - 2;
        if (arr[number1] == 0 && arr[number2] == 0) {
            if (!strcmp(command.items[0], "union")) {
                union_sets(sets[number1], sets[number2], universum);
            }
            else if (!strcmp(command.items[0], "intersect")) {
                intersect(sets, number1, number2, universum);
            }
            else if (!strcmp(command.items[0], "minus")) {
                minus(sets[number1], sets[number2], universum);
            }
            else if (!strcmp(command.items[0], "subseteq")) {
                subsetq(sets[number1], sets[number2]);
            }
            else if (!strcmp(command.items[0], "subset")) {
                subset(sets[number1], sets[number2]);
            }
            else if (!strcmp(command.items[0], "equals")) {
                equals(sets, number1, number2);
            }
            else {
                fprintf(stderr, "Incorrect command\n");
                exit(1);
            }
        }
        else {
            fprintf(stderr, "Set command over relation\n");
            exit(1);
        }
    }
    else if (command.count == 4) {

        int number1 = atoi(command.items[1]) - 2;
        int number2 = atoi(command.items[2]) - 2;
        int number3 = atoi(command.items[3]) - 2;
        if (arr[number1] == 1 && arr[number3] == 0) {
            if (!strcmp(command.items[0], "injective")) {
                if (injective(sets[number1], sets[number2], sets[number3]) ==
                    true) {
                    printf("true\n");
                }
                else {
                    printf("false\n");
                }
            }
            else if (!strcmp(command.items[0], "surjective")) {
                if (surjective(sets[number1], sets[number2], sets[number3]) ==
                    true) {
                    printf("true\n");
                }
                else {
                    printf("false\n");
                }
            }
            else if (!strcmp(command.items[0], "bijective")) {
                bijective(sets[number1], sets[number2], sets[number3]);
            }
            else {
                fprintf(stderr, "Invalid command\n");
                exit(1);
            }
        }
        else {
            fprintf(stderr, "Relation command over set\n");
            exit(1);
        }
    }
    else {
        fprintf(stderr, "Wrong number of parameters\n");
        exit(1);
    }
    for (int i = 0; i < command.count; i++) {
        free(command.items[i]);
        command.items[i] = NULL;
    }
    free(command.items);
    command.items = NULL;
}

int main(int argc, char **argv)
{
    // Checking for invalid number of arguments
    if (argc > 2) {
        fprintf(stderr, "Too many arguments passed.\n");
        return 1;
    }
    else if (argc < 2) {
        fprintf(stderr, "Not enough arguments passed.\n");
        return 1;
    }

    FILE *file;
    file = fopen(argv[1], "r");
    if (file == NULL) {
        fprintf(stderr, "Not existing file.\n");
        return 1;
    }
    char *line = readLine(file);
    uni_t universum;
    int sets_count = 0;
    set_t *sets = malloc(0);

    int *arr = (int *)malloc(sizeof(int) * sets_count);

    int line_count = 0;
    bool commands = false;
    // Loading lines from file and executes commands (U, S, R, C)
    while (line != NULL) {
        line_count++;
        if (line_count > 1000) {
            fprintf(stderr, "Too long file\n");
            return 1;
        }
        if (line_count == 1 && (line[0] != 'U')) {
            fprintf(stderr, "No universum\n");
            return 1;
        }
        switch (line[0]) {
        case 'U':
            if (non_alphabetic_character(line)) {
                fprintf(stderr, "Non alphabetic characters\n");
                return 1;
            }
            printf("%s\n", line);
            uni_ctor(line, &universum);
            if (invalid_words_in_universum(&universum)) {
                fprintf(stderr, "Invalid words in universum.\n");
                return 1;
            }
            if (duplicate_elements_in_universum(&universum)) {
                fprintf(stderr, "Duplicate elements in universum.\n");
                return 1;
            }
            break;
        case 'S':
            printf("%s\n", line);
            sets_count++;
            arr = (int *)realloc(arr, sizeof(int) * sets_count);
            arr[sets_count - 1] = 0;
            sets = realloc(sets, sizeof(set_t) * (sets_count));
            set_ctor(line, &sets[sets_count - 1], &universum);
            if (duplicate_elements_in_set(&sets[sets_count - 1])) {
                fprintf(stderr, "Duplicate elements in set.\n");
                return 1;
            }

            break;
        case 'R':
            printf("%s\n", line);
            sets_count++;
            arr = (int *)realloc(arr, sizeof(int) * sets_count);
            arr[sets_count - 1] = 1;
            sets = realloc(sets, sizeof(set_t) * (sets_count));
            removeprts(line);
            set_ctor(line, &sets[sets_count - 1], &universum);
            if (duplicate_elements_in_relation(&sets[sets_count - 1])) {
                fprintf(stderr, "Duplicate elements in relation.\n");
                return 1;
            }
            break;
        case 'C':
            command_detecter(line, universum, sets, arr);
            commands = true;
            break;
        default:
            fprintf(stderr, "Invalid line.\n");
            return 1;
        }

        free(line);
        line = NULL;

        line = readLine(file);
    }

    // Free allocated memory
    free(line);
    line = NULL;

    for (int i = 0; i < sets_count; i++) {
        free(sets[i].items);
        sets[i].items = NULL;
    }
    free(sets);
    sets = NULL;
    if (commands == false) {
        fprintf(stderr, "No commands\n");
        return 1;
    }
    for (int i = 0; i < universum.count; i++) {
        free(universum.items[i]);
        universum.items[i] = NULL;
    }
    free(universum.items);
    universum.items = NULL;
    free(arr);
    arr = NULL;
    return 0;
}