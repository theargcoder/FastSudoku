#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <iterator>
#include <vector>

//////////////////////////////////////////////////
///
///   Got it Down to 4 μs with recursion,
///   will try the iterative one as well
///
/**********************************************
 *                                            *
 *  TESTED IN:                                *
 *    Linux ARCHbtw 6.18.4-arch1-1 #1 SM      *
 *    PREEMPT_DYNAMIC                         *
 *    Fri, 09 Jan 2026 19:43:48 +0000         *
 *     x86_64 GNU/Linux                       *
 *  COMPILER:                                 *
 *     g++ (GCC) 15.2.1 20260103              *
 *                                            *
 **********************************************

     Executed 5 runs of 10,000
     function calls on the same board.
     Average execution time per call
     ranged from:

                        [4412  ns , 4833   ns]

                        [4     μs , 4      μs]

 ************************************************/

class Solution
{
private:
  const constexpr static uint8_t BOARD_SIZE = 81;
  const constexpr static uint8_t PEERS_SIZE = 20;
  const constexpr static uint8_t ROW_SIZE = 9;
  const constexpr static uint16_t ALL_BITS_ON = 0b0000000111111111;

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
  uint16_t stack[BOARD_SIZE];

private: // pointers to stack to suffle arround and sort
  uint16_t *stack_view[BOARD_SIZE];
  bool undo[BOARD_SIZE][PEERS_SIZE];

public:
  void solveSudoku(std::vector<std::vector<char>> &sudoku)
  {
    std::memset(stack, ALL_BITS_ON, sizeof(stack));
    std::memset(&undo[0], false, sizeof(undo));

    int prevhash = 0, hash = 0;
    int i = 0;
    int8_t ct;
    for(int y = 0; y < ROW_SIZE; ++y)
    {
      for(int x = 0; x < ROW_SIZE; ++x)
      {
        ct = sudoku[y][x] - '1';
        if(ct >= 0 && ct <= 8)
        {
          const uint16_t SHF = 1 << ct;
          stack[i] = SHF;

          update_option(i, SHF);
        }

        i++;
      }
    }

    while(prevhash != hash)
    {
      prevhash = hash;
      hash = 0;
      for(i = 0; i < BOARD_SIZE; ++i)
      {
        const auto mrv = __builtin_popcount(stack[i]);
        if(mrv == 1)
        {
          update_option(i, stack[i]);
        }
        hash += mrv;
      }
    }

    for(i = 0; i < BOARD_SIZE; ++i)
    {
      stack_view[i] = &stack[i];
    }

    std::sort(&stack_view[0], &stack_view[BOARD_SIZE], [](const auto *a, const auto *b) { return __builtin_popcount(*a) < __builtin_popcount(*b); });

    auto low_bd
        = std::lower_bound(&stack_view[0], &stack_view[BOARD_SIZE], 2, [](const auto *c, int value) { return __builtin_popcount(*c) < value; });

    uint8_t st = std::distance(&stack_view[0], low_bd);

    backtrack_MVR(st);

    i = 0;
    for(int y = 0; y < ROW_SIZE; ++y)
    {
      for(int x = 0; x < ROW_SIZE; ++x)
      {
        sudoku[y][x] = __builtin_ctz(stack[i]) + '1';
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

    auto loc = static_cast<uint8_t>(stack_view[idx] - &stack[0]);

    uint16_t candidates = stack[loc];

    while(candidates)
    {
      int8_t s = __builtin_ctz(candidates);
      uint16_t SHF = 1U << s;
      candidates &= ~SHF; // Remove from local loop tracker

      update_option_undo(loc, SHF);

      if(!check_scan_and_swap(idx + 1))
      {
        goto backtrack;
      }
      if(backtrack_MVR(idx + 1))
      {
        return true;
      }

    backtrack:
      undo_option(loc, SHF);
      std::memset(&undo[loc][0], false, PEERS_SIZE);
    }
    return false;
  }

  [[nodiscard]] bool check_scan_and_swap(uint8_t beg)
  {
    auto best_it = beg;
    auto min_mvr = UINT16_MAX;

    for(auto i = beg; i < BOARD_SIZE; ++i)
    {
      auto val = __builtin_popcount(*stack_view[i]);

      if(val == 0)
      {
        return false; // dead end
      }
      else if(val < min_mvr)
      {
        min_mvr = val;
        best_it = i;
      }
    }

    if(best_it != beg)
    {
      auto *tmp = stack_view[best_it];
      stack_view[best_it] = stack_view[beg];
      stack_view[beg] = tmp;
    }

    return true;
  }

  void update_option(const uint8_t i, const uint16_t SHF)
  {
    for(const auto &j : pre_computed.PEERS[i])
    {
      if(stack[j] & SHF)
      {
        stack[j] &= ~SHF;
      }
    }
  }

  void update_option_undo(const uint8_t i, const uint16_t SHF)
  {
    const auto &peers = pre_computed.PEERS[i];
    for(int j = 0; j < PEERS_SIZE; j++)
    {
      if(stack[peers[j]] & SHF)
      {
        stack[peers[j]] &= ~SHF;
        undo[i][j] = true;
      }
    }
  }

  void undo_option(const uint8_t i, const uint16_t SHF)
  {
    const auto &peers = pre_computed.PEERS[i];
    for(int j = 0; j < PEERS_SIZE; j++)
    {
      if(undo[i][j])
      {
        stack[peers[j]] |= SHF;
      }
    }
  }
};

/////////////////////////////////////////////////////////
///// ITERATIVE SOLUTION, NO FUNCTION RECURSION     /////
/////////////////////////////////////////////////////////
/*
class Solution
{
private:
  const constexpr static auto BOARD_SIZE = 81;
  const constexpr static auto PEERS_SIZE = 20;
  const constexpr static auto ROW_SIZE = 9;

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
  uint16_t stack[BOARD_SIZE];
  uint8_t MRV[BOARD_SIZE];

private: // holders
  int8_t board[BOARD_SIZE];

private: // pointers to stack to suffle arround and sort
  uint8_t *MRV_view[BOARD_SIZE];

public:
  void solveSudoku(std::vector<std::vector<char>> &sudoku)
  {
    long prevhash = 0, hash = 0;
    int i = 0;
    for(int y = 0; y < ROW_SIZE; ++y)
    {
      for(int x = 0; x < ROW_SIZE; ++x)
      {
        board[i] = sudoku[y][x] - '1';
        stack[i] = 0b0000000111111111;
        MRV[i] = ROW_SIZE;
        i++;
      }
    }
    for(i = 0; i < BOARD_SIZE; ++i)
    {
      if(board[i] >= 0)
      {
        uint16_t SHF = 1 << board[i];
        stack[i] = SHF;

        MRV[i] = 1;
        hash += MRV[i];

        update_option(i, SHF);
      }
    }
    while(prevhash != hash)
    {
      prevhash = hash;
      hash = 0;
      for(i = 0; i < BOARD_SIZE; ++i)
      {
        if(MRV[i] == 1)
        {
          board[i] = __builtin_ctz(stack[i]);

          uint16_t SHF = 1 << board[i];
          update_option(i, SHF);

          stack[i] = SHF;
        }
        hash += MRV[i];
      }
    }
    for(i = 0; i < BOARD_SIZE; ++i)
    {
      MRV[i] = __builtin_popcount(stack[i]);
      MRV_view[i] = &MRV[i];
    }

    //// THIS TAKES FUCKING 8.84% of CPU TIME WTF
    std::sort(&MRV_view[0], &MRV_view[BOARD_SIZE], [](const auto *a, const auto *b) { return *a < *b; });
    ////

    auto low_bd = std::lower_bound(&MRV_view[0], &MRV_view[BOARD_SIZE], 2, [](const auto *d, auto e) { return *d < e; });
    uint8_t st = std::distance(&MRV_view[0], low_bd);

    backtrack_MVR(st);

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
  struct __attribute__((packed)) Frame
  {
    uint16_t stack_state[BOARD_SIZE]; // snapshot of stack before any choice at this depth
    uint8_t mrv_state[BOARD_SIZE];    // snapshot of MRV before any choice at this depth
  };

  bool backtrack_MVR(uint idx)
  {
    Frame frames[BOARD_SIZE];
    bool started[BOARD_SIZE];
    std::memset(&started[0], 0, sizeof(started));

    while(true)
    {
      if(idx >= BOARD_SIZE)
      {
        return true;
      }

      uint8_t loc = static_cast<uint8_t>(MRV_view[idx] - &MRV[0]);

      if(!started[idx])
      {
        std::memcpy(frames[idx].stack_state, stack, sizeof(stack));
        std::memcpy(frames[idx].mrv_state, MRV, sizeof(MRV));
        started[idx] = true;
      }

      bool went_deeper = false;

      while(frames[idx].mrv_state[loc])
      {
        frames[idx].mrv_state[loc]--;
        const auto s = static_cast<uint8_t>(__builtin_ctz(frames[idx].stack_state[loc]));

        const auto SHF = static_cast<uint16_t>(1U << s);
        const auto NSHF = static_cast<uint16_t>(~SHF);

        frames[idx].stack_state[loc] &= NSHF;

        stack[loc] &= NSHF;

        board[loc] = static_cast<int8_t>(s);

        update_option(loc, SHF);

        if(!check_scan_and_swap(idx + 1))
        {
          board[loc] = -1;

          std::memmove(stack, frames[idx].stack_state, sizeof(stack));
          std::memmove(MRV, frames[idx].mrv_state, sizeof(MRV));

          continue;
        }

        idx++;
        went_deeper = true;
        break;

      } // while(frames[idx].remaining)

      if(went_deeper)
      {
        continue; // handle deeper frame in next iteration
      }

      // no remaining candidates for this depth: backtrack
      started[idx] = false; // reset this frame so if we return later it re-initializes

      // if we are at root and exhausted choices -> failure
      if(idx == 0)
      {
        return false;
      }

      idx--;

      board[loc] = -1;

      std::memmove(stack, frames[idx].stack_state, sizeof(stack));
      std::memmove(MRV, frames[idx].mrv_state, sizeof(MRV));

    } // while (true)
  }

  [[nodiscard]] bool check_scan_and_swap(uint8_t beg)
  {
    auto best_it = beg;
    auto min_mvr = UINT16_MAX;

    for(auto i = beg; i < BOARD_SIZE; ++i)
    {
      auto val = *MRV_view[i];

      if(val == 0)
      {
        return false; // dead end
      }
      else if(val < min_mvr)
      {
        min_mvr = val;
        best_it = i;
      }
    }

    if(best_it != beg)
    {
      auto *tmp = MRV_view[best_it];
      MRV_view[best_it] = MRV_view[beg];
      MRV_view[beg] = tmp;
    }

    return true;
  }

  void update_option(const uint8_t i, const uint16_t SHF)
  {
    for(const auto &j : pre_computed.PEERS[i])
    {
      if(stack[j] & SHF)
      {
        stack[j] &= ~SHF;
        MRV[j] = __builtin_popcount(stack[j]);
      }
    }
  }
};
*/
