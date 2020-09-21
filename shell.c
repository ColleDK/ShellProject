#include "shell.h"

/**
 * Global variables and functions
 */
int TRUE = 1;
int isPipe = 0;
char* slash;
void grepCommand(); //i need to declare this because i use it in a function before the real declaration

/**
 *
 * @param fullPath is the char[] which has the path
 */
void previousCD(char* fullPath){
    /**
     * This will first clear the "/" in front and then clear until the next "/"
     */
    int place = strlen(fullPath)-2;
    fullPath[place+1] = '\0';
    while(fullPath[place] != '/'){
        fullPath[place--] = '\0';
    }
}


/**
 * Function to check whether a directory exists or not.
 * It returns 1 if given path is directory exists
 * otherwise returns 0.
 * https://codeforwin.org/2018/03/c-program-check-file-or-directory-exists-not.html
 */
int isDirectoryExists(const char *path)
{
    struct stat stats;
    stat(path, &stats);
    if (S_ISDIR(stats.st_mode))
        return 1;
    return 0;
}


/**
 * @param arr is a pointer to an array that needs to be erased of content
 * @param length is the length of content in the array (not length of array)
 */
void arrayCleaner(char* arr, int length){
    for (int i = 0; i < length; ++i) {
        arr[i] = '\0';
    }
}

/**
 * @param arr is a pointer to an array that needs to be split
 * @param inputNumber is the numbered input you want
 * @return returns a pointer to a new array
 */

char* inputSplitter(char* arr, int inputNumber){
    int spaceCounter=0, placeCounter=0, startpoint=0, endpoint=0;
    // i need a i-th number input therefore i move i-1 number of spaces up the array
    while (inputNumber-1 != spaceCounter){
        while (arr[placeCounter] != ' ' && arr[placeCounter] != '\0' && arr[placeCounter] != '\n'){
            placeCounter++;
        }
        spaceCounter++;
        // if multiple spaces in a row -> skip it
        while (arr[++placeCounter] == ' '){
        }
    }

    // this is input we want
    startpoint = placeCounter;
    while (arr[placeCounter] != ' ' && arr[placeCounter] != '\0' && arr[placeCounter] != '\n'){
        placeCounter++;
    }
    endpoint = placeCounter;

    // make space on heap for the input and return the pointer
    char *array = malloc((endpoint-startpoint) * sizeof(char));
    arrayCleaner(array, sizeof(array)/sizeof(char));
    int loopCounter=0;
    for (int i = startpoint; i < endpoint; ++i) {
        array[loopCounter++] = arr[i];
    }
    return array;
}

/**
 * Change directory
 */
void cdCommand(char* fullPath, char* h , char* secondInput) {
    // If second input is .. we need to go back 1 directory except if the directory is "/"
    if (strcmp("..", secondInput) == 0) {
        if (strlen(fullPath) == 1) {
            printf("Path is at lowest point\n");
        }
        else previousCD(fullPath);
    }

    // if second input is ~ go to home directory
    else if (strcmp("~", secondInput) == 0){
        char* home = getenv("HOME");
        free(fullPath);
        fullPath = malloc(strlen(home) * sizeof(char));
        arrayCleaner(fullPath, sizeof(fullPath) / sizeof(char));
        strcpy(fullPath, home);
        free(home);
    }

    // Makes a temporary path and checks if it exists, if yes copy the temp path to fullpath
    else if (secondInput[0] == '/') {
        char *tempPath = malloc((strlen(secondInput) + strlen(slash)) * sizeof(char));
        arrayCleaner(tempPath, sizeof(tempPath) / sizeof(char));
        strcat(tempPath, secondInput);
        strcat(tempPath, slash);
        if (isDirectoryExists(tempPath) == 0) {
            printf("%s\n", strerror(errno));
        } else {
            free(fullPath);
            fullPath = malloc(strlen(tempPath) * sizeof(char));
            arrayCleaner(fullPath, sizeof(fullPath) / sizeof(char));
            strcpy(fullPath, tempPath);
            free(tempPath);
        }

        // same as previous
    } else if (strlen(secondInput) != 0) {
        char *tempPath = malloc((strlen(fullPath) + strlen(secondInput) + strlen(slash)) * sizeof(char));
        strcpy(tempPath, fullPath);
        strcat(tempPath, secondInput);
        strcat(tempPath, slash);
        if (isDirectoryExists(tempPath) == 0) {
            printf("%s\n", strerror(errno));
        } else {
            arrayCleaner(fullPath, sizeof(fullPath) / sizeof(char));
            strcpy(fullPath, tempPath);
            free(tempPath);
        }
    } else {
        arrayCleaner(fullPath, sizeof(fullPath) / sizeof(char));
        strcpy(fullPath, h);
        strcat(fullPath, slash);
    }
}

