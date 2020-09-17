#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
//#include <asm/errno.h>
#include <errno.h>
#include <memory.h>
#include <sys/stat.h>

// GLOBAL VARIABLES
int TRUE = 1;


void previousCD(char* fullPath){
    int place = strlen(fullPath)-2;
    fullPath[place+1] = '\0';
    fullPath[place+2] = '\0';
    while(fullPath[place] != '/'){
        fullPath[place--] = '\0';
    }
}


// https://codeforwin.org/2018/03/c-program-check-file-or-directory-exists-not.html
/**
 * Function to check whether a directory exists or not.
 * It returns 1 if given path is directory and  exists
 * otherwise returns 0.
 */
int isDirectoryExists(const char *path)
{
    struct stat stats;

    stat(path, &stats);

    // Check for file existence
    if (S_ISDIR(stats.st_mode))
        return 1;

    return 0;
}






// CLEAN WHOLE ARRAY
void arrayCleaner(char* arr, int length){
    for (int i = 0; i < length; ++i) {
        arr[i] = '\0';
    }
}

char* inputSplitter(char* arr, int inputNumber){
    int spaceCounter=0, placeCounter=0, startpoint=0, endpoint=0;
    while (inputNumber-1 != spaceCounter){
        while (arr[placeCounter] != ' '){
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

    // allocate space for array (also to be used outside of function)
    char *array = malloc((endpoint-startpoint) * sizeof(char));
    arrayCleaner(array, endpoint-startpoint);
    int loopCounter=0;
    for (int i = startpoint; i < endpoint; ++i) {
        array[loopCounter++] = arr[i];
    }
    return array;
}





int main(int argc, char** argv) {
    char* firstInput="";
    char* secondInput="";
    // GETS NAME OF HOME PATH (  sry :(  ) https://www.tutorialspoint.com/c_standard_library/c_function_getenv.htm
    char *h = getenv("HOME");
    char slash[] = "/";
    //char desktop[] = "Desktop";
    char* fullPath = malloc((strlen(h) + strlen(slash)) * sizeof(char ));
    strcpy(fullPath,h);
    strcat(fullPath,slash);


    // DETERMINE MAX INPUT SIZE TO BE SAVE OF OVERFLOW AND TIME EFFICIENCY
    char input[256]="";
    if (argc != 0){
        // DO THE REQUEST

    }


    while(TRUE){
        arrayCleaner(input, sizeof(input));
        // LOOK LIKE BASH
        printf("%s $\t",fullPath);
        // READ INPUT FROM TERMINAL
        //use this to read whole line instead of scanf("%s",input);  https://www.quora.com/How-do-I-read-and-display-a-full-sentence-in-C
        scanf("%[^\n]%*c",input);



        // SPLIT INPUTS INTO
        firstInput = inputSplitter(input, 1);
        secondInput = inputSplitter(input, 2);


        // COMPARE FIRST INPUT WITH cd
        int result = strcmp("cd", firstInput);
        // IF cd USE FUNCTION
        if (result == 0){
            arrayCleaner(input, sizeof(input));
            if(strcmp("..", secondInput) == 0){
                if(strlen(fullPath) == 1){
                    printf("Path is at lowest point\n");
                }
                else previousCD(fullPath);
            }

            // CHECKS CURRENT PATH https://stackoverflow.com/questions/298510/how-to-get-the-current-directory-in-a-c-program
            else {
                //printf("%s\n", input);
                char *tempPath = malloc((strlen(fullPath) + strlen(secondInput) + strlen(slash)) * sizeof(char));
                strcpy(tempPath, fullPath);
                strcat(tempPath, secondInput);
                strcat(tempPath, slash);
                if (isDirectoryExists(tempPath) == 0) {
                    printf("%s\n", strerror(errno));
                } else {
                    arrayCleaner(fullPath,strlen(fullPath));
                    strcpy(fullPath,tempPath);
                }
            }
        }





        arrayCleaner(firstInput,strlen(firstInput));
        arrayCleaner(secondInput,strlen(secondInput));
    }
    return 0;
}
