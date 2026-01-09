#include <algorithm>
#include <array>
#include <bits/floatn-common.h>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <iterator>
#include <vector>

//////////////////////////////////////////////////
///
/// ATTEMPTING MVR PART 9
///
/// YESIR TARGET of 0 ms is HIT BOI
///
/// IDK what I'll try but I'll try something
///
/// PD: I rearanged the lower bound and index calculations
/// AND OMG that made it single digit micro second solution
///
/// PD 2: Eliminated the excessive __builtin_popcount calls
/// and updated MVRs directly before the backtrack is called
///
/// PD 3: Eliminated the references in update_option calls
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

                        [7844  ns , 8421   ns]

                        [7     μs , 7      μs]

 ************************************************/

class Solution
{
private:
  constexpr static auto BOARD_SIZE = 81;
  constexpr static auto ROW_SIZE = 9;

  constexpr static const std::array<int8_t, BOARD_SIZE> IDX
      = { 0,  0,  0,  0,  0,  0,  0,  0,  0,  9,  9,  9,  9,  9,  9,  9,  9,  9,  18, 18, 18, 18, 18, 18, 18, 18, 18,
          27, 27, 27, 27, 27, 27, 27, 27, 27, 36, 36, 36, 36, 36, 36, 36, 36, 36, 45, 45, 45, 45, 45, 45, 45, 45, 45,
          54, 54, 54, 54, 54, 54, 54, 54, 54, 63, 63, 63, 63, 63, 63, 63, 63, 63, 72, 72, 72, 72, 72, 72, 72, 72, 72 };

  constexpr static const std::array<int8_t, BOARD_SIZE> IDG
      = { 0,  0,  0,  3,  3,  3,  6,  6,  6,  0,  0,  0,  3,  3,  3,  6,  6,  6,  0,  0,  0,  3,  3,  3,  6,  6,  6,
          27, 27, 27, 30, 30, 30, 33, 33, 33, 27, 27, 27, 30, 30, 30, 33, 33, 33, 27, 27, 27, 30, 30, 30, 33, 33, 33,
          54, 54, 54, 57, 57, 57, 60, 60, 60, 54, 54, 54, 57, 57, 57, 60, 60, 60, 54, 54, 54, 57, 57, 57, 60, 60, 60 };

  constexpr static const std::array<int8_t, BOARD_SIZE> ROW
      = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4,
          4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8 };
  constexpr static const std::array<int8_t, BOARD_SIZE> COL
      = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 0, 1, 2, 3, 4, 5, 6, 7, 8, 0, 1, 2, 3, 4, 5, 6, 7, 8, 0, 1, 2, 3, 4, 5, 6, 7, 8, 0, 1, 2, 3, 4,
          5, 6, 7, 8, 0, 1, 2, 3, 4, 5, 6, 7, 8, 0, 1, 2, 3, 4, 5, 6, 7, 8, 0, 1, 2, 3, 4, 5, 6, 7, 8, 0, 1, 2, 3, 4, 5, 6, 7, 8 };
  constexpr static const std::array<int8_t, BOARD_SIZE> BOX
      = { 0, 0, 0, 1, 1, 1, 2, 2, 2, 0, 0, 0, 1, 1, 1, 2, 2, 2, 0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 3, 3, 3, 4, 4,
          4, 5, 5, 5, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 7, 8, 8, 8, 6, 6, 6, 7, 7, 7, 8, 8, 8, 6, 6, 6, 7, 7, 7, 8, 8, 8 };

private: // custom data container
  struct __attribute__((packed)) data
  {
    uint16_t stack;
    int8_t MVR;
    uint8_t LOC;
    bool operator<(const data &other) const
    {
      return MVR < other.MVR;
    }
  };

private: // holders
  std::array<int8_t, BOARD_SIZE> board;
  std::array<data, BOARD_SIZE> stack_data;
  std::array<uint16_t, ROW_SIZE> row_m, col_m, box_m;

private: // pointers to stack_data to suffle arround and sort
  std::array<data *, BOARD_SIZE> stack;

