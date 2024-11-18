#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
    int rc1 = fork();
    if(rc1<0){
        printf("Fork Failed!\n");
        exit(1);
    }
    else if(rc1==0){
        // printf("Running child (PID: %d)\n", getpid());
        execl("./date", "./date", NULL);
        exit(1);
    }

    int rc2 = fork();
    if(rc2<0){
        printf("Fork Failed!\n");
        exit(1);
    }
    else if(rc2==0){
        // printf("Running child (PID: %d)\n", getpid());
        execl("./cal", "./cal", argv[1], argv[2], NULL);
        exit(1);
    }

    int rc3 = fork();
    if(rc3<0){
        printf("Fork Failed!\n");
        exit(1);
    }
    else if(rc3==0){
        // printf("Running child (PID: %d)\n", getpid());
        execl("./uptime", "./uptime", NULL);
        exit(1);
    }

    for(int i=0; i<3; i++){
        wait(NULL);
        // printf("Finished child (PID: %d)\n", wc);
    }

    // printf("All child processes have finished.\n");
    return 0;
}
