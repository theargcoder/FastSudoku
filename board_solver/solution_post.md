# 🚀 **1 ms Sudoku Solver PID 37 – The Ultimate 50 μs Solution: MVR, Backtracking, Bitmasks, Shifts & Hyper-Optimization ⚡**

## 🔥 IMPORTANT DISCLOSURE

Test benching numbers provided for all examples were obtained on the same board (for the 10k call test) and the same batch of 12 test benches (for cumulative test bench time, similar to LeetCode's).

All versions and testing code can be found in the full **3000 lines of code** on my GitHub [here](https://github.com/theargcoder/LeetCode/tree/main/cpp/problem_id_37), including all versions and progressions between different implementations.

**All test case figures provided were compiled using the `-Ofast` flag and tested on my machine.**

-------------------------------------------------------------------------------------------------------------------------------------------------

## 🧠 **Intuition & Approach**

### **Initial Approach - Pure Backtracking**

- [Approach 1 - Raw Backtracking - Beats 90.49%](#approach-1---raw-backtracking)

To solve this problem efficiently, I first explored the classic brute-force backtracking method using a **propositional table** (i.e., a bitmask with options) for each cell. The plan was simple:

Each cell's possible values were stored in `options[][]`, respecting the constraints of:
- **Row**
- **Column**
- **3×3 Box**

**Issues Faced:**
- `backtrack` was painfully slow due to trying every possible permutation.
- Early pruning was implemented but wasn't enough.

**Benchmark:**
- Average time across **10k calls** (on a hard board).
  - **771 μs**
- **12 hardest boards: (can see in github @theargcoder)**
  - **14 ms**
- LeetCode Solution Time:
  - **146 ms** *HORRIBLE* (still beats 90.49%)

-------------------------------------------------------------------------------------------------------------------------------------------------

### **Second Approach - Still backtracking**

- [Approach 2 - Raw Backtracking Loop Recursion - Beats 94.36%](#approach-2---raw-backtracking-loop-recursion)

**Optimizations Applied:**
- Instead of using function recursion, I used **loop recursion**, which can be faster in some scenarios (depends on branch prediction behavior).
- Used `uint8_t[81]` instead of recursively accessing a **2D `std::vector`**, which has massive overhead.

**Benchmark:**
- Average time across **10k calls** (on a hard board).
  - **594 μs**
- **12 hardest boards: (can see in github @theargcoder)**
  - **11 ms**
- LeetCode Solution Time:
  - **86 ms** *MEH* (still beats 94.36%)

-------------------------------------------------------------------------------------------------------------------------------------------------

### **Third Approach - MVR Backtracking**

- [Approach 3: MVR Backtracking - Beats 99.03%](#approach-3-mvr-backtracking)

**Problems Identified in Approaches #1 and #2:**
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

### **Fourth Approach - MVR Dynamic** 🚀

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

## 📊 **Complexity**
idk

-------------------------------------------------------------------------------------------------------------------------------------------------

## 💻 **Code Implementations**

### **Approach 1 - Raw Backtracking**

```cpp
// workin slow but working 
/**********************************************
 *                                            *
 *  TESTED IN:                                *
 *  Linux macbook-air 6.13.7-arch1-1 #1 SMP   *
 *  PREEMPT_DYNAMIC Thu, 13 Mar 2025          *
 *  18:12:00 +0000 x86_64 GNU/Linux           *
 *                                            *
 **********************************************

     Executed 5 runs of 10,000
     function calls on the same board.
     Average execution time per call
     ranged from:

                        [304309 ns, 324194 ns]

                        [303    μs, 323    μs]

 ************************************************/

class Solution
{
 private:
   unsigned short options[9][9];
   unsigned short MASK = 0b0000000111111111, SEL;
   unsigned char i, j;
   constexpr static const unsigned char ID[9] = { 0, 0, 0, 3, 3, 3, 6, 6, 6 };
   constexpr static const unsigned char LI[9] = { 3, 3, 3, 6, 6, 6, 9, 9, 9 };
   unsigned char x, y;

 public:
   void
   solveSudoku (std::vector<std::vector<char> > &board)
   {
      std::fill (&options[0][0], &options[0][0] + 81, MASK);

      for (j = 0; j < 9; j++)
         {
            for (i = 0; i < 9; i++)
               {
                  if (board[j][i] != '.')
                     {
                        SEL = (board[j][i] - '1');
                        options[j][i] &= (1 << SEL);
                     }
                  else
                     {
                        for (x = 0; x < 9; x++)
                           {
                              if (x != i && board[j][x] != '.')
                                 {
                                    SEL = (board[j][x] - '1');
                                    options[j][i] &= ~(1 << SEL);
                                 }
                              if (x != j && board[x][i] != '.')
                                 {
                                    SEL = (board[x][i] - '1');
                                    options[j][i] &= ~(1 << SEL);
                                 }
                           }
                        for (y = ID[j]; y < LI[j]; y++)
                           {
                              for (x = ID[i]; x < LI[i]; x++)
                                 {
                                    if (board[y][x] == '.')
                                       continue;

                                    SEL = (board[y][x] - '1');
                                    options[j][i] &= ~(1 << SEL);
                                 }
                           }
                     }
               }
         }
      backtrack (board, 0, 0);
   }

 private:
   bool
   backtrack (std::vector<std::vector<char> > &board, unsigned char p,
              unsigned char q)
   {

      if (p >= 9)
         return true; // done
      if (q >= 9)
         return backtrack (board, p + 1, 0);
      if (board[p][q] != '.')
         return backtrack (board, p, q + 1);

      for (unsigned char r = 0; r < 9; r++)
         {
            if (!(options[p][q] & (1 << r)))
               continue;
            board[p][q] = '1' + r;

            if (validate (board, p, q))
               {

                  if (backtrack (board, p, q + 1))
                     {
                        return true;
                     }
               }
            board[p][q] = '.';
         }
      return false;
   }

   inline bool
   validate (std::vector<std::vector<char> > &sudoku, const unsigned char &p,
             const unsigned char &q)
   {

      for (unsigned char i = 0; i < 9; i++)
         {
            if (i != q && sudoku[p][i] == sudoku[p][q])
               return false;
            if (i != p && sudoku[i][q] == sudoku[p][q])
               return false;
         }
      for (unsigned char y = ID[p]; y < LI[p]; y++)
         {
            for (unsigned char x = ID[q]; x < LI[q]; x++)
               {
                  if (y == p && x == q)
                     continue;
                  if (sudoku[y][x] == sudoku[p][q])
                     {
                        return false;
                     }
               }
         }

      return true;
   }
};

```

### **Approach 2 - Raw Backtracking Loop Recursion**

```cpp
// I have a loop recursion dream
/**********************************************
 *                                            *
 *  TESTED IN:                                *
 *  Linux macbook-air 6.13.7-arch1-1 #1 SMP   *
 *  PREEMPT_DYNAMIC Thu, 13 Mar 2025          *
 *  18:12:00 +0000 x86_64 GNU/Linux           *
 *                                            *
 **********************************************

     Executed 5 runs of 10,000
     function calls on the same board.
     Average execution time per call
     ranged from:

                        [228375 ns, 234703 ns]

                        [227    μs, 234    μs]

 ************************************************/

class Solution
{
 private:
   constexpr static const int IDX[81]
       = { 0,  0,  0,  0,  0,  0,  0,  0,  0,  9,  9,  9,  9,  9,  9,  9,  9,
           9,  18, 18, 18, 18, 18, 18, 18, 18, 18, 27, 27, 27, 27, 27, 27, 27,
           27, 27, 36, 36, 36, 36, 36, 36, 36, 36, 36, 45, 45, 45, 45, 45, 45,
           45, 45, 45, 54, 54, 54, 54, 54, 54, 54, 54, 54, 63, 63, 63, 63, 63,
           63, 63, 63, 63, 72, 72, 72, 72, 72, 72, 72, 72, 72 };

   constexpr static const int IDG[81]
       = { 0,  0,  0,  3,  3,  3,  6,  6,  6,  0,  0,  0,  3,  3,  3,  6,  6,
           6,  0,  0,  0,  3,  3,  3,  6,  6,  6,  27, 27, 27, 30, 30, 30, 33,
           33, 33, 27, 27, 27, 30, 30, 30, 33, 33, 33, 27, 27, 27, 30, 30, 30,
           33, 33, 33, 54, 54, 54, 57, 57, 57, 60, 60, 60, 54, 54, 54, 57, 57,
           57, 60, 60, 60, 54, 54, 54, 57, 57, 57, 60, 60, 60 };

 private:
   constexpr const static unsigned short IS_CONST = 0b1000000000000000;

 private:
   struct data
   {
      unsigned short option = 0b0000000111111111;
      char digit;
      char stack;
   };

 public:
   void
   solveSudoku (std::vector<std::vector<char> > &sudoku)
   {
      unsigned short SEL;
      int i, x, y, p, v;
      unsigned short s;
      data board[81];

      i = 0;
      for (y = 0; y < 9; y++)
         {
            for (x = 0; x < 9; x++)
               {
                  board[i].digit = sudoku[y][x];
                  i++;
               }
         }

      for (i = 0; i < 81; i++)
         {
            if (board[i].digit != '.')
               {
                  // not necesary since it will never be acessed
                  // stack[i] = board[i] - '1';

                  SEL = board[i].digit - '1';
                  board[i].option &= (1 << SEL);

                  // TURN ON THE CONST DIGIT SO IN RECURSION WE SKIP IT
                  board[i].option |= IS_CONST;
               }
            else
               {
                  board[i].stack = 0;

                  for (x = IDX[i]; x < IDX[i] + 9; x++)
                     {
                        if (x != i && board[x].digit != '.')
                           {
                              SEL = board[x].digit - '1';
                              board[i].option &= ~(1 << SEL);
                           }
                     }

                  for (y = i - IDX[i]; y < 81; y += 9)
                     {
                        if (y != i && board[y].digit != '.')
                           {
                              SEL = board[y].digit - '1';
                              board[i].option &= ~(1 << SEL);
                           }
                     }

                  for (y = IDG[i]; y < IDG[i] + 27; y += 9)
                     {
                        for (x = y; x < y + 3; x++)
                           {
                              if (x != i && board[x].digit != '.')
                                 {
                                    SEL = board[x].digit - '1';
                                    board[i].option &= ~(1 << SEL);
                                 }
                           }
                     }
               }
         }
      p = 0;
      while (p < 81)
         {
            if (board[p].option & IS_CONST)
               {
                  p++;
                  continue;
               }
            for (; board[p].stack < 9; board[p].stack++)
               {
                  s = 1 << board[p].stack;
                  if (!(board[p].option & s))
                     continue;

                  board[p].digit = '1' + board[p].stack;

                  v = 1;
                  for (i = IDX[p]; i < p; i++)
                     {
                        if (board[i].digit == board[p].digit)
                           {
                              v = 0;
                              break;
                           }
                     }

                  if (v)
                     {
                        for (i = p - IDX[p]; i < p; i += 9)
                           {
                              if (board[i].digit == board[p].digit)
                                 {
                                    v = 0;
                                    break;
                                 }
                           }
                     }

                  if (v)
                     {
                        for (y = IDG[p]; y < IDG[p] + 27; y += 9)
                           {
                              for (x = y; x < y + 3; x++)
                                 {
                                    if (x >= p)
                                       break;
                                    if (board[x].digit == board[p].digit)
                                       {
                                          v = 0;
                                          break;
                                       }
                                 }
                           }
                     }

                  if (v)
                     {
                        board[p].stack++;
                        break;
                     }
               }

            if (v)
               {
                  p++;
               }
            else
               {
                  // not necesary since validate()
                  // checks up to the curr cel.
                  // board[p] = '.';
                  board[p].stack = 0;
                  if (p == 0)
                     {
                        break;
                     }
                  p--;
                  while (board[p].option & IS_CONST)
                     {
                        p--;
                     }
               }
         }

      i = 0;
      for (y = 0; y < 9; y++)
         {
            for (x = 0; x < 9; x++)
               {
                  sudoku[y][x] = board[i].digit;
                  i++;
               }
         }

      return;
   }
};

```

### **Approach 3 MVR Backtracking**

47 Microsecond solution on most 1 solution boards
NOT THE BEST FOR EMPTY/EMPTYISH boards due to repeated sorting

```cpp
//////////////////////////////////////////////////
///
/// ATTEMPTING MVR PART 4 ILL BE BACK
///
/**********************************************
 *                                            *
 *  TESTED IN:                                *
 *  Linux macbook-air 6.13.7-arch1-1 #1 SMP   *
 *  PREEMPT_DYNAMIC Thu, 13 Mar 2025          *
 *  18:12:00 +0000 x86_64 GNU/Linux           *
 *                                            *
 **********************************************

     Executed 5 runs of 10,000
     function calls on the same board.
     Average execution time per call
     ranged from:

                        [47916  ns , 50756  ns]

                        [47     μs , 50     μs]

 ************************************************/

class Solution
{
 private:
   constexpr static const int8_t IDX[81]
       = { 0,  0,  0,  0,  0,  0,  0,  0,  0,  9,  9,  9,  9,  9,  9,  9,  9,
           9,  18, 18, 18, 18, 18, 18, 18, 18, 18, 27, 27, 27, 27, 27, 27, 27,
           27, 27, 36, 36, 36, 36, 36, 36, 36, 36, 36, 45, 45, 45, 45, 45, 45,
           45, 45, 45, 54, 54, 54, 54, 54, 54, 54, 54, 54, 63, 63, 63, 63, 63,
           63, 63, 63, 63, 72, 72, 72, 72, 72, 72, 72, 72, 72 };

   constexpr static const int8_t IDG[81]
       = { 0,  0,  0,  3,  3,  3,  6,  6,  6,  0,  0,  0,  3,  3,  3,  6,  6,
           6,  0,  0,  0,  3,  3,  3,  6,  6,  6,  27, 27, 27, 30, 30, 30, 33,
           33, 33, 27, 27, 27, 30, 30, 30, 33, 33, 33, 27, 27, 27, 30, 30, 30,
           33, 33, 33, 54, 54, 54, 57, 57, 57, 60, 60, 60, 54, 54, 54, 57, 57,
           57, 60, 60, 60, 54, 54, 54, 57, 57, 57, 60, 60, 60 };

   constexpr static const int8_t ROW[81]
       = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2,
           2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4,
           4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 6, 6,
           7, 7, 7, 7, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8 };
   constexpr static const int8_t COL[81]
       = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 0, 1, 2, 3, 4, 5, 6, 7, 8, 0, 1, 2,
           3, 4, 5, 6, 7, 8, 0, 1, 2, 3, 4, 5, 6, 7, 8, 0, 1, 2, 3, 4, 5,
           6, 7, 8, 0, 1, 2, 3, 4, 5, 6, 7, 8, 0, 1, 2, 3, 4, 5, 6, 7, 8,
           0, 1, 2, 3, 4, 5, 6, 7, 8, 0, 1, 2, 3, 4, 5, 6, 7, 8 };
   constexpr static const int8_t BOX[81]
       = { 0, 0, 0, 1, 1, 1, 2, 2, 2, 0, 0, 0, 1, 1, 1, 2, 2, 2, 0, 0, 0,
           1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 3, 3, 3, 4, 4, 4,
           5, 5, 5, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 7, 8, 8, 8,
           6, 6, 6, 7, 7, 7, 8, 8, 8, 6, 6, 6, 7, 7, 7, 8, 8, 8 };

 private: // custom data container
   struct data
   {
      uint16_t option;
      uint16_t stack;
      int8_t MVR;
      uint8_t LOC;
      bool
      operator< (const data &other) const
      {
         return MVR < other.MVR;
      }
   };

 private: // holders
   std::array<int8_t, 81> board;
   std::array<data, 81> stack;
   std::array<uint16_t, 9> row_m, col_m, box_m;

 private: // end
   uint8_t end;

 public:
   void
   solveSudoku (std::vector<std::vector<char> > &sudoku)
   {
      long prevhash = 0, hash = 0;
      int i = 0;
      for (int y = 0; y < 9; y++)
         {
            row_m[y] = 0;
            col_m[y] = 0;
            box_m[y] = 0;
            for (int x = 0; x < 9; x++)
               {
                  board[i] = sudoku[y][x] - '1';
                  stack[i].option = 0b0000000111111111;
                  i++;
               }
         }
      for (i = 0; i < 81; i++)
         {
            if (board[i] >= 0)
               {
                  stack[i].option &= (1 << board[i]);
                  turn_on (i, board[i]);
                  stack[i].LOC = i;
                  stack[i].MVR = __builtin_popcount (stack[i].option);
                  hash += stack[i].MVR;
               }
            else
               {
                  for (int x = IDX[i]; x < IDX[i] + 9; x++)
                     {
                        if (x != i && board[x] >= 0)
                           {
                              stack[i].option &= ~(1 << board[x]);
                           }
                     }
                  for (int y = i - IDX[i]; y < 81; y += 9)
                     {
                        if (y != i && board[y] >= 0)
                           {
                              stack[i].option &= ~(1 << board[y]);
                           }
                     }
                  for (int y = IDG[i]; y < IDG[i] + 27; y += 9)
                     {
                        for (int x = y; x < y + 3; x++)
                           {
                              if (x != i && board[x] >= 0)
                                 {
                                    stack[i].option &= ~(1 << board[x]);
                                 }
                           }
                     }
                  stack[i].LOC = i;
                  hash += stack[i].MVR;
               }
         }
      while (prevhash != hash)
         {
            prevhash = hash;
            hash = 0;
            for (i = 0; i < 81; i++)
               {
                  int count = __builtin_popcount (stack[i].option);
                  if (count == 1)
                     {
                        int8_t p = __builtin_ctz (stack[i].option);
                        update_option (i, p);
                        board[i] = p;
                        stack[i].option = 1 << p;
                        turn_on (i, p);
                     }
                  hash += count;
               }
         }
      for (i = 0; i < 81; i++)
         {
            stack[i].stack = stack[i].option;
            stack[i].MVR = __builtin_popcount (stack[i].option);
            if (stack[i].MVR == 1)
               stack[i].MVR = 127;
         }

      std::sort (stack.begin (), stack.end ());
      auto low_bd
          = std::lower_bound (stack.begin (), stack.end (), 127,
                              [] (const data &d, int e) { return d.MVR < e; });
      end = std::distance (stack.begin (), low_bd);

      if (!backtrack_MVR (0))
         {
            return;
         }

      i = 0;
      for (int y = 0; y < 9; y++)
         {
            for (int x = 0; x < 9; x++)
               {
                  sudoku[y][x] = board[i] + '1';
                  i++;
               }
         }
   }

 private:
   template <typename It>
   void
   insertionSort (It begin, It end)
   {
      for (It i = begin + 1; i < end; ++i)
         {
            data key = *i;
            It j = i - 1;
            while (j >= begin && key < *j)
               {
                  *(j + 1) = *j;
                  --j;
               }
            *(j + 1) = key;
         }
   }
   bool
   backtrack_MVR (uint8_t idx)
   {
      if (idx >= end)
         return true;
      uint8_t loc = stack[idx].LOC;

      while (stack[idx].MVR)
         {
            stack[idx].MVR--;
            int8_t s = __builtin_ctz (stack[idx].stack);
            stack[idx].stack &= ~(1 << s);
            if (validate (loc, s))
               {
                  board[loc] = s;
                  turn_on (loc, s);

                  // bottleneck
                  std::array<data, 81> stack_copy;
                  memcpy (&stack_copy, &stack, sizeof (stack));
                  // bottleneck

                  update_stack (idx, s, end);
                  std::sort (stack.begin () + idx + 1, stack.begin () + end);
                  if (backtrack_MVR (idx + 1))
                     return true;
                  turn_off (loc, s);
                  board[loc] = -1;
                  memcpy (&stack, &stack_copy, sizeof (stack));
               }
         }
      return false;
   }
   inline void
   update_option (const int &i, int8_t &p)
   {
      for (int j = IDX[i]; j < IDX[i] + 9; j++)
         {
            stack[j].option &= ~(1 << p);
         }
      for (int j = i - IDX[i]; j < 81; j += 9)
         {
            stack[j].option &= ~(1 << p);
         }
      for (int j = IDG[i]; j < IDG[i] + 27; j += 9)
         {
            for (int k = j; k < j + 3; k++)
               {
                  stack[k].option &= ~(1 << p);
               }
         }
   }

   inline void
   turn_on (const uint8_t &p, const int8_t &s)
   {
      uint16_t tmp = 1 << s;
      row_m[ROW[p]] |= tmp;
      col_m[COL[p]] |= tmp;
      box_m[BOX[p]] |= tmp;
   }
   inline void
   turn_off (const uint8_t &p, const int8_t &s)
   {
      uint16_t tmp = 1 << s;
      row_m[ROW[p]] &= ~tmp;
      col_m[COL[p]] &= ~tmp;
      box_m[BOX[p]] &= ~tmp;
   }

   inline bool
   validate (const uint8_t &p, const int8_t &s)
   {
      uint16_t tmp = 1 << s;
      return !(row_m[ROW[p]] & tmp) && !(col_m[COL[p]] & tmp)
             && !(box_m[BOX[p]] & tmp);
   }

   inline void
   update_stack (const uint8_t &i, const int8_t &p, const uint8_t &end)
   {
      uint8_t loc = stack[i].LOC;
      const uint8_t r = ROW[loc];
      const uint8_t c = COL[loc];
      const uint8_t b = BOX[loc];
      uint16_t mask = ~(1 << p);
      for (uint8_t idx = 0; idx < end; idx++)
         {
            uint8_t curr_loc = stack[idx].LOC;
            if (ROW[curr_loc] == r || COL[curr_loc] == c || BOX[curr_loc] == b)
               {
                  uint16_t &s = stack[idx].stack;
                  if (s & ~mask)
                     {
                        s &= mask;
                        stack[idx].MVR--;
                     }
               }
         }
   }
};
```

### **Approach 4: MVR Dynamic**

```cpp

//////////////////////////////////////////////////
///
/// ATTEMPTING MVR PART 3 ILL BE BACK
///
///       USING HEAP SUCKS ASS BRO
///        WE LOST 10 microseconds only in heap ops
///
/**********************************************
 *                                            *
 *  TESTED IN:                                *
 *  Linux macbook-air 6.13.7-arch1-1 #1 SMP   *
 *  PREEMPT_DYNAMIC Thu, 13 Mar 2025          *
 *  18:12:00 +0000 x86_64 GNU/Linux           *
 *                                            *
 **********************************************

     Executed 5 runs of 10,000
     function calls on the same board.
     Average execution time per call
     ranged from:

                        [47916  ns , 50756  ns]

                        [47     μs , 50     μs]

 ************************************************/

class Solution
{
 private:
   constexpr static const int8_t IDX[81]
       = { 0,  0,  0,  0,  0,  0,  0,  0,  0,  9,  9,  9,  9,  9,  9,  9,  9,
           9,  18, 18, 18, 18, 18, 18, 18, 18, 18, 27, 27, 27, 27, 27, 27, 27,
           27, 27, 36, 36, 36, 36, 36, 36, 36, 36, 36, 45, 45, 45, 45, 45, 45,
           45, 45, 45, 54, 54, 54, 54, 54, 54, 54, 54, 54, 63, 63, 63, 63, 63,
           63, 63, 63, 63, 72, 72, 72, 72, 72, 72, 72, 72, 72 };

   constexpr static const int8_t IDG[81]
       = { 0,  0,  0,  3,  3,  3,  6,  6,  6,  0,  0,  0,  3,  3,  3,  6,  6,
           6,  0,  0,  0,  3,  3,  3,  6,  6,  6,  27, 27, 27, 30, 30, 30, 33,
           33, 33, 27, 27, 27, 30, 30, 30, 33, 33, 33, 27, 27, 27, 30, 30, 30,
           33, 33, 33, 54, 54, 54, 57, 57, 57, 60, 60, 60, 54, 54, 54, 57, 57,
           57, 60, 60, 60, 54, 54, 54, 57, 57, 57, 60, 60, 60 };

   constexpr static const int8_t ROW[81]
       = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2,
           2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4,
           4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 6, 6,
           7, 7, 7, 7, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8 };
   constexpr static const int8_t COL[81]
       = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 0, 1, 2, 3, 4, 5, 6, 7, 8, 0, 1, 2,
           3, 4, 5, 6, 7, 8, 0, 1, 2, 3, 4, 5, 6, 7, 8, 0, 1, 2, 3, 4, 5,
           6, 7, 8, 0, 1, 2, 3, 4, 5, 6, 7, 8, 0, 1, 2, 3, 4, 5, 6, 7, 8,
           0, 1, 2, 3, 4, 5, 6, 7, 8, 0, 1, 2, 3, 4, 5, 6, 7, 8 };
   constexpr static const int8_t BOX[81]
       = { 0, 0, 0, 1, 1, 1, 2, 2, 2, 0, 0, 0, 1, 1, 1, 2, 2, 2, 0, 0, 0,
           1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 3, 3, 3, 4, 4, 4,
           5, 5, 5, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 7, 8, 8, 8,
           6, 6, 6, 7, 7, 7, 8, 8, 8, 6, 6, 6, 7, 7, 7, 8, 8, 8 };

 private: // custom data container
   struct data
   {
      uint16_t option;
      uint16_t stack;
      int8_t MVR;
      uint8_t LOC;
      bool
      operator< (const data &other) const
      {
         return MVR < other.MVR;
      }
   };

 private: // holders
   std::array<int8_t, 81> board;
   std::array<uint16_t, 9> row_m, col_m, box_m;

 private: // holders
   std::vector<std::array<data, 81> *> stack_ptrs;

 private: // end
   uint8_t end;

 public:
   void
   solveSudoku (std::vector<std::vector<char> > &sudoku)
   {
      if (!stack_ptrs.empty ())
         stack_ptrs.clear ();

      stack_ptrs.push_back (new std::array<data, 81>{});

      long prevhash = 0, hash = 0;
      int i = 0;
      for (int y = 0; y < 9; y++)
         {
            row_m[y] = 0;
            col_m[y] = 0;
            box_m[y] = 0;
            for (int x = 0; x < 9; x++)
               {
                  board[i] = sudoku[y][x] - '1';
                  (*stack_ptrs.front ())[i].option = 0b0000000111111111;
                  i++;
               }
         }
      for (i = 0; i < 81; i++)
         {
            if (board[i] >= 0)
               {
                  (*stack_ptrs.front ())[i].option &= (1 << board[i]);
                  turn_on (i, board[i]);
                  (*stack_ptrs.front ())[i].LOC = i;
                  (*stack_ptrs.front ())[i].MVR
                      = __builtin_popcount ((*stack_ptrs.front ())[i].option);
                  hash += (*stack_ptrs.front ())[i].MVR;
               }
            else
               {
                  for (int x = IDX[i]; x < IDX[i] + 9; x++)
                     {
                        if (x != i && board[x] >= 0)
                           {
                              (*stack_ptrs.front ())[i].option
                                  &= ~(1 << board[x]);
                           }
                     }
                  for (int y = i - IDX[i]; y < 81; y += 9)
                     {
                        if (y != i && board[y] >= 0)
                           {
                              (*stack_ptrs.front ())[i].option
                                  &= ~(1 << board[y]);
                           }
                     }
                  for (int y = IDG[i]; y < IDG[i] + 27; y += 9)
                     {
                        for (int x = y; x < y + 3; x++)
                           {
                              if (x != i && board[x] >= 0)
                                 {
                                    (*stack_ptrs.front ())[i].option
                                        &= ~(1 << board[x]);
                                 }
                           }
                     }
                  (*stack_ptrs.front ())[i].LOC = i;
                  hash += (*stack_ptrs.front ())[i].MVR;
               }
         }
      while (prevhash != hash)
         {
            prevhash = hash;
            hash = 0;
            for (i = 0; i < 81; i++)
               {
                  int count
                      = __builtin_popcount ((*stack_ptrs.front ())[i].option);
                  if (count == 1)
                     {
                        int8_t p
                            = __builtin_ctz ((*stack_ptrs.front ())[i].option);
                        update_option (i, p);
                        board[i] = p;
                        (*stack_ptrs.front ())[i].option = 1 << p;
                        turn_on (i, p);
                     }
                  hash += count;
               }
         }
      for (i = 0; i < 81; i++)
         {
            (*stack_ptrs.front ())[i].stack = (*stack_ptrs.front ())[i].option;
            (*stack_ptrs.front ())[i].MVR
                = __builtin_popcount ((*stack_ptrs.front ())[i].option);
            if ((*stack_ptrs.front ())[i].MVR == 1)
               (*stack_ptrs.front ())[i].MVR = 127;
         }

      std::sort ((*stack_ptrs.front ()).begin (),
                 (*stack_ptrs.front ()).end ());
      auto low_bd = std::lower_bound (
          (*stack_ptrs.front ()).begin (), (*stack_ptrs.front ()).end (), 127,
          [] (const data &d, int e) { return d.MVR < e; });
      end = std::distance ((*stack_ptrs.front ()).begin (), low_bd);

      if (!backtrack_MVR (0))
         {
            return;
         }

      i = 0;
      for (int y = 0; y < 9; y++)
         {
            for (int x = 0; x < 9; x++)
               {
                  sudoku[y][x] = board[i] + '1';
                  i++;
               }
         }
      // Proper deallocation
      for (auto ptr : stack_ptrs)
         {
            delete ptr; // NOT delete[] ptr;
         }
   }

 private:
   bool
   backtrack_MVR (uint8_t idx)
   {
      if (idx >= end)
         return true;
      uint8_t loc = (*stack_ptrs.back ())[idx].LOC;

      while ((*stack_ptrs.back ())[idx].MVR)
         {
            (*stack_ptrs.back ())[idx].MVR--;
            int8_t s = __builtin_ctz ((*stack_ptrs.back ())[idx].stack);
            (*stack_ptrs.back ())[idx].stack &= ~(1 << s);
            if (validate (loc, s))
               {
                  board[loc] = s;
                  turn_on (loc, s);

                  // Save the current stack size
                  size_t originalSize = stack_ptrs.size ();

                  // Bottleneck: Push a copy of the current state.
                  auto ptr = new std::array<data, 81>{};
                  memcpy (ptr, stack_ptrs.back (), sizeof (*ptr));
                  stack_ptrs.push_back (ptr);
                  // End bottleneck

                  update_stack (idx, s, end);
                  std::sort ((*stack_ptrs.back ()).begin () + idx + 1,
                             (*stack_ptrs.back ()).begin () + end);

                  if (backtrack_MVR (idx + 1))
                     return true;

                  // Backtracking: restore state
                  turn_off (loc, s);
                  board[loc] = -1;

                  // Free all states that were pushed beyond the original
                  // state.
                  for (size_t i = originalSize; i < stack_ptrs.size (); i++)
                     {
                        delete stack_ptrs[i];
                     }
                  stack_ptrs.erase (stack_ptrs.begin () + originalSize,
                                    stack_ptrs.end ());
               }
         }
      return false;
   }
   inline void
   update_option (const int &i, int8_t &p)
   {
      for (int j = IDX[i]; j < IDX[i] + 9; j++)
         {
            (*stack_ptrs.front ())[j].option &= ~(1 << p);
         }
      for (int j = i - IDX[i]; j < 81; j += 9)
         {
            (*stack_ptrs.front ())[j].option &= ~(1 << p);
         }
      for (int j = IDG[i]; j < IDG[i] + 27; j += 9)
         {
            for (int k = j; k < j + 3; k++)
               {
                  (*stack_ptrs.front ())[k].option &= ~(1 << p);
               }
         }
   }

   inline void
   turn_on (const uint8_t &p, const int8_t &s)
   {
      uint16_t tmp = 1 << s;
      row_m[ROW[p]] |= tmp;
      col_m[COL[p]] |= tmp;
      box_m[BOX[p]] |= tmp;
   }
   inline void
   turn_off (const uint8_t &p, const int8_t &s)
   {
      uint16_t tmp = 1 << s;
      row_m[ROW[p]] &= ~tmp;
      col_m[COL[p]] &= ~tmp;
      box_m[BOX[p]] &= ~tmp;
   }

   inline bool
   validate (const uint8_t &p, const int8_t &s)
   {
      uint16_t tmp = 1 << s;
      return !(row_m[ROW[p]] & tmp) && !(col_m[COL[p]] & tmp)
             && !(box_m[BOX[p]] & tmp);
   }

   inline void
   update_stack (const uint8_t &i, const int8_t &p, const uint8_t &end)
   {
      uint8_t loc = (*stack_ptrs.back ())[i].LOC;
      const uint8_t r = ROW[loc];
      const uint8_t c = COL[loc];
      const uint8_t b = BOX[loc];
      uint16_t mask = ~(1 << p);
      for (uint8_t idx = 0; idx < end; idx++)
         {
            uint8_t curr_loc = (*stack_ptrs.back ())[idx].LOC;
            if (ROW[curr_loc] == r || COL[curr_loc] == c || BOX[curr_loc] == b)
               {
                  uint16_t &s = (*stack_ptrs.back ())[idx].stack;
                  if (s & ~mask)
                     {
                        s &= mask;
                        (*stack_ptrs.back ())[idx].MVR--;
                     }
               }
         }
   }
};

```

