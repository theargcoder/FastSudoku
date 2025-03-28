# FastSudoku
This repository contains an efficient implementation of a Sudoku solving algorithm designed to solve any standard 9x9 Sudoku puzzle. It can be able to solve any board in under 900 micro-seconds. The algorithm employs advanced techniques, such as MVR Backtracking, to optimize the solving process. 

# 🚀 **1 ms Sudoku Solver PID 37 – The Ultimate 50 μs Solution: MVR, Backtracking, Bitmasks, Shifts & Hyper-Optimization ⚡**

## 🔥 IMPORTANT DISCLOSURE

Test benching numbers provided for all examples were obtained on the same board (for the 10k call test) and the same batch of 12 test benches (for cumulative test bench time, similar to LeetCode's).

All versions and testing code can be found in the full **3000 lines of code** on my GitHub [here](https://github.com/theargcoder/LeetCode/tree/main/cpp/problem_id_37), including all versions and progressions between different implementations.

**All test case figures provided were compiled using the `-Ofast` flag and tested on my machine.**

-------------------------------------------------------------------------------------------------------------------------------------------------

### **1st Approach - MVR Backtracking**

**Problems Identified in previous approaches :**
- Excessive board accesses for checking.
- Inefficient conditional branching.
- Inefficient digit selection (looping through bits—what was I thinking?).
- Propositional Table didn't eliminate options with only **one valid value** per row, column, or 3×3 grid, leading to unnecessary backtracking.
- No priority order in cell selection.

**Solutions Applied:**
- Used **3 `std::array<uint16_t, 9>`** masks for fast bitwise validity checks (way faster than looping through options).
- Used `__builtin_popcount` to count set bits in the option field for each cell.
- Used `__builtin_ctz` to extract the LSB digit and validate against masks.
- Implemented **MVR (Minimum Remaining Values)** to prioritize cells with the fewest options.
- Assigned a **MVR value of 127** to constant cells so they get sorted and ignored.
- Iteratively updated the **Propositional Table** until no further simplifications were possible.
- Precomputed all indexes to reduce overhead.

**Benchmark:**
- Average time across **10k calls** (on a hard board).
  - **54 μs**
- **12 hardest boards: (can see in github @theargcoder)**
  - **26 ms** (note its slower, thats due to some of my testcase been empty/emptyish this algorithm is excelent at 1 solution style boards)
- LeetCode Solution Time:
  - **1 ms** *HELL YEAH* (beats 99.03%)

-------------------------------------------------------------------------------------------------------------------------------------------------

### **2nd Approach - MVR Dynamic** 

- [Approach 4: MVR Backtracking Dynamic - Beats 98.86%](#approach-4-mvr-dynamic)

**Key Changes:**
- Designed to **avoid copying back the stack**, at the cost of **using dynamically allocated memory**.
- Without optimization flags, **performance drops significantly** due to repeated `.back()` and `.front()` calls.


**Benchmark:**
- Average time across **10k calls** (on a hard board).
  - **55 μs**
- **12 hardest boards: (can see in github @theargcoder)**
  - **25 ms** (note its slower, thats due to some of my testcase been empty/emptyish this algorithm is excelent at 1 solution style boards)
- LeetCode Solution Time:
  - **2 ms** *HELL YEAH* (beats 98.86%)

-------------------------------------------------------------------------------------------------------------------------------------------------

-------------------------------------------------------------------------------------------------------------------------------------------------

## 💻 **Code Implementations**