/**
 * https://stackoverflow.com/questions/845556/how-to-ignore-hidden-files-with-opendir-and-readdir-in-c-library
 * Searches the directory for all files except for ones that start with . or .. (hidden files)
 */

//TODO: make ls -a /directory | grep word
void lsCommand(char* fullPath, char* secondInput, char* thirdInput, char* fourthInput, char* fifthInput, char* sixthInput){
    // check if there is pipe
    if (isPipe == 0) {
        // if second input is ~ then list home directory
        if (strcmp(secondInput, "~") == 0) {
            char *home = getenv("HOME");
            DIR *pd = opendir(home);
            struct dirent *cur;
            while (cur = readdir(pd)) {
                if (cur->d_name[0] != '.') {
                    printf("%s\n", cur->d_name);
                }
            }
            closedir(pd);
            free(cur);
        }
        else if (secondInput[0] == '/') {
            DIR *pd = opendir(secondInput);
            struct dirent *cur;
            while (cur = readdir(pd)) {
                if (cur->d_name[0] != '.') {
                    printf("%s\n", cur->d_name);
                }
            }
            closedir(pd);
            free(cur);
        }
        else if (thirdInput[0] == '/'){
            DIR *pd = opendir(thirdInput);
            struct dirent *cur;
            while (cur = readdir(pd)) {
                if (strcmp("-a", secondInput) == 0) {
                    printf("%s\n", cur->d_name);
                } else {
                    if (cur->d_name[0] != '.') {
                        printf("%s\n", cur->d_name);
                    }
                }
            }
            closedir(pd);
            free(cur);
        }

        else {
            // else we list either all files (-a) or visible files
            DIR *pd = opendir(fullPath);
            struct dirent *cur;
            while (cur = readdir(pd)) {
                if (strcmp("-a", secondInput) == 0) {
                    printf("%s\n", cur->d_name);
                } else {
                    if (cur->d_name[0] != '.') {
                        printf("%s\n", cur->d_name);
                    }
                }
            }
            closedir(pd);
            free(cur);
        }
    }
    else{
        // if pipe is enabled (only grep is allowed in program) then get name and send to grepcommand
        if (strcmp(secondInput, "|") == 0){
            DIR *pd = opendir(fullPath);
            struct dirent *cur;
            while (cur = readdir(pd)) {
                if (strcmp("-a", secondInput) == 0) {
                    grepCommand(fullPath,fullPath,secondInput,thirdInput,1,cur->d_name,fourthInput);
                } else {
                    if (cur->d_name[0] != '.') {
                        grepCommand(fullPath,fullPath,secondInput,thirdInput,1,cur->d_name,fourthInput);
                    }
                }
            }
            closedir(pd);
            free(cur);
        }
        else if (strcmp(thirdInput, "|") == 0){
            DIR *pd = opendir(fullPath);
            struct dirent *cur;
            while (cur = readdir(pd)) {
                if (strcmp("-a", secondInput) == 0) {
                    grepCommand(fullPath,fullPath,secondInput,thirdInput,1,cur->d_name,fifthInput);
                } else {
                    if (cur->d_name[0] != '.') {
                        grepCommand(fullPath,fullPath,secondInput,thirdInput,1,cur->d_name,fifthInput);
                    }
                }
            }
            closedir(pd);
            free(cur);


            // check for ls -a dirName | grep grepWord
        } else if (strcmp(fourthInput, "|") == 0){
            if (strcmp("-a", secondInput) == 0 && strcmp(fifthInput,"grep") == 0) {
                if (strcmp(thirdInput, "~") == 0) {
                    char *home = getenv("HOME");
                    DIR *pd = opendir(home);
                    struct dirent *cur;
                    while (cur = readdir(pd)) {
                        grepCommand(fullPath, fullPath, secondInput, thirdInput, 1, cur->d_name, sixthInput);
                    }
                    free(cur);
                    closedir(pd);
                }

                // typed own directory path
                else if (thirdInput[0] == '/') {
                    DIR *pd = opendir(thirdInput);
                    struct dirent *cur;
                    while (cur = readdir(pd)) {
                        grepCommand(fullPath, fullPath, secondInput, thirdInput, 1, cur->d_name, sixthInput);
                    }
                    free(cur);
                    closedir(pd);
                }

                //directory in this directory
                else {
                    char *tempPath = malloc((strlen(fullPath) + strlen(slash) + strlen(thirdInput)) * sizeof(char));
                    strcpy(tempPath, fullPath);
                    strcat(tempPath, slash);
                    strcat(tempPath, thirdInput);
                    if (isDirectoryExists(tempPath) == 1) {
                        DIR *pd = opendir(tempPath);
                        struct dirent *cur;
                        while (cur = readdir(pd)) {
                            grepCommand(fullPath, fullPath, secondInput, thirdInput, 1, cur->d_name, sixthInput);
                        }
                        free(cur);
                        closedir(pd);
                    } else printf("Directory not found\n");
                }
            }
        }
    }
}

