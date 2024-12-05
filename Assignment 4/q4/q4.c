#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

void part1(const char *input){
    int file1 = open(input,  O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(file1 < 0){
        printf("Error in Opening file1");
        exit(0);
    }
    if(dup2(file1, STDOUT_FILENO) < 0){
        printf("Error redirecting stdout");
        close(file1);
        exit(0);
    }
    printf("This text is written through dup2 in file1.\n");
    close(file1);
}
void part2(const char *input, const char *command){
    int file2 = open(input, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(file2 < 0){
        perror("Error in Opening file2");
        exit(0);
    }
    pid_t pid = fork();
    if(pid < 0){
        perror("fork failed!");
        close(file2);
        exit(0);
    }
    else if(pid == 0){
        if(dup2(file2, STDOUT_FILENO) < 0 || dup2(file2, STDERR_FILENO) < 0){
            perror("Error in dup2!");
            close(file2);
            exit(0);
        }
        execlp("sh", "sh", "-c", command, (char *)NULL);
        perror("Error in given command!");
        close(file2);
        exit(0);
    }
    else{
        if (wait(NULL) < 0) {
            perror("Error in wait");
        }
    }
    close(file2);
}

int main(int argc, char *argv[]){
    if(argc<3){
        printf("Invalid input!\n");
        return 1;
    }
    if(strcmp(argv[1], "-p1")==0 && argc==3){
        part1(argv[2]);
    }
    else if(strcmp(argv[1], "-p2")==0 && argc==4){
        part2(argv[3], argv[2]);
    }
    else{
        printf("Invalid input!\n");
    }
    return 0;
}