# **INSTALLATION**: 
Unzip package with shell files --> open terminal and go to .../ShellProject/
--> Compile the files with "gcc -o shell shell.c" --> Write command "./shell".

# **RUNNING**:
The command will not take all commands from a linux terminal, but will take:   
1. ls ( -a ) ( /_directory_ ) ( | grep _grepWord_ )  
2. cd ( .. ) ( /_directoryName_ ) ( ~ ) ( _directoryName_ )
3. mkdir _dirName_
4. rmdir _dirName_
5. grep _grepWord_ _fileName_
6. cat _fileName_ ( | grep _grepWord_ )    

Everything inside the parenthesis is optional and ls -a | grep _grepWord_ works together.   

# **BIBLIOGRAPHY**:
1. https://codeforwin.org/2018/03/c-program-check-file-or-directory-exists-not.html
2. https://stackoverflow.com/questions/845556/how-to-ignore-hidden-files-with-opendir-and-readdir-in-c-library
3. https://stackoverflow.com/questions/13450809/how-to-search-a-string-in-a-char-array-in-c
4. https://www.tutorialspoint.com/c_standard_library/c_function_getenv.htm
5. https://www.quora.com/How-do-I-read-and-display-a-full-sentence-in-C
6. https://stackoverflow.com/questions/2347770/how-do-you-clear-the-console-screen-in-c