# Assignment 1

## Q1
This helper function generates a random number between 1 and n (in this case, between 1 and 100).
```c
int randomNumber(int n) { return (rand() % n) + 1; }
```

Each child process:
- Seeds the random number generator using `srand(time(0) ^ getpid())` to ensure different seeds across processes.
- Generates four random numbers and computes their average.
- Prints the mean and its PID.
```c
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

## Q3
### cal.c
Determines if a year is a leap year, which affects the number of days in February.
```c
int isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}
```
Computes the day of the week for the 1st of a given month using Zeller's Congruence formula. The result is adjusted to match the Gregorian calendar's days of the week.
```c
int zellerCongruence(int day, int month, int year) {
    if (month < 3) {
        month += 12;
        year--;
    }

    int k = year % 100;
    int j = year / 100;

    return ((day + (13 * (month + 1)) / 5 + k + k / 4 + j / 4 - 2 * j) % 7 + 6) % 7;
}
```
- Ensures that exactly two arguments (month and year) are provided and are valid.
- Uses an array of `Month` structs to store month names and the number of days.
- Modifies February's days if the year is a leap year.
- Displays the calendar in a formatted layout.
```c
int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <month> <year>\n", argv[0]);
        return 1;
    }

    int month = atoi(argv[1]);
    int year = atoi(argv[2]);

    if (month < 1 || month > 12 || year < 1) {
        printf("Invalid month or year.\n");
        return 1;
    }

    struct Month months[] = {{"January", 31}, {"February", 28}, {"March", 31},
        {"April", 30},   {"May", 31},      {"June", 30},
        {"July", 31},    {"August", 31},   {"September", 30},
        {"October", 31}, {"November", 30}, {"December", 31}};

    if (isLeapYear(year) && month == 2) {
        months[1].days = 29;
    }

    printf("    %s %d\n", months[month - 1].name, year);
    printf("Su Mo Tu We Th Fr Sa\n");

    int dayOne = zellerCongruence(1, month, year);

    for (int i = 0; i < dayOne; i++) {
        printf("   ");
    }

    for (int i = 1; i <= months[month - 1].days; i++) {
        printf("%2d ", i);
        if ((i + dayOne) % 7 == 0) {
            printf("\n");
        }
    }
    printf("\n");

    return 0;
}
```