/**
 * makes directory
 */
void mkdirCommand(char* fullPath, char* secondInput){
    // make temporary path and check if exist
    char *tempPath = malloc((strlen(fullPath) + strlen(secondInput) + strlen(slash)) * sizeof(char));
    arrayCleaner(tempPath,sizeof(tempPath)/sizeof(char));
    strcpy(tempPath,fullPath);
    strcat(tempPath, secondInput);
    strcat(tempPath, slash);
    //if it doesnt exist make directory
    if (isDirectoryExists(tempPath) == 0) {
        mkdir(tempPath,777);
        free(tempPath);
    } else {
        printf("Directory already exists\n");
        free(tempPath);
    }
}

/**
 * removes directory
 */
void rmdirCommand(char* fullPath, char* secondInput){
    // make temporary path and check if exist
    char *tempPath = malloc((strlen(fullPath) + strlen(secondInput) + strlen(slash)) * sizeof(char));
    arrayCleaner(tempPath, sizeof(tempPath) / sizeof(char));
    strcpy(tempPath,fullPath);
    strcat(tempPath, secondInput);
    strcat(tempPath, slash);
    //if it doesnt exist dont delete directory
    if (isDirectoryExists(tempPath) == 0) {
        free(tempPath);
        printf("Directory doesn't exist\n");
    } else {
        rmdir(tempPath);
        free(tempPath);
    }
}

/**
 * https://stackoverflow.com/questions/13450809/how-to-search-a-string-in-a-char-array-in-c
 * use strstr() to search for word in and output if found
 */
void grepCommand(char* fullPath, char* secondInput, char* thirdInput, int lsEnabled, char* lsName, char* grepName){
    //check if it came from lscommand
    if (lsEnabled == 0) {
        // make temporary path to the file
        char *tempPath = malloc((strlen(fullPath) + strlen(thirdInput) + strlen(slash)) * sizeof(char));
        strcpy(tempPath, fullPath);
        strcat(tempPath, thirdInput);
        // give pointer read permissions
        FILE *f = fopen(tempPath, "r");
        // check if file exists
        if (f != NULL) {
            char fileArr[256] = "";
            arrayCleaner(fileArr, sizeof(fileArr));
            char *string;
            // get 1 line at a time
            while (fgets(fileArr, 256, f) != NULL) {
                // built in function to check if letter or word is in another char[]
                string = strstr(fileArr, secondInput);
                // if grepped word is found in line then print out line
                if (string != NULL) {
                    printf("%s", fileArr);
                }
            }
        } else {
            printf("No file found\n");
        }
        close(f);
        free(tempPath);
    }
    else{
        // maybe pointless but check if pointer is non empty
        if (lsName != NULL){
            char* string;
            // check if grep word is in name of ls file/directory
            string = strstr(lsName,grepName);
            if (string != NULL){
                printf("%s\n", lsName);
            }
        }
        else {}
    }
}


/**
 * cat will output the content of a file given that the file exists
 * max number of characters on a line is set to 256
 * Pipeline is placed here also for grepping word in cat
 */
