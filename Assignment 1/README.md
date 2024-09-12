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

- The parent creates seven child processes using `fork()`.
- After creating each child, the parent waits for the child to complete using `wait()`.
- The PID of the completed child is printed by the parent.

### Execution:
```bash
gcc q1.c -o q1
./q1
```

## Q2
- Binary search is an efficient algorithm for finding an element in a sorted array.
- The search space is halved with each step by comparing the target with the middle element:
    - If the target is smaller than the middle, search the left half.
    - If the target is larger, search the right half.
- `fork()` creates a child process, which runs independently from the parent process.
- In this program, the parent waits (`wait()`) for the child to finish searching before terminating.
### Execution:
```bash
gcc q2.c -o q2
./q2
```

## Q3
### cal.c
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
---
### date.c
- **No arguments:** Displays the local date and time in IST format.
- `-u`: Displays the UTC date and time.
- `-r`: Displays the date and time in the RFC 2822 format.
- **Invalid or extra arguments:** Prints an error message indicating incorrect or invalid flags.
---
### uptime.c
- `struct sysinfo`: A structure used to store various system information.
- `sysinfo(&temp)`: Populates the temp structure with system data. Returns 0 if successful; returns -1 if there is an error.
- Uptime Calculation:
    - The `uptime` field in the `sysinfo` structure represents the number of seconds the system has been running.
    - The program converts this uptime from seconds into hours, minutes, and seconds.
---
### main.c
- Forking and Executing Commands:
    - **First Child:** Executes the `date` command.
    - **Second Child:** Executes the `cal` command with two arguments passed from the command-line.
    - **Third Child:** Executes the `uptime` command.

- Parent Process:
    - Waits for all three child processes to complete using `wait`.
    - Prints the PID of each finished child process and a final message once all children have finished.

- Error Handling:
    - Checks for errors during `fork` and `execl` and exits with an error message if any issues occur.
---
### Makefile
- Targets and Rules:
    - **all:** Default target that builds `date`, `cal`, `uptime`, and `main`.
    - **date:** Compiles `date.c` into an executable named `date`.
    - **cal:** Compiles `cal.c` into an executable named `cal`.
    - **uptime:** Compiles `uptime.c` into an executable named `uptime`.
    - **main:** Compiles `main.c` into an executable named `main`.
- Each compilation uses `gcc` with the `-Wall` option to enable all warnings.
- `clean`: Removes all compiled executables (`date`, `cal`, `uptime`, `main`) to clean up the directory.

### Execution:
```bash
make all
./main <month> <year>
./cal <month> <year>
./uptime
./date <flag>
make clean
```

## Q4

- The program uses a `struct Process` to store information about each process
- Sorting functions are implemented for arrival time and burst time
- Helper functions calculate average response time and average turnaround time
- Processes: 
    1. **FIFO**: Processes are executed in the order they arrive.
    2. **SJF**: Non-preemptive algorithm where the process with the shortest burst time is executed first.
    3. **SRTF**: Preemptive version of SJF, where the process with the shortest remaining time is executed.
    4. **RR**: Processes are executed in a circular queue for a fixed time quantum.

### Execution:
```bash
gcc q4.c -o q4
./q4
```