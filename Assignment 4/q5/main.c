#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void runit(const char *program, char *const lists[]){
    pid_t pid = fork();
    if (pid < 0) {
        perror("Fork Failed!");
        return 1;
    }
    else if (pid == 0){
        execvp(program, lists);
        perror("Error!");
        return 1;
    }
    else{
        wait(NULL);
    }
}

int main(){
    printf("Running 'list' program...\n");
    char *list[] = {"./list", "..", NULL};
    runit("./list", list);

    printf("\nRunning 'countw' program...\n");
    char *countw[] = {"./countw", "Makefile", NULL};
    runit("./countw", countw);

    printf("\nRunning 'copy' program...\n");
    char *copy[] = {"./copy", "list.c", "../list.c", NULL};
    runit("./copy", copy);

    printf("\nRunning 'move' program...\n");
    char *move[] = {"./move", "q5", "q5_files", NULL};
    runit("./move", move);

    printf("\nAll tasks completed.\n");
    return 0;
}
