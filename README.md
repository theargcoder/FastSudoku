# 🔢 FastSudoku

This repository contains a **super efficient** implementation of a Sudoku solving algorithm capable of handling **any valid** [9x9 Sudoku puzzle](https://github.com/theargcoder/FastSudoku/blob/main/board_validator/main.cpp).

The latest version can solve *almost* any board in **under 9 microseconds (μs)** — that’s **under 9 millionths of a second**!

The algorithm uses advanced techniques like **Minimum Remaining Values (MRV) Backtracking** to accelerate the solving process.

-----------------------------------------------------------------------------------------------------------------------------

## 🔥 IMPORTANT DISCLOSURE 🔥

All benchmark results were obtained under **controlled conditions** using the **selected board** (#x) for the 10k test, and a **fixed batch of 12 test cases** for cumulative timing — similar in style to how **LeetCode** benchmarks.

> ⚠️ All binaries were compiled using the `-Ofast` optimization flag.

**TESTED ON:**  
- `Linux macbook-air 6.13.7-arch1-1 #1 SMP`  
- `PREEMPT_DYNAMIC Thu, 7 Apr 2025`  
- `11:34:00 +0000 x86_64 GNU/Linux`  

-----------------------------------------------------------------------------------------------------------------------------

## ⚡ Benchmark Results (Latest Version)

| Test Case                          | Time (ns) | Time (μs) | Time (ms) |
|-----------------------------------|-----------|-----------|-----------|
| **Board #7 (10k average)**        |   8,218   |     7     |     –     |
| **12-board cumulative test**      |   –       |  6,243    |     6     |
| **Hardest board (#6)**            | 1,920,297 |  1,919    |     1     |
| **Second hardest board (#2)**     | 1,729,112 |  1,728    |     1     |
| **Third hardest board (#10)**     | 1,717,267 |  1,716    |     1     |
| **Empty board**                   |  10,227   |     9     |     0     |
| **Single-cell board**             |   8,981   |     8     |     0     |
| **Three-cell board**              |   8,031   |     7     |     0     |
| **Five-cell board**               |   7,789   |     7     |     0     |

-----------------------------------------------------------------------------------------------------------------------------

## 📥 Download, Compile & Run It Yourself

Clone the repository:

```bash
git clone https://github.com/theargcoder/FastSudoku/
cd FastSudoku
```
### OPTIONAL 

-----------------------------------------------------------------------------------------------------------------------------

#### Edit the following in `board_solver/main.cpp`

- `test_individual`, `test_n_times`, and `test_commulative` functions – modify these to suit your test style.

- `boards_gpt`– update this vector to try out your own boards.

- You can also uncomment previous versions of the solver to benchmark them too!

-----------------------------------------------------------------------------------------------------------------------------

Compile inside `board_solver/` with:

For **some** compiler **optimization**
```bash
 g++ -std=c++17 -Wall -O2 main.cpp -o main
```

For **a lot** of compiler **optimization**
```bash
 g++ -std=c++17 -Wall -O3 main.cpp -o main
```

For **THE MOST** compiler **optimization** possible
```bash
 g++ -std=c++17 -Wall -Ofast main.cpp -o main
```


-----------------------------------------------------------------------------------------------------------------------------


Enjoy the speed. 🧠💨
