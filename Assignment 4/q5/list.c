#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

int main(int argc, char *argv[]) {
    const char *path;
    if(argc == 1){
        path = ".";
    }
    else if(argc == 2){
        path = argv[1];
    }
    else{
        printf("Invalid input!");
        return 1;
    }

    DIR *src = opendir(path);
    struct dirent *subsrc;
    if (src == NULL) {
        perror("Error in Opening directory");
        return 1;
    }
    while((subsrc = readdir(src)) != NULL){
        printf("%s\n", subsrc->d_name);
    }
    closedir(src);
    return 0;
}
