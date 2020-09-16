#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <asm/errno.h>
#include <errno.h>
#include <memory.h>
#include <sys/stat.h>

// GLOBAL VARIABLES
int TRUE = 1;
char directory[256];


// CD FUNCTION https://stackoverflow.com/questions/12510874/how-can-i-check-if-a-directory-exists
void cdFunction(char* name){
    DIR* dir = opendir(name);
    if (dir){
        printf("cool");
        closedir(dir);
    }
    else if (ENOENT == errno){
        printf("%x\n", ENOENT);
    }
    else{
        printf("some error occured\n");
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
    // GETS NAME OF USER (  sry :(  ) https://www.unix.com/programming/21041-getting-username-c-program-unix.html
    char *p=getenv("USER");
    char slash[] = "/";
    char desktop[] = "Desktop";
    char *normalPath = malloc((strlen(p) + 2 * strlen(slash) + strlen(desktop)) * sizeof(char ));
    strcpy(normalPath,slash);
    strcat(normalPath,p);
    strcat(normalPath,slash);
    strcat(normalPath,desktop);
    printf("%s\n", normalPath);

    printf("%x\n",isDirectoryExists(normalPath));
    free(normalPath);

    arrayCleaner(directory, sizeof(directory));


    // DETERMINE MAX INPUT SIZE TO BE SAVE OF OVERFLOW AND TIME EFFICIENCY
    char input[256];
    if (argc != 0){
        // DO THE REQUEST

    }


    while(TRUE){
        arrayCleaner(input, sizeof(input));
        // LOOK LIKE BASH
        printf("$\t");
        // READ INPUT FROM TERMINAL
        //use this to read whole line instead of scanf("%s",input);  https://www.quora.com/How-do-I-read-and-display-a-full-sentence-in-C
        scanf("%[^\n]%*c",input);



        char* firstInput = inputSplitter(input, 1);
        printf("%s \n", firstInput);



        char* secondInput = inputSplitter(input, 2);
        printf("%s \n", secondInput);

        // COMPARE FIRST INPUT WITH cd
        int result = strcmp("cd", firstInput);
        // IF cd USE FUNCTION
        if (result == 0){
            arrayCleaner(input, sizeof(input));
            // CHECKS CURRENT PATH https://stackoverflow.com/questions/298510/how-to-get-the-current-directory-in-a-c-program
            if (getcwd(input, sizeof(input)) != NULL ){
                printf("%s\n", input);
                char* fullPath = malloc((strlen(input) + strlen(secondInput) + strlen(slash)) * sizeof(char));
                strcpy(fullPath, input);
                strcat(fullPath, slash);
                strcat(fullPath,secondInput);
                printf("%s",fullPath);
                printf("%x",isDirectoryExists(fullPath));
            }
            else{
                printf("%s\n", strerror(errno));
            }
        }







        free(firstInput);
        free(secondInput);
        //printf("%s\n",input);
    }
    return 0;
}
