# 🚀 **Valid Sudoku Board id. 36 – The Ultimate 237 μs Solution: Bitmasks, Shifts & Hyper-Optimization ⚡**

## 🧠 **Intuition & Approach**

### **Initial Approach - The String Search Saga**

- [Approach 1 - String Buffers](#approach-1---string-buffers)  

In my sleep-deprived state, I pursued the classic brute-force approach using `std::string` buffers. The plan was simple:

- **Row Check:** Populate buffer `buffx`
- **Column Check:** Populate buffer `buffy`
- **3x3 Box Check:** Use another buffer and hope for the best.

**Issues Faced:**
- `std::string::find()` was painfully slow.
- Constant memory reallocation and copying slowed things further.
- Triple nested loops for 3x3 boxes made it a sluggish $$O(n^2)$$ algorithm.

**Benchmark:**
- Average time across **10M calls**.
    - **2817 ns** 
- **507 random boards:**
  - **2,807 μs**

---

### **Second Approach - Bit Manipulation**

- [Approach 2 - Bit Manipulation](#approach-2---bit-manipulation)  

**Optimizations Applied:**

- Replaced string buffers with **bit manipulation**.
- Used `unsigned short` (16-bit) integers for bitwise operations.
- Reduced memory consumption by eliminating buffers.

**Benchmark:**
- Average time across **10M calls**.
    - **420 ns** 
- **507 random boards:**
  - **197 μs**

---

### **Third Approach - Tiny Tweaks, Massive Gains**

- [Approach 3: Minimal Memory with Maximum Speed](#approach-3-minimal-memory-with-maximum-speed)  

**Problems Identified:**
- Excessive board accesses.
- Inefficient conditional branching.
- Extra checks for non-digit cells.

**Solutions Applied:**
- Used **2 `unsigned_64_int`** registers for quick bit manipulation.
- Precomputed masks and reduced unnecessary operations.

**Benchmark:**
- Average time across **10M calls**.
    - **320 ns** 
- **507 random boards:**
  - **335 μs**

---

### **Fourth Approach - Memory Be Gone, Speed On!** 🚀

- [Approach 4: GO CRAZY Eliminating Every Microsecond](#approach-4-go-crazy-eliminating-every-microsecond)  

**Key Changes:**

- Reduced memory constraints by accessing each cell **once**.
- Optimized conditional checks.
- Efficiently handled row and grid masks.

**Benchmark:**
- Average time across **10M calls**.
    - **260 ns** 
- **507 random boards:**
    - **237 μs**

---

## 📊 **Complexity**

| Approach | Time Complexity | Space Complexity |
|-----------|-----------------|-------------------|
| 1         | $$O(N^2)$$       | $$O(N)$$ bytes     |
| 2         | $$O(N)$$         | $$O(1)$$ bytes     |
| 3         | $$O(N)$$         | $$O(1)$$ bytes     |
| 4         | $$O(N)$$         | $$O(1)$$ bytes     |

---

## 💻 **Code Implementations**

### **Approach 1 - String Buffers**

```cpp
// very very slow
class Solution
{
 public:
   bool
   isValidSudoku (std::vector<std::vector<char> > &board)
   {
      int size = board.size ();
      for (int i = 0; i < size; i++)
         {
            std::string buffx, buffy;
            for (int j = 0; j < size; j++)
               {
                  if (buffx.find (board[i][j]) != std::string::npos)
                     return false;
                  else if (std::isdigit (board[i][j]))
                     buffx.push_back (board[i][j]);
                  if (buffy.find (board[j][i]) != std::string::npos)
                     return false;
                  else if (std::isdigit (board[j][i]))
                     buffy.push_back (board[j][i]);
               }
         }
      // no rows or columns with repeating numbers now on to the 3x3 boards

      for (int i = 0; i < 3; i++)
         {
            for (int j = 0; j < 3; j++)
               {
                  std::string buff;
                  for (int x = i * 3; x < (i * 3 + 3); x++)
                     {
                        for (int y = j * 3; y < (j * 3 + 3); y++)
                           {
                              if (buff.find (board[x][y]) != std::string::npos)
                                 return false;
                              else if (std::isdigit (board[x][y]))
                                 buff.push_back (board[x][y]);
                           }
                     }
               }
         }
      return true;
   }
};
```

### **Approach 2 - Bit Manipulation**

```cpp
// 0 ms (leetcode solution)
// beats  100  % of solutions in time
// beats  99.6 % of solution in memory
/**********************************************
 *                                            *
 *  TESTED IN:                                *
 *  Linux macbook-air 6.13.7-arch1-1 #1 SMP   *
 *  PREEMPT_DYNAMIC Thu, 13 Mar 2025          *
 *  18:12:00 +0000 x86_64 GNU/Linux           *
 *                                            *
 **********************************************

     Executed 5 runs of 10,000,000
     function calls on the same board.
     Average execution time per call
     ranged from:

                                [409 ns, 422 ns]

 ************************************************/

class Solution
{
 public:
   bool
   isValidSudoku (std::vector<std::vector<char> > &board)
   {
      const unsigned char ID[9] = { 0, 0, 0, 3, 3, 3, 6, 6, 6 };
      const unsigned char LI[9] = { 3, 3, 3, 6, 6, 6, 9, 9, 9 };
      unsigned char MEL;
      unsigned short SEL;
      unsigned short buffx, buffy;

      for (unsigned char j = 0; j < 9; j++)
         {
            buffx = 0;
            buffy = 0;
            for (unsigned char i = 0; i < 9; i++)
               {
                  if (board[j][i] != '.')
                     {
                        MEL = board[j][i] - '1';
                        SEL = 1 << MEL;
                        if (buffx & SEL)
                           return false;
                        buffx |= SEL;
                     }
                  if (board[i][j] != '.')
                     {
                        MEL = board[i][j] - '1';
                        SEL = 1 << MEL;
                        if (buffy & SEL)
                           return false;
                        buffy |= SEL;
                     }
               }
         }
      for (unsigned char j = 0; j < 9; j += 3)
         {
            for (unsigned char i = 0; i < 9; i += 3)
               {
                  buffx = 0;
                  for (unsigned char y = ID[j]; y < LI[j]; y++)
                     {
                        for (unsigned char x = ID[i]; x < LI[i]; x++)
                           {
                              if (board[y][x] == '.')
                                 continue;
                              MEL = board[y][x] - '1';
                              SEL = 1 << MEL;
                              if (buffx & SEL)
                                 return false;
                              buffx |= SEL;
                           }
                     }
               }
         }

      return true;
   }
};

```

### **Approach 3: Minimal Memory with Maximum Speed**

```cpp
// PERFECT BALANCE BETWEEN
// MEMORY EFFICIENT 42 Bytes
// AND SUPER PERFORMANT ~320 ns
/**********************************************
 *                                            *
 *  TESTED IN:                                *
 *  Linux macbook-air 6.13.7-arch1-1 #1 SMP   *
 *  PREEMPT_DYNAMIC Thu, 13 Mar 2025          *
 *  18:12:00 +0000 x86_64 GNU/Linux           *
 *                                            *
 **********************************************

     Executed 5 runs of 10,000,000
     function calls on the same board.
     Average execution time per call
     ranged from:

                                [317 ns, 329 ns]

 ************************************************/

class Solution
{
 public:
   inline bool
   isValidSudoku (std::vector<std::vector<char> > &board)
   {
      unsigned char i, j;
      std::uint64_t MEL, MASK, YMASK;
      std::uint64_t X, Y;

      // OPTIMIZATION #1
      // No need to unroll this loop since the overhead is negligible.
      // The only memory access within the loop is reading from the board,
      // which is unavoidable. Further unrolling wouldn't provide
      // noticeable performance gains.

      // OPTIMIZATION #2
      // Eliminated the if statement inside the 'j' loops to reduce branch
      // predictions. This results in a more consistent execution time since
      // the CPU no longer needs to predict branches 9 times per loop
      // iteration. Applied this optimization across the entire solution.

      for (j = 0; j < 9; j++)
         {
            X = 0;
            Y = 0;
            MASK = 0;
            YMASK = 0;
            for (i = 0; i < 9; i++)
               {
                  MEL = (std::uint64_t)(board[j][i] - '.');
                  MEL = MEL << 2;
                  MEL = 1ULL << MEL;
                  MASK |= MEL;
                  X += MEL;
                  MEL = (std::uint64_t)(board[i][j] - '.');
                  MEL = MEL << 2;
                  MEL = 1ULL << MEL;
                  YMASK |= MEL;
                  Y += MEL;
               }
            if (((X >> 4) != (MASK >> 4)) || ((Y >> 4) != (YMASK >> 4)))
               return false;
         }

      // Unrolled the 3x3 board loops to reduce array index calculations
      // and limit checks, minimizing overhead.
      // Resulted in approximately a 25% improvement (~100 ns saved)
      // from the original ~450 ns without loop unrolling.

      X = 0;
      MASK = 0;
      for (j = 0; j < 3; j++)
         {
            for (i = 0; i < 3; i++)
               {
                  MEL = (std::uint64_t)(board[j][i] - '.');
                  MEL = MEL << 2;
                  MEL = 1ULL << MEL;
                  MASK |= MEL;
                  X += MEL;
               }
            if (((X >> 4) != (MASK >> 4)))
               return false;
         }
      X = 0;
      MASK = 0;
      for (j = 0; j < 3; j++)
         {
            for (i = 3; i < 6; i++)
               {
                  MEL = (std::uint64_t)(board[j][i] - '.');
                  MEL = MEL << 2;
                  MEL = 1ULL << MEL;
                  MASK |= MEL;
                  X += MEL;
               }
            if (((X >> 4) != (MASK >> 4)))
               return false;
         }
      X = 0;
      MASK = 0;
      for (j = 0; j < 3; j++)
         {
            for (i = 6; i < 9; i++)
               {
                  MEL = (std::uint64_t)(board[j][i] - '.');
                  MEL = MEL << 2;
                  MEL = 1ULL << MEL;
                  MASK |= MEL;
                  X += MEL;
               }
            if (((X >> 4) != (MASK >> 4)))
               return false;
         }
      X = 0;
      MASK = 0;
      for (j = 3; j < 6; j++)
         {
            for (i = 0; i < 3; i++)
               {
                  MEL = (std::uint64_t)(board[j][i] - '.');
                  MEL = MEL << 2;
                  MEL = 1ULL << MEL;
                  MASK |= MEL;
                  X += MEL;
               }
            if (((X >> 4) != (MASK >> 4)))
               return false;
         }
      X = 0;
      MASK = 0;
      for (j = 3; j < 6; j++)
         {
            for (i = 3; i < 6; i++)
               {
                  MEL = (std::uint64_t)(board[j][i] - '.');
                  MEL = MEL << 2;
                  MEL = 1ULL << MEL;
                  MASK |= MEL;
                  X += MEL;
               }
            if (((X >> 4) != (MASK >> 4)))
               return false;
         }
      X = 0;
      MASK = 0;
      for (j = 3; j < 6; j++)
         {
            for (i = 6; i < 9; i++)
               {
                  MEL = (std::uint64_t)(board[j][i] - '.');
                  MEL = MEL << 2;
                  MEL = 1ULL << MEL;
                  MASK |= MEL;
                  X += MEL;
               }
            if (((X >> 4) != (MASK >> 4)))
               return false;
         }
      X = 0;
      MASK = 0;
      for (j = 6; j < 9; j++)
         {
            for (i = 0; i < 3; i++)
               {
                  MEL = (std::uint64_t)(board[j][i] - '.');
                  MEL = MEL << 2;
                  MEL = 1ULL << MEL;
                  MASK |= MEL;
                  X += MEL;
               }
            if (((X >> 4) != (MASK >> 4)))
               return false;
         }
      X = 0;
      MASK = 0;
      for (j = 6; j < 9; j++)
         {
            for (i = 3; i < 6; i++)
               {
                  MEL = (std::uint64_t)(board[j][i] - '.');
                  MEL = MEL << 2;
                  MEL = 1ULL << MEL;
                  MASK |= MEL;
                  X += MEL;
               }
            if (((X >> 4) != (MASK >> 4)))
               return false;
         }

      X = 0;
      MASK = 0;
      for (j = 6; j < 9; j++)
         {
            for (i = 6; i < 9; i++)
               {
                  MEL = (std::uint64_t)(board[j][i] - '.');
                  MEL = MEL << 2;
                  MEL = 1ULL << MEL;
                  MASK |= MEL;
                  X += MEL;
               }
            if (((X >> 4) != (MASK >> 4)))
               return false;
         }

      return true;
   }
};
```

### **Approach 4: GO CRAZY Eliminating Every Microsecond**

```cpp
// As fast as I can make it but uses A LOT more memory
/**********************************************
 *                                            *
 *  TESTED IN:                                *
 *  Linux macbook-air 6.13.7-arch1-1 #1 SMP   *
 *  PREEMPT_DYNAMIC Thu, 13 Mar 2025          *
 *  18:12:00 +0000 x86_64 GNU/Linux           *
 *                                            *
 **********************************************

     Executed 5 runs of 10,000,000
     function calls on the same board.
     Average execution time per call
     ranged from:

             for full boards     [260 ns, 269 ns]

             for empish boards   [158 ns, 170 ns]

 ************************************************/

class Solution
{
 public:
   inline bool
   isValidSudoku (std::vector<std::vector<char> > &board)
   {
      char pp;
      unsigned char i, j;
      // using ~ 304 Bytes in MASKS and X,Y,G
      std::uint64_t MEL, MASK, YMASK[9], GMASK[9];
      std::uint64_t X, Y[9], G[9];

      memset (&Y[0], 0, sizeof (std::uint64_t) * 9);
      memset (&YMASK[0], 0, sizeof (std::uint64_t) * 9);
      memset (&G[0], 0, sizeof (std::uint64_t) * 9);
      memset (&GMASK[0], 0, sizeof (std::uint64_t) * 9);

      // a minor memory hungry array.... 81 Bytes......
      const unsigned char IDS[9][9]
          = { { 0, 0, 0, 1, 1, 1, 2, 2, 2 }, { 0, 0, 0, 1, 1, 1, 2, 2, 2 },
              { 0, 0, 0, 1, 1, 1, 2, 2, 2 }, { 3, 3, 3, 4, 4, 4, 5, 5, 5 },
              { 3, 3, 3, 4, 4, 4, 5, 5, 5 }, { 3, 3, 3, 4, 4, 4, 5, 5, 5 },
              { 6, 6, 6, 7, 7, 7, 8, 8, 8 }, { 6, 6, 6, 7, 7, 7, 8, 8, 8 },
              { 6, 6, 6, 7, 7, 7, 8, 8, 8 } };

      // OPTIMIZATION #1
      // No need to unroll this loop since the overhead is negligible.
      // The only memory access within the loop is reading from the board,
      // which is unavoidable. Further unrolling wouldn't provide
      // noticeable performance gains.

      // OPTIMIZATION #2
      // Eliminated the if statement inside the 'j' loops to reduce branch
      // predictions. This results in a more consistent execution time since
      // the CPU no longer needs to predict branches 9 times per loop
      // iteration. Applied this optimization across the entire solution.

      // OPTIMIZATION #3
      // only accesing std::vector once, to reduce overhead
      // skipping '.' since its usefull in reducing time validation when empty
      // board

      for (j = 0; j < 9; j++)
         {
            X = 0;
            MASK = 0;
            for (i = 0; i < 9; i++)
               {
                  pp = board[j][i];
                  if (pp == '.')
                     continue;

                  pp -= '.';
                  pp <<= 2;
                  MEL = 1ULL << pp;

                  // columns
                  MASK |= MEL;
                  X += MEL;

                  // rows
                  YMASK[i] |= MEL;
                  Y[i] += MEL;

                  GMASK[IDS[j][i]] |= MEL;
                  G[IDS[j][i]] += MEL;
               }
            if ((X >> 4) != (MASK >> 4))
               return false;
         }

      for (j = 0; j < 9; j++)
         {
            if ((Y[j] >> 4) != (YMASK[j] >> 4))
               return false;
            if ((G[j] >> 4) != (GMASK[j] >> 4))
               return false;
         }

      return true;
   }
};
```
