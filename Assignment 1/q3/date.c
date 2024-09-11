#include <stdio.h>
#include <string.h>
#include <time.h>

void date(){
    struct tm* ptr;
    time_t t;
    t = time(NULL);
    ptr = localtime(&t);
    printf("IST %s", asctime(ptr));
}

void dateUTC(){
    struct tm* ptr;
    time_t t;
    t = time(NULL);
    ptr = gmtime(&t);
    printf("UTC %s", asctime(ptr));
}

void dateRFC(){
    struct tm* ptr;
    time_t t;
    t = time(NULL);
    ptr = localtime(&t);
    printf("%s", asctime(ptr));
}

int main(int argc, char *argv[]){
    if(argc==1){
        date();
    }
    else if(argc==2){
        if(strcmp(argv[1], "-u") == 0){
            dateUTC();
        }
        else if(strcmp(argv[1], "-r") == 0){
            dateRFC();
        }
        else{
            printf("Incorrect flags!\n");
        }
    }
    else{
        printf("Invalid command!\n");
    }
    return 0;
}
