#include <chrono>
#include <cstdint>
#include <iostream>
#include <vector>

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
  inline bool isValidSudoku(std::vector<std::vector<char>> &board)
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

    for(j = 0; j < 9; j++)
    {
      X = 0;
      Y = 0;
      MASK = 0;
      YMASK = 0;
      for(i = 0; i < 9; i++)
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
      if(((X >> 4) != (MASK >> 4)) || ((Y >> 4) != (YMASK >> 4)))
        return false;
    }

    // Unrolled the 3x3 board loops to reduce array index calculations
    // and limit checks, minimizing overhead.
    // Resulted in approximately a 25% improvement (~100 ns saved)
    // from the original ~450 ns without loop unrolling.

    X = 0;
    MASK = 0;
    for(j = 0; j < 3; j++)
    {
      for(i = 0; i < 3; i++)
      {
        MEL = (std::uint64_t)(board[j][i] - '.');
        MEL = MEL << 2;
        MEL = 1ULL << MEL;
        MASK |= MEL;
        X += MEL;
      }
      if(((X >> 4) != (MASK >> 4)))
        return false;
    }
    X = 0;
    MASK = 0;
    for(j = 0; j < 3; j++)
    {
      for(i = 3; i < 6; i++)
      {
        MEL = (std::uint64_t)(board[j][i] - '.');
        MEL = MEL << 2;
        MEL = 1ULL << MEL;
        MASK |= MEL;
        X += MEL;
      }
      if(((X >> 4) != (MASK >> 4)))
        return false;
    }
    X = 0;
    MASK = 0;
    for(j = 0; j < 3; j++)
    {
      for(i = 6; i < 9; i++)
      {
        MEL = (std::uint64_t)(board[j][i] - '.');
        MEL = MEL << 2;
        MEL = 1ULL << MEL;
        MASK |= MEL;
        X += MEL;
      }
      if(((X >> 4) != (MASK >> 4)))
        return false;
    }
    X = 0;
    MASK = 0;
    for(j = 3; j < 6; j++)
    {
      for(i = 0; i < 3; i++)
      {
        MEL = (std::uint64_t)(board[j][i] - '.');
        MEL = MEL << 2;
        MEL = 1ULL << MEL;
        MASK |= MEL;
        X += MEL;
      }
      if(((X >> 4) != (MASK >> 4)))
        return false;
    }
    X = 0;
    MASK = 0;
    for(j = 3; j < 6; j++)
    {
      for(i = 3; i < 6; i++)
      {
        MEL = (std::uint64_t)(board[j][i] - '.');
        MEL = MEL << 2;
        MEL = 1ULL << MEL;
        MASK |= MEL;
        X += MEL;
      }
      if(((X >> 4) != (MASK >> 4)))
        return false;
    }
    X = 0;
    MASK = 0;
    for(j = 3; j < 6; j++)
    {
      for(i = 6; i < 9; i++)
      {
        MEL = (std::uint64_t)(board[j][i] - '.');
        MEL = MEL << 2;
        MEL = 1ULL << MEL;
        MASK |= MEL;
        X += MEL;
      }
      if(((X >> 4) != (MASK >> 4)))
        return false;
    }
    X = 0;
    MASK = 0;
    for(j = 6; j < 9; j++)
    {
      for(i = 0; i < 3; i++)
      {
        MEL = (std::uint64_t)(board[j][i] - '.');
        MEL = MEL << 2;
        MEL = 1ULL << MEL;
        MASK |= MEL;
        X += MEL;
      }
      if(((X >> 4) != (MASK >> 4)))
        return false;
    }
    X = 0;
    MASK = 0;
    for(j = 6; j < 9; j++)
    {
      for(i = 3; i < 6; i++)
      {
        MEL = (std::uint64_t)(board[j][i] - '.');
        MEL = MEL << 2;
        MEL = 1ULL << MEL;
        MASK |= MEL;
        X += MEL;
      }
      if(((X >> 4) != (MASK >> 4)))
        return false;
    }

    X = 0;
    MASK = 0;
    for(j = 6; j < 9; j++)
    {
      for(i = 6; i < 9; i++)
      {
        MEL = (std::uint64_t)(board[j][i] - '.');
        MEL = MEL << 2;
        MEL = 1ULL << MEL;
        MASK |= MEL;
        X += MEL;
      }
      if(((X >> 4) != (MASK >> 4)))
        return false;
    }

    return true;
  }
};

