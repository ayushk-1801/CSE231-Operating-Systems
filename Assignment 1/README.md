# Assignment 1

## Q1
This helper function generates a random number between 1 and n (in this case, between 1 and 100).
```c
int randomNumber(int n) { return (rand() % n) + 1; }
```

```c
Each child process:
- Seeds the random number generator using `srand(time(0) ^ getpid())` to ensure different seeds across processes.
- Generates four random numbers and computes their average.
- Prints the mean and its PID.
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

- The parent creates seven child processes using `fork()`.
- After creating each child, the parent waits for the child to complete using `wait()`.
- The PID of the completed child is printed by the parent.
```c
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

## Q2
- Binary search is an efficient algorithm for finding an element in a sorted array.
- The search space is halved with each step by comparing the target with the middle element:
    - If the target is smaller than the middle, search the left half.
    - If the target is larger, search the right half.
- `fork()` creates a child process, which runs independently from the parent process.
- In this program, the parent waits (`wait()`) for the child to finish searching before terminating.

```c
void binarySearch(int arr[], int low, int high, int target) {
    if (low > high) {
        printf("-1\n");
        exit(0);
    }
    int mid = low + (high - low) / 2;
    if (arr[mid] == target) {
        printf("Target value's index: %d", mid);
        exit(0);
    }
    int rc = fork();
    if (rc < 0) {
        printf("Fork Failed!\n");
        exit(1);
    } else if (rc == 0) {
        if (arr[mid] > target) {
            binarySearch(arr, low, mid - 1, target);
        } else {
            binarySearch(arr, mid + 1, high, target);
        }
    } else {
        wait(NULL);
        exit(0);
    }
}
```

