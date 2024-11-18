#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>

int randomNumber(int n) { return (rand() % n) + 1; }

void childProcess() {
    srand(time(0) ^ getpid());
    double ans = 0;
    for (int i = 0; i < 4; i++) {
        ans += randomNumber(100);
    }
    ans /= 4;
    printf("Mean (PID: %d) = %lf\n", getpid(), ans);
}

int main() {
    for (int i = 1; i <= 7; i++) {
        int rc = fork();

        if (rc < 0) {
            printf("Fork Failed!\n");
            exit(1);
        } else if (rc == 0) {
            printf("Executing child %d (PID: %d)\n", i, getpid());
            childProcess();
            exit(0);
        }
    }

    for (int i = 1; i <= 7; i++) {
        int wc = wait(NULL);
        printf("Finished child %d (PID: %d)\n", i, wc);
    }
    return 0;
}
