# Assignment-2
This README file contains the assumptions, taken for each question, and the steps to run the code.

## Q1
- Fixed Array Size: The array is hardcoded to 16 elements.
- Process Creation: It assumes the system can handle enough processes for recursion depth without hitting limits.
- Pipes for Communication: Parent and child processes communicate through pipes, assuming pipes and communication succeed.
- Equal Split of Array: The array can be evenly split for sorting.
- Homogeneous Data: The array consists of integers.
- No Memory or Fork Failures: Assumes sufficient memory for temporary arrays and no `fork()` failures.
- Non-concurrent Merging: Only sorting is parallel; merging is done serially.
- To run the code, use the following commands:
```
gcc q1.c -o q1
./q1
```

---

## Q2
- The input logical address is a 4-character hexadecimal number.
- The segments are limited to the sizes defined in `CODE_BOUND`, `HEAP_BOUND`, and `STACK_BOUND`.
- No detailed memory protection mechanism is implemented except for simple bounds checking.
- The logical address is considered valid if it fits within the format and does not exceed the segment bounds.
- To run the code, use the following commands:
```
gcc q2.c -o q2
./q2
```

---

## Q3
- Memory Layout: The system uses 16 pages of 4KB each, mapped to physical memory frames.
- Protection Bits: Set to `0` for simplicity, meaning all pages have read/write access. If non-zero protection bits are encountered, the system raises a `PROTECTION_FAULT`.
- Page Validity: All pages are initialized as valid (`isValid = true`).
- To run the code, use the following commands:
```
gcc q3.c -o q3
./q3
```

---

## Q4
- Virtual Memory System: Uses a two-level page table (Page Directory and Page Table) with 4 KB pages.
- Fixed Physical Memory: 16 MB of physical memory is allocated.
- Dynamic Page Allocation: Page faults trigger dynamic allocation of page tables and frames.
- Physical Frames: Frames are allocated sequentially in a circular manner.
- Page Table Entries: Each entry stores a frame number and a validity flag.
- Page Hit/Miss: Tracks page hits and misses during load and store operations.
```
gcc q4.c -o q4
./q4
```

---

## Q5
- Fixed Number of Frames: The number of available frames is fixed and provided by the user.
- Fixed Maximum Page Requests: The maximum number of page requests is 1000, and page numbers are 8-bit values (`uint8_t`).
- Clock Replacement Algorithm: The program uses the Clock Page Replacement Algorithm to handle page faults.
- All Pages Initially Empty: All frames are initially empty, with page numbers set to `-1`.
- Reference and Dirty Bits: Each frame has a reference bit (used for the clock algorithm) and a dirty bit, though the dirty bit isn't used in the current logic.
- No Page Modifications: The program assumes that no pages are modified (dirty bit remains `0`), as there's no logic for handling page writes.
- Sequential Page Requests: Page requests are processed sequentially with no regard to any other optimization strategies.
- To run the code, use the following commands:
```
gcc q5.c -o q5
./q5
```
