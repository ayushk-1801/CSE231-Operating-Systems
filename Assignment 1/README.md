# Assignment 1

## Q1
```
int randomNumber(int n) { return (rand() % n) + 1; }
```
This helper function generates a random number between 1 and n (in this case, between 1 and 100).

```
void childProcess() {
    srand(time(0) ^ getpid());
    double ans = 0;
    for (int i = 0; i < 4; i++) {
        ans += randomNumber(100);
    }
    ans /= 4;
    printf("Mean (PID: %d) = %lf\n", getpid(), ans);
}
```
Each child process:
- Seeds the random number generator using srand(time(0) ^ getpid()) to ensure different seeds across processes.
- Generates four random numbers and computes their average.
- Prints the mean and its PID.

```
for (int i = 1; i <= 7; i++) {
    int rc = fork();
    if (rc < 0) {
        printf("Fork Failed!\n");
        exit(1);
    } else if (rc == 0) {
        childProcess();
        exit(0);
    }
}

for (int i = 1; i <= 7; i++) {
    int wc = wait(NULL);
    printf("Finished child %d (PID: %d)\n", i, wc);
}
```
- The parent creates seven child processes using fork().
- After creating each child, the parent waits for the child to complete using wait().
- The PID of the completed child is printed by the parent.

