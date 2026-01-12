#include "../include/solver.h"
#include <chrono>
#include <iostream>

inline bool isValidSudokuwithempies(std::vector<std::vector<char>> &board)
{
  char pp;
  unsigned char i, j;
  // using ~ 304 Bytes in MASKS and X,Y,G
  std::uint64_t MEL, MASK, YMASK[9], GMASK[9];
  std::uint64_t X, Y[9], G[9];

  memset(&Y[0], 0, sizeof(std::uint64_t) * 9);
  memset(&YMASK[0], 0, sizeof(std::uint64_t) * 9);
  memset(&G[0], 0, sizeof(std::uint64_t) * 9);
  memset(&GMASK[0], 0, sizeof(std::uint64_t) * 9);

  // a minor memory hungry array.... 81 Bytes......
  const unsigned char IDS[9][9] = { { 0, 0, 0, 1, 1, 1, 2, 2, 2 }, { 0, 0, 0, 1, 1, 1, 2, 2, 2 }, { 0, 0, 0, 1, 1, 1, 2, 2, 2 },
                                    { 3, 3, 3, 4, 4, 4, 5, 5, 5 }, { 3, 3, 3, 4, 4, 4, 5, 5, 5 }, { 3, 3, 3, 4, 4, 4, 5, 5, 5 },
                                    { 6, 6, 6, 7, 7, 7, 8, 8, 8 }, { 6, 6, 6, 7, 7, 7, 8, 8, 8 }, { 6, 6, 6, 7, 7, 7, 8, 8, 8 } };

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

  for(j = 0; j < 9; j++)
  {
    X = 0;
    MASK = 0;
    for(i = 0; i < 9; i++)
    {
      pp = board[j][i];
      if(pp == '.')
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
    if((X >> 4) != (MASK >> 4))
      return false;
  }

  for(j = 0; j < 9; j++)
  {
    if((Y[j] >> 4) != (YMASK[j] >> 4))
      return false;
    if((G[j] >> 4) != (GMASK[j] >> 4))
      return false;
  }

  return true;
}
// be aware edited so that if it finds a point it retuns false;
inline bool isValidSudoku(std::vector<std::vector<char>> &board)
{
  char pp;
  unsigned char i, j;
  // using ~ 304 Bytes in MASKS and X,Y,G
  std::uint64_t MEL, MASK, YMASK[9], GMASK[9];
  std::uint64_t X, Y[9], G[9];

  memset(&Y[0], 0, sizeof(std::uint64_t) * 9);
  memset(&YMASK[0], 0, sizeof(std::uint64_t) * 9);
  memset(&G[0], 0, sizeof(std::uint64_t) * 9);
  memset(&GMASK[0], 0, sizeof(std::uint64_t) * 9);

  // a minor memory hungry array.... 81 Bytes......
  const unsigned char IDS[9][9] = { { 0, 0, 0, 1, 1, 1, 2, 2, 2 }, { 0, 0, 0, 1, 1, 1, 2, 2, 2 }, { 0, 0, 0, 1, 1, 1, 2, 2, 2 },
                                    { 3, 3, 3, 4, 4, 4, 5, 5, 5 }, { 3, 3, 3, 4, 4, 4, 5, 5, 5 }, { 3, 3, 3, 4, 4, 4, 5, 5, 5 },
                                    { 6, 6, 6, 7, 7, 7, 8, 8, 8 }, { 6, 6, 6, 7, 7, 7, 8, 8, 8 }, { 6, 6, 6, 7, 7, 7, 8, 8, 8 } };

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

  for(j = 0; j < 9; j++)
  {
    X = 0;
    MASK = 0;
    for(i = 0; i < 9; i++)
    {
      pp = board[j][i];
      if(pp < '1' || pp > '9')
        return false;

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
    if((X >> 4) != (MASK >> 4))
      return false;
  }

  for(j = 0; j < 9; j++)
  {
    if((Y[j] >> 4) != (YMASK[j] >> 4))
      return false;
    if((G[j] >> 4) != (GMASK[j] >> 4))
      return false;
  }

  return true;
}

std::vector<std::vector<std::vector<char>>> boards_gtp = { // testcase 0
  { { '1', '.', '.', '.', '.', '7', '4', '.', '.' },
    { '.', '3', '.', '.', '2', '.', '.', '.', '8' },
    { '.', '.', '9', '6', '.', '.', '.', '.', '.' },
    { '.', '5', '.', '.', '.', '.', '.', '7', '.' },
    { '.', '.', '.', '.', '7', '.', '.', '.', '.' },
    { '.', '.', '.', '3', '.', '.', '.', '.', '.' },
    { '.', '.', '.', '.', '.', '1', '.', '.', '.' },
    { '.', '.', '.', '.', '.', '.', '2', '.', '.' },
    { '.', '.', '8', '.', '.', '.', '.', '.', '.' } },
  // testcase 1
  { { '5', '3', '.', '.', '7', '.', '.', '.', '.' },
    { '6', '.', '.', '1', '9', '5', '.', '.', '.' },
    { '.', '9', '8', '.', '.', '.', '.', '6', '.' },
    { '8', '.', '.', '.', '6', '.', '.', '.', '3' },
    { '4', '.', '.', '8', '.', '3', '.', '.', '1' },
    { '7', '.', '.', '.', '2', '.', '.', '.', '6' },
    { '.', '6', '.', '.', '.', '.', '2', '8', '.' },
    { '.', '.', '.', '4', '1', '9', '.', '.', '5' },
    { '.', '.', '.', '.', '8', '.', '.', '7', '9' } },
  { // Test Case 2
    { '8', '.', '.', '.', '.', '.', '.', '.', '.' },
    { '.', '.', '3', '6', '.', '.', '.', '.', '.' },
    { '.', '7', '.', '.', '9', '.', '2', '.', '.' },
    { '.', '5', '.', '.', '.', '7', '.', '.', '.' },
    { '.', '.', '.', '.', '4', '5', '7', '.', '.' },
    { '.', '.', '.', '1', '.', '.', '.', '3', '.' },
    { '.', '.', '1', '.', '.', '.', '.', '6', '8' },
    { '.', '.', '8', '5', '.', '.', '.', '1', '.' },
    { '.', '9', '.', '.', '.', '.', '4', '.', '.' } },
  { // Test Case 4
    { '.', '.', '9', '7', '4', '8', '.', '.', '.' },
    { '7', '.', '.', '.', '.', '.', '.', '.', '.' },
    { '.', '2', '.', '1', '.', '9', '.', '.', '.' },
    { '.', '.', '7', '.', '.', '.', '2', '4', '.' },
    { '.', '6', '4', '.', '1', '.', '5', '9', '.' },
    { '.', '9', '8', '.', '.', '.', '3', '.', '.' },
    { '.', '.', '.', '8', '.', '3', '.', '2', '.' },
    { '.', '.', '.', '.', '.', '.', '.', '.', '6' },
    { '.', '.', '.', '2', '7', '5', '9', '.', '.' } },
  { // Test Case 5
    { '.', '.', '.', '2', '6', '.', '7', '.', '1' },
    { '6', '8', '.', '.', '7', '.', '.', '9', '.' },
    { '1', '9', '.', '.', '.', '4', '5', '.', '.' },
    { '8', '2', '.', '1', '.', '.', '.', '4', '.' },
    { '.', '.', '4', '6', '.', '2', '9', '.', '.' },
    { '.', '5', '.', '.', '.', '3', '.', '2', '8' },
    { '.', '.', '9', '3', '.', '.', '.', '7', '4' },
    { '.', '4', '.', '.', '5', '.', '.', '3', '6' },
    { '7', '.', '3', '.', '1', '8', '.', '.', '.' } },
  { // Test Case 6
    { '.', '.', '5', '.', '.', '.', '.', '.', '.' },
    { '.', '.', '.', '.', '.', '3', '.', '8', '.' },
    { '.', '7', '.', '.', '1', '.', '2', '.', '.' },
    { '.', '5', '.', '.', '.', '.', '.', '.', '.' },
    { '3', '.', '.', '9', '.', '7', '.', '.', '1' },
    { '.', '.', '.', '.', '.', '.', '.', '6', '.' },
    { '.', '.', '1', '.', '4', '.', '.', '7', '.' },
    { '.', '2', '.', '6', '.', '.', '.', '.', '.' },
    { '.', '.', '.', '.', '.', '.', '3', '.', '.' } },
  { // Test Case 7
    { '8', '.', '.', '.', '.', '.', '.', '.', '.' },
    { '.', '.', '3', '6', '.', '.', '.', '.', '.' },
    { '.', '7', '.', '.', '9', '.', '2', '.', '.' },
    { '.', '5', '.', '.', '.', '7', '.', '.', '.' },
    { '.', '.', '.', '.', '4', '5', '7', '.', '.' },
    { '.', '.', '.', '1', '.', '.', '.', '3', '.' },
    { '.', '.', '1', '.', '.', '.', '.', '6', '8' },
    { '.', '.', '8', '5', '.', '.', '.', '1', '.' },
    { '.', '9', '.', '.', '.', '.', '4', '.', '.' } },
  { // Test Case 8
    { '.', '.', '5', '3', '.', '.', '.', '.', '.' },
    { '8', '.', '.', '.', '.', '.', '.', '2', '.' },
    { '.', '7', '.', '.', '1', '.', '5', '.', '.' },
    { '4', '.', '.', '.', '.', '5', '3', '.', '.' },
    { '.', '1', '.', '.', '7', '.', '.', '.', '6' },
    { '.', '.', '3', '2', '.', '.', '.', '8', '.' },
    { '.', '6', '.', '5', '.', '.', '.', '.', '9' },
    { '.', '.', '4', '.', '.', '.', '.', '3', '.' },
    { '.', '.', '.', '.', '.', '9', '7', '.', '.' } },
  { // Test Case 9
    { '.', '2', '.', '.', '.', '.', '.', '.', '9' },
    { '.', '.', '.', '3', '.', '.', '.', '.', '.' },
    { '8', '.', '4', '.', '.', '.', '.', '.', '.' },
    { '.', '.', '.', '.', '.', '6', '.', '.', '.' },
    { '.', '5', '.', '.', '9', '.', '.', '.', '.' },
    { '.', '.', '.', '.', '.', '.', '2', '.', '.' },
    { '.', '.', '.', '5', '.', '.', '.', '7', '.' },
    { '.', '.', '.', '.', '.', '.', '.', '.', '.' },
    { '.', '.', '.', '.', '.', '.', '.', '.', '.' } },
  { // Test Case 10
    { '.', '.', '.', '.', '.', '.', '.', '.', '.' },
    { '.', '.', '.', '.', '.', '.', '.', '.', '.' },
    { '.', '.', '.', '.', '.', '.', '.', '.', '.' },
    { '.', '.', '.', '.', '.', '.', '.', '.', '.' },
    { '.', '.', '.', '.', '.', '.', '.', '.', '.' },
    { '.', '.', '.', '.', '.', '.', '.', '.', '.' },
    { '.', '.', '.', '.', '.', '.', '.', '.', '.' },
    { '.', '.', '.', '.', '.', '.', '.', '.', '.' },
    { '.', '.', '.', '.', '.', '.', '.', '.', '.' } },
  { // Test Case 11
    { '8', '.', '.', '.', '.', '.', '.', '.', '.' },
    { '.', '.', '3', '6', '.', '.', '.', '.', '.' },
    { '.', '7', '.', '.', '9', '.', '2', '.', '.' },
    { '.', '5', '.', '.', '.', '7', '.', '.', '.' },
    { '.', '.', '.', '.', '4', '5', '7', '.', '.' },
    { '.', '.', '.', '1', '.', '.', '.', '3', '.' },
    { '.', '.', '1', '.', '.', '.', '.', '6', '8' },
    { '.', '.', '8', '5', '.', '.', '.', '1', '.' },
    { '.', '9', '.', '.', '.', '.', '4', '.', '.' } }
};

void test_n_times_no_halt(size_t board_numb, bool prints)
{

  /////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////
  ///////////                                                     /////////
  ///////////        10k call TEST CASE STARTS                    /////////
  ///////////                                                     /////////
  /////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////

  if(board_numb >= boards_gtp.size())
  {
    std::cout << "bruh board_numb inside of test_commulative is > than "
                 "vect size";
    return;
  }
  auto board_sel = boards_gtp[board_numb];
  Solution solver;

  if(prints)
  {
    // Print input board
    std::cout << "input Sudoku in test_n_times for board selected is # " << board_numb << " \n";
    for(int x = 0; x < 9; x++)
    {
      for(int y = 0; y < 9; y++)
      {
        std::cout << board_sel[x][y] << " ";
        if(y == 2 || y == 5)
          std::cout << "  ";
      }
      std::cout << "\n";
      if(x == 2 || x == 5)
        std::cout << "\n";
    }
    std::cout << '\n';
  }

  constexpr int iterations = 10'000;
  auto total_duration_ns = std::chrono::nanoseconds(0);
  auto total_duration_micro = std::chrono::microseconds(0);
  auto start = std::chrono::high_resolution_clock::now();

  for(int i = 0; i < iterations; ++i)
  {
    auto board = board_sel;
    solver.solveSudoku(board);
  }

  auto stop = std::chrono::high_resolution_clock::now();
  total_duration_ns += stop - start;
  total_duration_micro += std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  // outside loop so we can print it
  solver.solveSudoku(board_sel);

  auto average_duration_ns = total_duration_ns / iterations;
  auto average_duration_micro = total_duration_micro / iterations;
  auto average_duration_milli = total_duration_micro / iterations;
  std::cout << "Average Execution Time of the " << iterations << " iterations is : \n"
            << "\t " << std::chrono::duration_cast<std::chrono::nanoseconds>(average_duration_ns).count() << " ns\n"
            << "\t " << std::chrono::duration_cast<std::chrono::microseconds>(average_duration_micro).count() << " μs\n";
  std::cout << "\033[1;32m"
            << "\t" << std::chrono::duration_cast<std::chrono::milliseconds>(average_duration_milli).count() << " ms\n"
            << "\033[0m";

  if(prints)
  {
    // print output board
    std::cout << "OUTPUT Sudoku in test_n_times for board selected is # " << board_numb << " \n";
    for(int x = 0; x < 9; x++)
    {
      for(int y = 0; y < 9; y++)
      {
        std::cout << board_sel[x][y] << " ";
        if(y == 2 || y == 5)
          std::cout << "  ";
      }
      std::cout << "\n";
      if(x == 2 || x == 5)
        std::cout << "\n";
    }
    std::cout << '\n';
  }

  // validate the outpout for correctness
  if(isValidSudoku(board_sel))
  {
    std::cout << "\033[1;32m PROPERLY SOLVED THE boards_gtp[" << board_numb
              << "]\n"
                 "\033[0m";
  }
  else
  {
    std::cout << "\033[1;31m DIDN'T SOLVE THE boards_gtp[" << board_numb << "] \n \033[0m";
  }

  /////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////
  ///////////                                                     /////////
  ///////////        10k call TEST CASE ENDS                      /////////
  ///////////                                                     /////////
  /////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////

  return;
}

void test_n_times_with_average_exec_time(size_t board_numb, bool prints)
{

  /////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////
  ///////////                                                     /////////
  ///////////        10k call TEST CASE STARTS                    /////////
  ///////////                                                     /////////
  /////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////

  if(board_numb >= boards_gtp.size())
  {
    std::cout << "bruh board_numb inside of test_commulative is > than "
                 "vect size";
    return;
  }
  auto board_sel = boards_gtp[board_numb];
  Solution solver;

  if(prints)
  {
    // Print input board
    std::cout << "input Sudoku in test_n_times for board selected is # " << board_numb << " \n";
    for(int x = 0; x < 9; x++)
    {
      for(int y = 0; y < 9; y++)
      {
        std::cout << board_sel[x][y] << " ";
        if(y == 2 || y == 5)
          std::cout << "  ";
      }
      std::cout << "\n";
      if(x == 2 || x == 5)
        std::cout << "\n";
    }
    std::cout << '\n';
  }

  constexpr int iterations = 10000;
  auto total_duration_ns = std::chrono::nanoseconds(0);
  auto total_duration_micro = std::chrono::microseconds(0);

  for(int i = 0; i < iterations; ++i)
  {
    auto board = board_sel;
    auto start = std::chrono::high_resolution_clock::now();
    solver.solveSudoku(board);
    auto stop = std::chrono::high_resolution_clock::now();
    total_duration_ns += stop - start;
    total_duration_micro += std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  }
  // outside loop so we can print it
  solver.solveSudoku(board_sel);

  auto average_duration_ns = total_duration_ns / iterations;
  auto average_duration_micro = total_duration_micro / iterations;
  auto average_duration_milli = total_duration_micro / iterations;
  std::cout << "Average Execution Time of the " << iterations << " iterations is : \n"
            << "\t " << std::chrono::duration_cast<std::chrono::nanoseconds>(average_duration_ns).count() << " ns\n"
            << "\t " << std::chrono::duration_cast<std::chrono::microseconds>(average_duration_micro).count() << " μs\n";
  std::cout << "\033[1;32m"
            << "\t" << std::chrono::duration_cast<std::chrono::milliseconds>(average_duration_milli).count() << " ms\n"
            << "\033[0m";

  if(prints)
  {
    // print output board
    std::cout << "OUTPUT Sudoku in test_n_times for board selected is # " << board_numb << " \n";
    for(int x = 0; x < 9; x++)
    {
      for(int y = 0; y < 9; y++)
      {
        std::cout << board_sel[x][y] << " ";
        if(y == 2 || y == 5)
          std::cout << "  ";
      }
      std::cout << "\n";
      if(x == 2 || x == 5)
        std::cout << "\n";
    }
    std::cout << '\n';
  }

  // validate the outpout for correctness
  if(isValidSudoku(board_sel))
  {
    std::cout << "\033[1;32m PROPERLY SOLVED THE boards_gtp[" << board_numb
              << "]\n"
                 "\033[0m";
  }
  else
  {
    std::cout << "\033[1;31m DIDN'T SOLVE THE boards_gtp[" << board_numb << "] \n \033[0m";
  }

  /////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////
  ///////////                                                     /////////
  ///////////        10k call TEST CASE ENDS                      /////////
  ///////////                                                     /////////
  /////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////

  return;
}

void test_commulative(bool prints)
{

  Solution solver;

  /////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////
  ///////////                                                     /////////
  ///////////        CUMMULATIVE TEST CASE STARTS                 /////////
  ///////////                                                     /////////
  /////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////

  auto total_duration = std::chrono::microseconds(0);

  for(size_t i = 0; i < boards_gtp.size(); i++)
  {
    if(prints)
    {
      // Print input board
      std::cout << "input Sudoku # " << i << " \n";
      for(int x = 0; x < 9; x++)
      {
        for(int y = 0; y < 9; y++)
        {
          std::cout << boards_gtp[i][x][y] << " ";
          if(y == 2 || y == 5)
            std::cout << "  ";
        }
        std::cout << "\n";
        if(x == 2 || x == 5)
          std::cout << "\n";
      }
      std::cout << '\n';
    }
    if(isValidSudokuwithempies(boards_gtp[i]))
    {
      std::cout << "\033[1;32m INPUT # " << i << " IS VALID \n \033[0m";
    }
    else
    {
      std::cout << "\033[1;31m INPUT # " << i << " ISN'T VALID \n \033[0m";
    }

    auto start = std::chrono::high_resolution_clock::now();
    solver.solveSudoku(boards_gtp[i]);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    total_duration += duration;
    if(prints)
    {
      std::cout << "OUTPUT Sudoku # " << i << " \n";
      for(int x = 0; x < 9; x++)
      {
        for(int y = 0; y < 9; y++)
        {
          std::cout << boards_gtp[i][x][y] << " ";
          if(y == 2 || y == 5)
            std::cout << "  ";
        }
        std::cout << "\n";
        if(x == 2 || x == 5)
          std::cout << "\n";
      }
      std::cout << '\n';
    }
    if(isValidSudoku(boards_gtp[i]))
    {
      std::cout << "\033[1;32m PROPERLY SOLVED THE BOARD # " << i << "\n\t IT took = " << duration << " μs\n" << "\033[0m";
    }
    else
    {
      std::cout << "\033[1;31m DIDN'T SOLVE THE BOARD # " << i << "\033[0m" << '\n';
    }
    std::cout << '\n';
  }

  std::cout << "TOTAL SUM OF EXECUTION Time of the " << boards_gtp.size() + 1 // 1 is random board
            << " GPT provided 1 solution boards is : \n";

  std::cout << "\033[1;32m"
            << "\t"

            << std::chrono::duration_cast<std::chrono::microseconds>(total_duration).count() << " μs\n"
            << "\033[0m";

  std::cout << "\033[1;32m"
            << "\t" << std::chrono::duration_cast<std::chrono::milliseconds>(total_duration).count() << " ms\n"
            << "\033[0m";

  /////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////
  ///////////                                                     /////////
  ///////////        CUMMULATIVE TEST CASE ENDS                   /////////
  ///////////                                                     /////////
  /////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////

  return;
}

void test_individual(size_t test_num, bool prints)
{
  Solution solver;
  /////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////
  ///////////                                                     /////////
  ///////////        INDIVIDUAL TEST CASE BEGINS                  /////////
  ///////////                                                     /////////
  /////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////

  if(test_num >= boards_gtp.size())
  {
    std::cout << "bruh board_numb inside of test_commulative is > than "
                 "vect size";
    return;
  }
  auto board_sel = boards_gtp[test_num];

  if(prints)
  {
    std::cout << "input Sudoku boards_gtp[" << test_num << "] \n";
    for(int x = 0; x < 9; x++)
    {
      for(int y = 0; y < 9; y++)
      {
        std::cout << board_sel[x][y] << " ";
        if(y == 2 || y == 5)
          std::cout << "  ";
      }
      std::cout << "\n";
      if(x == 2 || x == 5)
        std::cout << "\n";
    }
    std::cout << '\n';
  }

  auto start = std::chrono::high_resolution_clock::now();
  solver.solveSudoku(board_sel);
  auto stop = std::chrono::high_resolution_clock::now();

  auto times_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
  auto times_mcs = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  auto times_mils = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

  if(prints)
  {
    std::cout << "OUTPUT Sudoku boards_gtp[" << test_num << "] \n";
    for(int x = 0; x < 9; x++)
    {
      for(int y = 0; y < 9; y++)
      {
        std::cout << board_sel[x][y] << " ";
        if(y == 2 || y == 5)
          std::cout << "  ";
      }
      std::cout << "\n";
      if(x == 2 || x == 5)
        std::cout << "\n";
    }
    std::cout << "\n";
  }

  std::cout << "\nExecuting solver.solveSudoku() took:\n";
  std::cout << "\t" << times_ns.count() << " ns\n";
  std::cout << "\t" << times_mcs.count() << " μs\n";
  std::cout << "\t" << times_mils.count() << " ms\n";

  if(isValidSudoku(board_sel))
  {
    std::cout << "\033[1;32m PROPERLY SOLVED THE boards_gtp[" << test_num
              << "]\n"
                 "\033[0m";
  }
  else
  {
    std::cout << "\033[1;31m DIDN'T SOLVE THE boards_gtp[" << test_num << "] \n \033[0m";
  }

  /////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////
  ///////////                                                    /////////
  ///////////        INDIVIDUAL TEST CASE ENDS                   /////////
  ///////////                                                    /////////
  /////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////

  return;
}

int main()
{
  // test_individual(7, true);
  test_n_times_with_average_exec_time(7, false);
  //  test_n_times_no_halt(10, false);
  std::cout << "\033[1;31m "
               "\n\n[----------------------------------------------------------"
               "--------------------------"
            << "] \n \033[0m";
  std::cout << "\033[1;31m [---------------------------------i am a spacer "
               "--------------------------"
            << "] \n \033[0m";
  std::cout << "\033[1;31m "
               "[----------------------------------------------------------"
               "--------------------------"
            << "] \n\n \033[0m";
  // test_commulative(false);
  return 0;
};
