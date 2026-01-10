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

/*

 {
 //0
   {
      {1,2,3,4,5,6,7,8
      9,18,27,36,45,54,63,72
      10,11,19,20},
      {0,2,3,4,5,6,7,8
      10,19,28,37,46,55,64,73
      9,11,18,20}

   }
 }


*/

class Solution
{
private:
  const constexpr static auto BOARD_SIZE = 81;
  const constexpr static auto PEERS_SIZE = 20;
  const constexpr static auto ROW_SIZE = 9;

  constexpr static const int8_t ROW[BOARD_SIZE]
      = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4,
          4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8 };
  constexpr static const int8_t COL[BOARD_SIZE]
      = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 0, 1, 2, 3, 4, 5, 6, 7, 8, 0, 1, 2, 3, 4, 5, 6, 7, 8, 0, 1, 2, 3, 4, 5, 6, 7, 8, 0, 1, 2, 3, 4,
          5, 6, 7, 8, 0, 1, 2, 3, 4, 5, 6, 7, 8, 0, 1, 2, 3, 4, 5, 6, 7, 8, 0, 1, 2, 3, 4, 5, 6, 7, 8, 0, 1, 2, 3, 4, 5, 6, 7, 8 };
  constexpr static const int8_t BOX[BOARD_SIZE]
      = { 0, 0, 0, 1, 1, 1, 2, 2, 2, 0, 0, 0, 1, 1, 1, 2, 2, 2, 0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 3, 3, 3, 4, 4,
          4, 5, 5, 5, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 7, 8, 8, 8, 6, 6, 6, 7, 7, 7, 8, 8, 8, 6, 6, 6, 7, 7, 7, 8, 8, 8 };

private:
  struct PreComputed
  {
  public:
    uint8_t PEERS[BOARD_SIZE][PEERS_SIZE];

    consteval PreComputed()
    {
      init_peers();
    }

  private:
    // Consteval ensures this function is executed ONLY at compile time
    consteval void init_peers()
    {
      for(int i = 0; i < BOARD_SIZE; ++i)
      {
        int count = 0;
        int r = i / ROW_SIZE;
        int c = i % ROW_SIZE;
        int br = (r / 3) * 3; // Box row start
        int bc = (c / 3) * 3; // Box column start

        for(int x = 0; x < BOARD_SIZE; ++x)
        {
          if(x == i)
          {
            continue;
          }

          int xr = x / ROW_SIZE;
          int xc = x % ROW_SIZE;

          // Check Row, Column, or 3x3 Box
          bool sameRow = (r == xr);
          bool sameCol = (c == xc);
          bool sameBox = (xr >= br && xr < br + 3 && xc >= bc && xc < bc + 3);

          if(sameRow || sameCol || sameBox)
          {
            // Safety check: count should never exceed 19 (indices 0-19)
            PEERS[i][count++] = static_cast<uint8_t>(x);
          }
        }
      }
    }
  };

private:
  PreComputed pre_computed{};

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
  int8_t board[BOARD_SIZE];
  data stack_data[BOARD_SIZE];
  uint16_t row_m[ROW_SIZE];
  uint16_t col_m[ROW_SIZE];
  uint16_t box_m[ROW_SIZE];

private: // pointers to stack_data to suffle arround and sort
  data *stack[BOARD_SIZE];

public:
  void solveSudoku(std::vector<std::vector<char>> &sudoku)
  {
    long prevhash = 0, hash = 0;
    int i = 0;
    for(int y = 0; y < ROW_SIZE; ++y)
    {
      row_m[y] = 0;
      col_m[y] = 0;
      box_m[y] = 0;
      for(int x = 0; x < ROW_SIZE; ++x)
      {
        board[i] = sudoku[y][x] - '1';
        stack_data[i].stack = 0b0000000111111111;
        stack_data[i].MVR = ROW_SIZE;
        i++;
      }
    }
    for(i = 0; i < BOARD_SIZE; ++i)
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
      for(i = 0; i < BOARD_SIZE; ++i)
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
    for(i = 0; i < BOARD_SIZE; ++i)
    {
      stack_data[i].stack = stack_data[i].stack;
      stack_data[i].MVR = __builtin_popcount(stack_data[i].stack);
      stack[i] = &stack_data[i];
    }

    //// THIS TAKES FUCKING 8.84% of CPU TIME WTF
    std::sort(&stack[0], &stack[BOARD_SIZE], [](const data *a, const data *b) { return a->MVR < b->MVR; });
    ////

    auto low_bd = std::lower_bound(&stack[0], &stack[BOARD_SIZE], 2, [](const data *d, int8_t e) { return d->MVR < e; });
    uint8_t st = std::distance(&stack[0], low_bd);

    if(!backtrack_MVR(st))
    {
      return;
    }

    i = 0;
    for(int y = 0; y < ROW_SIZE; ++y)
    {
      for(int x = 0; x < ROW_SIZE; ++x)
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
        // std::partial_sort(stack.begin() + idx + 1, stack.begin() + idx + 2, stack.end(),
        //                  [](const data *a, const data *b) { return a->MVR < b->MVR; });
        ///
        if(!check_scan_and_swap(&stack[idx + 1], &stack[BOARD_SIZE]))
        {
          goto backtrack;
        }

        if(backtrack_MVR(idx + 1))
          return true;

      backtrack:
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

  [[nodiscard]] inline bool check_scan_and_swap(data **beg, data **end)
  {
    auto best_it = beg;
    int8_t min_mvr = INT8_MAX;

    for(auto it = beg; it != end; ++it)
    {
      int8_t val = (*it)->MVR;

      if(val == 0)
      {
        return false; // dead end
      }

      if(val < min_mvr)
      {
        min_mvr = val;
        best_it = it;
      }
    }

    if(best_it != beg)
    {
      data *tmp = *best_it;
      *best_it = *beg;
      *beg = tmp;
    }

    return true;
  }

  inline void update_option(const uint8_t i, const uint16_t SHF, const uint16_t NSHF)
  {
    for(const auto &j : pre_computed.PEERS[i])
    {
      if(stack_data[j].stack & SHF)
      {
        stack_data[j].stack &= NSHF;
        --stack_data[j].MVR;
      }
    }
  }
};
