#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <memory.h>
#include <sys/stat.h>

/**
 * Global variables
 */
int TRUE = 1;
int isPipe = 0;

/**
 *
 * @param fullPath is the char[] which has the path
 */
void previousCD(char* fullPath){
    /** This will first clear the "/" in front and then clear until the next "/"
    *
    */
    int place = strlen(fullPath)-2;
    fullPath[place+1] = '\0';
    while(fullPath[place] != '/'){
        fullPath[place--] = '\0';
    }
}


// https://codeforwin.org/2018/03/c-program-check-file-or-directory-exists-not.html
/**
 * Function to check whether a directory exists or not.
 * It returns 1 if given path is directory exists
 * otherwise returns 0.
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
    while (inputNumber-1 != spaceCounter){
        while (arr[placeCounter] != ' ' && arr[placeCounter] != '\0'){
            placeCounter++;
        }
        placeCounter++;
        spaceCounter++;
    }
    startpoint = placeCounter;
    while (arr[placeCounter] != ' ' && arr[placeCounter] != '\0' && arr[placeCounter] != '\n'){
        placeCounter++;
    }
    endpoint = placeCounter;

    char *array = malloc((endpoint-startpoint) * sizeof(char));
    arrayCleaner(array, endpoint-startpoint);
    int loopCounter=0;
    for (int i = startpoint; i < endpoint; ++i) {
        array[loopCounter++] = arr[i];
    }
    return array;
}





int main(int argc, char** argv) {
    /**
     * Declare some array pointers to use later with malloc, and a max sized input array
     */
    char* firstInput="";
    char* secondInput="";
    char input[256]="";
    /**
     * Gets name of home path (maybe i should do current directory instead?) https://www.tutorialspoint.com/c_standard_library/c_function_getenv.htm
     */
    char *h = getenv("HOME");
    /**
     * This is used for changing directory later on
     */
    char slash[] = "/";
    /**
     * Allocate memory on heap for the path that is HOME+/
     */
    char* fullPath = malloc((strlen(h) + strlen(slash)) * sizeof(char ));
    strcpy(fullPath,h);
    strcat(fullPath,slash);



    /**
     * Check if there is input before starting program and do them
     */
    if (argc != 0){
        // DO THE REQUEST

    }


    while(TRUE){
        isPipe = 0;
        arrayCleaner(input, sizeof(input));
        arrayCleaner(firstInput,strlen(firstInput));
        arrayCleaner(secondInput,strlen(secondInput));
        /**
         * Show current path + $ in the console
         */
        printf("%s $\t",fullPath);


        /**
         * Read input of whole line.
         * Instead of scanf("%s",input);  https://www.quora.com/How-do-I-read-and-display-a-full-sentence-in-C
         */
        scanf("%[^\n]%*c",input);


        /**
         * Check for pipe
         */
        for (int j = 0; j < strlen(input); ++j) {
            if (input[j] ==  '|'){
                isPipe = 1;
            }
        }


        /**
         * Splits input into arrays
         */
        firstInput = inputSplitter(input, 1);
        secondInput = inputSplitter(input, 2);


        /**
         * Compare first input with cd
         * If cd use function
         */
        if (strcmp("cd", firstInput) == 0){
            /**
             * If second input is .. we need to go back 1 directory except if the directory is "/"
             */
            if(strcmp("..", secondInput) == 0){
                if(strlen(fullPath) == 1){
                    printf("Path is at lowest point\n");
                }
                else previousCD(fullPath);
            }

            /**
             *  Checks current path https://stackoverflow.com/questions/298510/how-to-get-the-current-directory-in-a-c-program
             *  Makes a temporary path and checks if it exists, if yes copy the temp path to fullpath
             */
            else if (strlen(secondInput) != 0){
                char *tempPath = malloc((strlen(fullPath) + strlen(secondInput) + strlen(slash)) * sizeof(char));
                strcpy(tempPath, fullPath);
                strcat(tempPath, secondInput);
                strcat(tempPath, slash);
                if (isDirectoryExists(tempPath) == 0) {
                    printf("%s\n", strerror(errno));
                } else {
                    arrayCleaner(fullPath,strlen(fullPath));
                    strcpy(fullPath,tempPath);
                    free(tempPath);
                }
            }
        }

        /**
         * https://stackoverflow.com/questions/845556/how-to-ignore-hidden-files-with-opendir-and-readdir-in-c-library
         * Searches the directory for all files except for ones that start with . or .. (hidden files)
         */
        else if (strcmp("ls",firstInput) == 0){
            DIR* pd = opendir(fullPath);
            struct dirent *cur;
            while (cur = readdir(pd)) {
                if (cur->d_name[0] != '.') {
                    printf("%s\n",cur->d_name);
                }
            }
            closedir(pd);
        }

        /**
         * will clear console but only on linux system
         */
        else if (strcmp("clear",firstInput) == 0){
            system("clear\n");
        }
        /**
         * Echoes everything after echo and space
         */
        else if (strcmp("echo",firstInput) == 0) {
            for (int i = 5; i < strlen(input); ++i) {
                printf("%c",input[i]);
            }
            printf("\n");

        }

        /**
         * cat will output the content of a file given that the file exists
         * max number of characters on a line is set to 256
         */
        else if (strcmp("cat",firstInput) == 0) {
            char *tempPath = malloc((strlen(fullPath) + strlen(secondInput) + strlen(slash)) * sizeof(char));
            strcpy(tempPath, fullPath);
            strcat(tempPath, secondInput);
            FILE *f = fopen(tempPath, "r");
            if (f != NULL) {
                char fileArr[256] = "";
                arrayCleaner(fileArr, strlen(fileArr));
                while (fgets(fileArr, 256, f) != NULL) {
                    printf("%s", fileArr);
                }
            }
            else {
                printf("No file found\n");
            }
            free(tempPath);
        }


        else{
            printf("Command not found\n");
        }


        if (isPipe){
            printf("is pipe\n");
        }


        /**
         * Clean the arrays for next loop and flush input from keyboard
         */
        arrayCleaner(firstInput,strlen(firstInput));
        arrayCleaner(secondInput,strlen(secondInput));
        fflush(stdin);
    }
    return 0;
}