void catCommand(char* fullPath, char* secondInput, char* fourthInput, char* fifthInput){
    if (isPipe == 0) {
        // make temporary path to file
        char *tempPath = malloc((strlen(fullPath) + strlen(secondInput) + strlen(slash)) * sizeof(char));
        strcpy(tempPath, fullPath);
        strcat(tempPath, secondInput);
        // give pointer read permissions
        FILE *f = fopen(tempPath, "r");
        // check if file exists
        if (f != NULL) {
            char fileArr[256] = "";
            arrayCleaner(fileArr, sizeof(fileArr));
            while (fgets(fileArr, 256, f) != NULL) {
                // print out content of file 1 line at a time
                printf("%s", fileArr);
            }
        } else {
            printf("No file found\n");
        }
        free(tempPath);
        close(f);
    }
    else {
        // if grep is in cat then send to grep command (since this is not from ls last 3 parameters doesnt matter)
        if (strcmp("grep", fourthInput) == 0) {
            grepCommand(fullPath, fifthInput, secondInput,0,slash,slash);
        }
    }
}






int main() {
    /**
     * Declare some array pointers to use later with malloc, and a max sized input array
     */
    char *firstInput = "";
    char *secondInput = "";
    char *thirdInput = "";
    char *fourthInput = "";
    char *fifthInput = "";
    char *sixthInput = "";
    char input[256] = "";
    /**
     * Gets name of home path (maybe i should do current directory instead?) https://www.tutorialspoint.com/c_standard_library/c_function_getenv.htm
     */
    char *h = getenv("HOME");
    /**
     * This is used for changing directory later on
     */
    slash = "/";
    /**
     * Allocate memory on heap for the path that is HOME+/
     */
    char *fullPath = malloc((strlen(h) + strlen(slash)) * sizeof(char));
    strcpy(fullPath, h);
    strcat(fullPath, slash);



    while (TRUE) {
        isPipe = 0;
        arrayCleaner(input, sizeof(input));
        /**
         * Show current path + $ in the console
         */
        printf("%s $\t", fullPath);


        /**
         * Read input of whole line.
         * Instead of scanf() cause i had problems  https://www.quora.com/How-do-I-read-and-display-a-full-sentence-in-C
         */
        scanf("%[^\n]%*c", input);


        /**
         * Check for pipe
         */
        for (int j = 0; j < strlen(input); ++j) {
            if (input[j] == '|') {
                isPipe = 1;
            }
        }


        /**
         * Splits input into arrays
         */
        firstInput = inputSplitter(input, 1);
        secondInput = inputSplitter(input, 2);
        thirdInput = inputSplitter(input, 3);
        fourthInput = inputSplitter(input,4);
        fifthInput = inputSplitter(input,5);
        sixthInput = inputSplitter(input,6);



        if (strcmp("cd", firstInput) == 0) {
            cdCommand(fullPath, h, secondInput);
        }

        else if (strcmp("ls", firstInput) == 0) {
            lsCommand(fullPath, secondInput,thirdInput,fourthInput,fifthInput,sixthInput);
        }

            /**
             * will clear console but only through command line
             * https://stackoverflow.com/questions/2347770/how-do-you-clear-the-console-screen-in-c
             */
        else if (strcmp("clear", firstInput) == 0) {
            system("clear\n");
        }

            /**
             * Echoes everything after echo and space
             */
        else if (strcmp("echo", firstInput) == 0) {
            for (int i = 5; i < strlen(input); ++i) {
                printf("%c", input[i]);
            }
            printf("\n");

        }

        else if (strcmp("pwd", firstInput) == 0){
            printf("%s\n",fullPath);
        }

        else if (strcmp("mkdir", firstInput) == 0){
            mkdirCommand(fullPath,secondInput);
        }

        else if (strcmp("rmdir", firstInput) == 0) {
            rmdirCommand(fullPath,secondInput);
        }

        else if (strcmp("grep", firstInput) == 0) {
            grepCommand(fullPath,secondInput,thirdInput,0,slash,slash);
        }

        else if (strcmp("cat", firstInput) == 0) {
            catCommand(fullPath,secondInput,fourthInput,fifthInput);
        }

        else {
            printf("Command not found\n");
        }



        /**
         * Clean the arrays for next loop and flush input from keyboard
         */
        free(firstInput);
        free(secondInput);
        free(thirdInput);
        free(fourthInput);
        free(fifthInput);
        free(sixthInput);
        fflush(stdin);
    }
    return 0;
}
