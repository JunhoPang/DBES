#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h> // getopt_long()
#include <unistd.h>  // access()
#include <dirent.h> // opendir()

// This struct value manages if functions of DBES are execute.
struct option_arguments {

    // If user wants each function, value of the function flag is 1.
    // If user don't want each function, value of the function flag is 0.
    int flag_backup;
    int flag_encrypt;
    int flag_localstorage;
    int flag_cloudstorage;
};

// Main Function Define.
int Auto_BackUp();


// Sub Function Define.
int Is_Dir();

enum ErrorCode {

    PATH_NOT_EXIST = -5, PATH_NOT_DIRECTORY, LARGE_PATH_STRING
};



int main(int argc, char *argv[]) {

    struct option_arguments Option_argus = {0, 0, 0, 0};
    struct option_arguments *pOption_argus = &Option_argus;
    int option_cursor;
    int option_index = 0;

    struct option long_options[] = {

        {"backup", no_argument, &(pOption_argus->flag_backup), 1},
        {"encrypt", no_argument, &(pOption_argus->flag_encrypt), 1},
        {"local", no_argument, &(pOption_argus->flag_localstorage), 1},
        {"cloud", no_argument, &(pOption_argus->flag_cloudstorage), 1},
        {0, 0, 0, 0}
    };

    while(1) {

        option_cursor = getopt_long(argc, argv, "", long_options, &option_index);

        // When all options be checked.
        if(option_cursor == -1) {
            break;
        }

        //// When any option was invalid.
        //if(option_cursor == 63) { // getpot_long return '?' when input is invalid string.
        //    printf("%s is invalid option.\n", long_options[option_index].name);
        //    exit(0);
        //}

        //printf("%c\n", option_cursor);
    }

    printf("%d, %d, %d, %d\n", pOption_argus->flag_backup
    , pOption_argus->flag_encrypt
    , pOption_argus->flag_localstorage
    , pOption_argus->flag_cloudstorage);

    // Run Backup function when --backup argument was input.
    if((pOption_argus->flag_backup) == 1) {

        Auto_BackUp();
    }

    return 0;
}

// Backup Function Code.
int Auto_BackUp() {

    int stdin_error_check;

    do {

        puts("Please input directory path where you want to backup");

        stdin_error_check = Is_Dir();

        // printf("%d\n", stdin_error_check);
    } while(stdin_error_check == -3);

    return 0;
}

int Is_Dir() {

    const int maxLength_path_string = 256;
    char path_string[maxLength_path_string];
    char *pEnd_string = NULL;

    // Input Directory String.
    fgets(path_string, maxLength_path_string, stdin);

    if((pEnd_string = strchr(path_string, '\n')) != NULL) {

        *pEnd_string = '\0'; // Remove fgets's '\n'
        // printf("%s\n", path_string);
    }
    else {
        // When input exceeded maxLength_path_string.

        puts("Input directroy path is too much long.");

        // Clear the stdin.
        /* NOTE
        1. %*[^\n] scans everything until a \n, but doesn't scan in the \n.
           The asterisk(*) tells it to discard whatever was scanned.
        
        2. %*c scans a single character, which will be the \n left over by %*[^\n] in this case.
           The asterisk instructs scanf to discard the scanned character.
        */
        scanf("%*[^\n]");
        scanf("%*c");



        // printf("%s\n", path_string);
        return LARGE_PATH_STRING;
    }

    /* Legacy Code
    // When input exceeded maxLength_path_string.
    if(path_string[strlen(path_string) - 1] != '\n') {

        puts("Input directroy path is too much long.");

        printf("%s\n", path_string);

        return -3;
    }

    path_string[strlen(path_string) - 1] = '\0'; // Remove fgets's '\n'
    */


    DIR *dircheck = NULL;

    if(access(path_string, F_OK) != -1) {
        // File OR Directory exists.

        if((dircheck = opendir(path_string)) != NULL) {
            // path_string is Directory.
            
            closedir(dircheck); // But, we don't need diropen. so, close dir.
        }
        else {

            puts("Input directroy path is not directory. maybe it is executable file or something");

            return PATH_NOT_DIRECTORY; // path_string exists, but does not directory.
        }
    }
    else {

        puts("Input directroy path does not exist.");

        return PATH_NOT_EXIST; // path_string does not exist.
    }

    return 1; // path_string exist, and it's directory.
}