int main()
{
  // Example Sudoku board (empty cells as '.')
  std::vector<std::vector<char>> board = { { '5', '3', '.', '.', '7', '.', '.', '.', '.' }, { '6', '.', '.', '1', '9', '5', '.', '.', '.' },
                                           { '.', '9', '8', '.', '.', '.', '.', '6', '.' }, { '8', '.', '.', '.', '6', '.', '.', '.', '3' },
                                           { '4', '.', '.', '8', '.', '3', '.', '.', '1' }, { '7', '.', '.', '.', '2', '.', '.', '.', '6' },
                                           { '.', '6', '.', '.', '.', '.', '2', '8', '.' }, { '.', '.', '.', '4', '1', '9', '.', '.', '5' },
                                           { '.', '.', '.', '.', '8', '.', '.', '7', '9' } };
  std::vector<std::vector<char>> exp_res = { { '5', '3', '4', '6', '7', '8', '9', '1', '2' }, { '6', '7', '2', '1', '9', '5', '3', '4', '8' },
                                             { '1', '9', '8', '3', '4', '2', '5', '6', '7' }, { '8', '5', '9', '7', '6', '1', '4', '2', '3' },
                                             { '4', '2', '6', '8', '5', '3', '7', '9', '1' }, { '7', '1', '3', '9', '2', '4', '8', '5', '6' },
                                             { '9', '6', '1', '5', '3', '7', '2', '8', '4' }, { '2', '8', '7', '4', '1', '9', '6', '3', '5' },
                                             { '3', '4', '5', '2', '8', '6', '1', '7', '9' } };

  // Print input board
  std::cout << "input Sudoku:\n";
  for(int i = 0; i < 9; i++)
  {
    for(int j = 0; j < 9; j++)
    {
      std::cout << exp_res[i][j] << " ";
      if(j == 2 || j == 5)
        std::cout << "  ";
    }
    std::cout << "\n";
    if(i == 2 || i == 5)
      std::cout << "\n";
  }
  Solution solver;

  /*
   * FOR TESTBENCHING 10000000 FUNCTION CALL AVERAGE TIME
   */

  std::vector<bool> outs;
  constexpr int iterations = 10000000;
  auto total_duration = std::chrono::nanoseconds(0);

  for(int i = 0; i < iterations; ++i)
  {
    auto start = std::chrono::high_resolution_clock::now();
    volatile bool result = solver.isValidSudoku(exp_res);
    auto stop = std::chrono::high_resolution_clock::now();
    outs.push_back(result);
    total_duration += stop - start;
  }

  auto average_duration = total_duration / iterations;
  std::cout << "Average Execution Time of the " << iterations
            << " iterations is : " << std::chrono::duration_cast<std::chrono::nanoseconds>(average_duration).count() << " ns\n";

  bool bolita = true;
  for(auto bol : outs)
  {
    if(bol)
      continue;
    bolita = false;
    break;
  }
  if(bolita)
  {
    std::cout << "\033[1;32mTRUE \033[0m";
  }
  else
  {
    std::cout << "\033[1;31mFALSE \033[0m";
  }
  std::cout << '\n';

  //* TESTBENCH for the 507 testcases of leetcode
  //  sum of the time will be returned (its random)

  /*
 constexpr int iterations = 507;

 auto total_duration = std::chrono::nanoseconds (0);

 for (int i = 0; i < iterations; ++i)
    {

       for (int p = 0; p < 9; ++p)
          {
             for (int q = 0; q < 9; ++q)
                {
                   // since the FASTEST algorithm I checks all squares
                   // (if valid if not it halts and return false)
                   // to test speed its irrelevant whats inside the
                   // board
                   // (to some degree don't be annoying)

                   board[p][q] = '1' + rand () % 8;
                }
          }
       auto start = std::chrono::high_resolution_clock::now ();
       // volatile so compiler doesnt optimize it away lol
       volatile bool result = solver.isValidSudoku (exp_res);
       auto stop = std::chrono::high_resolution_clock::now ();
       total_duration += stop - start;
    }

 std::cout << "TOTAL SUM OF EXECUTION Time of the " << iterations
           << " 507 random boards is : \n"
           << "\033[1;32m"
           << "\t"
           << std::chrono::duration_cast<std::chrono::nanoseconds> (
                  total_duration)
                  .count ()
           << " ns\n"
           << "\033[0m";

 std::cout << "\033[1;32m"
           << "\t"

           << std::chrono::duration_cast<std::chrono::microseconds> (
                  total_duration)
                  .count ()
           << " μs\n"
           << "\033[0m";

 std::cout << "\033[1;32m"
           << "\t"
           << std::chrono::duration_cast<std::chrono::milliseconds> (
                  total_duration)
                  .count ()
           << " ms\n"
           << "\033[0m";
           */

  // Individual test
  /*
   auto start = std::chrono::high_resolution_clock::now ();
   bool result = solver.isValidSudoku (exp_res);
   auto stop = std::chrono::high_resolution_clock::now ();

   auto times_ns
       = std::chrono::duration_cast<std::chrono::nanoseconds> (stop - start);
   auto times_mcs
       = std::chrono::duration_cast<std::chrono::microseconds> (stop - start);
   auto times_mils
       = std::chrono::duration_cast<std::chrono::milliseconds> (stop - start);

   std::cout << "\nExecuting solver.isValidSudoku() took:\n";
   std::cout << "\t" << times_ns.count () << " ns\n";
   std::cout << "\t" << times_mcs.count () << " μs\n";
   std::cout << "\t" << times_mils.count () << " ms\n";
  */

  return 0;
};
