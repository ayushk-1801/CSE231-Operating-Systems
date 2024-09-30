# Assignment-2222222222222222222222222222222222222222222222
This README file contains the assumptions, taken for each question, and the steps to run the code.

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

## Q3
- Memory Layout: The system uses 16 pages of 4KB each, mapped to physical memory frames.
- Protection Bits: Set to `0` for simplicity, meaning all pages have read/write access. If non-zero protection bits are encountered, the system raises a `PROTECTION_FAULT`.
- Page Validity: All pages are initialized as valid (`isValid = true`).
- To run the code, use the following commands:
```
gcc q3.c -o q3
./q3
```