public:
  void solveSudoku(std::vector<std::vector<char>> &sudoku)
  {
    long prevhash = 0, hash = 0;
    int i = 0;
    for(int y = 0; y < ROW_SIZE; y++)
    {
      row_m[y] = 0;
      col_m[y] = 0;
      box_m[y] = 0;
      for(int x = 0; x < ROW_SIZE; x++)
      {
        board[i] = sudoku[y][x] - '1';
        stack_data[i].stack = 0b0000000111111111;
        stack_data[i].MVR = ROW_SIZE;
        i++;
      }
    }
    for(i = 0; i < BOARD_SIZE; i++)
    {
      stack_data[i].LOC = i;
      if(board[i] >= 0)
      {
        uint16_t SHF = 1 << board[i];
        uint16_t NSHF = ~SHF;
        stack_data[i].stack = SHF;

        // TURN OFF
        row_m[ROW[i]] &= NSHF;
        col_m[COL[i]] &= NSHF;
        box_m[BOX[i]] &= NSHF;
        //

        stack_data[i].MVR = 1;
        hash += stack_data[i].MVR;

        update_option(i, SHF, NSHF);
      }
    }
    while(prevhash != hash)
    {
      prevhash = hash;
      hash = 0;
      for(i = 0; i < BOARD_SIZE; i++)
      {
        if(stack_data[i].MVR == 1)
        {
          board[i] = __builtin_ctz(stack_data[i].stack);

          uint16_t SHF = 1 << board[i];
          uint16_t NSHF = ~SHF;
          update_option(i, SHF, NSHF);

          stack_data[i].stack = SHF;

          // TURN OFF
          row_m[ROW[i]] &= NSHF;
          col_m[COL[i]] &= NSHF;
          box_m[BOX[i]] &= NSHF;
          //
        }
        hash += stack_data[i].MVR;
      }
    }
    for(i = 0; i < BOARD_SIZE; i++)
    {
      stack_data[i].stack = stack_data[i].stack;
      stack_data[i].MVR = __builtin_popcount(stack_data[i].stack);
      stack[i] = &stack_data[i];
    }

    //// THIS TAKES FUCKING 8.84% of CPU TIME WTF
    std::sort(stack.begin(), stack.end(), [](const data *a, const data *b) { return a->MVR < b->MVR; });
    ////

    auto low_bd = std::lower_bound(stack.begin(), stack.end(), 2, [](const data *d, int8_t e) { return d->MVR < e; });
    uint8_t st = std::distance(stack.begin(), low_bd);

    if(!backtrack_MVR(st))
    {
      return;
    }

    i = 0;
    for(int y = 0; y < ROW_SIZE; y++)
    {
      for(int x = 0; x < ROW_SIZE; x++)
      {
        sudoku[y][x] = board[i] + '1';
        i++;
      }
    }
  }

private:
  bool backtrack_MVR(uint8_t idx)
  {
    if(idx >= BOARD_SIZE)
    {
      return true;
    }
    uint8_t loc = stack[idx]->LOC;

    while(stack[idx]->MVR)
    {
      stack[idx]->MVR--;
      int8_t s = __builtin_ctz(stack[idx]->stack);

      uint16_t SHF = 1 << s;
      uint16_t NSHF = ~SHF;

      stack[idx]->stack &= NSHF;

      if(!(row_m[ROW[loc]] & SHF) && !(col_m[COL[loc]] & SHF) && !(box_m[BOX[loc]] & SHF))
      {
        board[loc] = s;
        row_m[ROW[loc]] |= SHF;
        col_m[COL[loc]] |= SHF;
        box_m[BOX[loc]] |= SHF;

        // UPDATE:
        //// according to valgrind this takes < 1% of CPU time
        //// therefore its not a bottleneck
        std::array<data, 81> stack_copy;
        memcpy(&stack_copy, &stack_data, sizeof(stack_data));
        //// so its actually very efficient

        /// this function still takes 20.2% of CPU time
        update_option(loc, SHF, NSHF);
        ///

        /// and this takes 32.74 % of CPU time
        std::partial_sort(stack.begin() + idx + 1, stack.begin() + idx + 2, stack.end(),
                          [](const data *a, const data *b) { return a->MVR < b->MVR; });
        ///

        if(backtrack_MVR(idx + 1))
          return true;
        row_m[ROW[loc]] &= NSHF;
        col_m[COL[loc]] &= NSHF;
        box_m[BOX[loc]] &= NSHF;
        board[loc] = -1;
        /// same as the prev memcpy
        memcpy(&stack_data, &stack_copy, sizeof(stack_data));
        ///
      }
    }
    return false;
  }

  inline void update_option(const int &i, const uint16_t &SHF, const uint16_t &NSHF)
  {
    static int j, k;

    for(j = IDX[i]; j < IDX[i] + ROW_SIZE; j++)
    {
      if(stack_data[j].stack & SHF)
      {
        stack_data[j].stack &= NSHF;
        stack_data[j].MVR--;
      }
    }
    for(j = i - IDX[i]; j < BOARD_SIZE; j += ROW_SIZE)
    {
      if(stack_data[j].stack & SHF)
      {
        stack_data[j].stack &= NSHF;
        stack_data[j].MVR--;
      }
    }
    for(j = IDG[i]; j < IDG[i] + 27; j += ROW_SIZE)
    {
      for(k = j; k < j + 3; k++)
      {
        if(stack_data[k].stack & SHF)
        {
          stack_data[k].stack &= NSHF;
          stack_data[k].MVR--;
        }
      }
    }
  }
};
