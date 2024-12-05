# File Processing Performance Comparison: `mmap` vs `fread`

## mmap
- **Mechanism:** Maps the file directly into the process's memory space. The OS handles reading pages from disk into memory as needed.
- **Efficiency:**
  - Fewer system calls (`open`, `mmap`, `munmap`).
  - Ideal for large files as it minimizes buffer copying and allows random access.
- **Bottlenecks:**
  - Increased page faults if the file is large and doesn't fit in memory.

---

## fread
- **Mechanism:** Uses the C standard library's buffered I/O to read chunks of the file into user-space memory.
- **Efficiency:**
  - Performs multiple `read` system calls, which can be costly for large files with frequent I/O.
- **Bottlenecks:**
  - More system calls and buffer copying between kernel and user space.

---

## Performance Summary
- **`mmap`** is faster for large datasets due to reduced system call overhead and efficient memory mapping.
- **`fread`** may be simpler for smaller files but incurs higher overhead for repeated I/O operations.

---

## Observations with `strace`
- **`mmap`:**
  - ~3-5 system calls (`open`, `fstat`, `mmap`, `munmap`).
- **`fread`:**
  - Significantly more `read` calls, proportional to the file size and buffer size used.

---

## Recommendation
For **large datasets**, **`mmap`** is preferable due to better system-level optimization and fewer context switches. For smaller or simpler tasks, **`fread`** is sufficient.
