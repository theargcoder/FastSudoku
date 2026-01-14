#include <algorithm>
#include <cassert>
#include <climits>
#include <cstdint>
#include <cstring>
#include <vector>

//////////////////////////////////////////////////
///
/// 29 μs is as good as it gets,
/// it depends on the board, some are harder
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

                        [28400 ns , 29999  ns]

                        [28    μs , 29     μs]

 ************************************************/

/*
class Solution
{
private:
  const constexpr static uint8_t BOARD_SIZE = 81;
  const constexpr static uint8_t PEERS_SIZE = 20;
  const constexpr static uint8_t ROW_SIZE = 9;
  const constexpr static uint32_t ALL_BITS_ON = 0b0000'0000'0000'0000'0000'0001'1111'1111;
  const constexpr static uint32_t SHF_ST = 0b0000'0000'0000'0000'0000'1000'0000'0000;

private:
  struct PreComputed
  {
  public:
    mutable uint8_t PEERS[BOARD_SIZE][PEERS_SIZE];

    consteval PreComputed()
    {
      init_peers();
    }

  private:
    // Consteval ensures this function is executed ONLY at compile time
    consteval void init_peers() const
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
  uint32_t stack[BOARD_SIZE];

private: // pointers to stack to suffle arround and sort
  uint32_t *stack_view[BOARD_SIZE];

public:
  void solveSudoku(std::vector<std::vector<char>> &sudoku)
  {
    std::fill(&stack[0], &stack[BOARD_SIZE], ALL_BITS_ON);

    uint32_t i = 0;
    uint32_t ct;
    for(uint32_t y = 0; y < ROW_SIZE; ++y)
    {
      for(uint32_t x = 0; x < ROW_SIZE; ++x)
      {
        ct = sudoku[y][x] - '1';
        if(ct >= 0 && ct < ROW_SIZE)
        {
          const uint32_t SHF = 1U << ct;
          stack[i] = SHF;

          update_option(i, SHF);
        }

        stack_view[i] = &stack[i];
        i++;
      }
    }

    backtrack_MVR(0);

    i = 0;
    for(uint32_t y = 0; y < ROW_SIZE; ++y)
    {
      for(uint32_t x = 0; x < ROW_SIZE; ++x)
      {
        sudoku[y][x] = std::countr_zero(stack[i]) + '1';
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

    const auto loc = static_cast<uint8_t>(stack_view[idx] - &stack[0]);

    uint16_t candidates = stack[loc] & ALL_BITS_ON;
    uint8_t s;
    uint32_t SHF;

    while(candidates)
    {
      s = std::countr_zero(candidates);
      SHF = 1U << s;
      candidates &= ~SHF; // Remove from local loop tracker

      update_option_undo(loc, SHF);

      if(check_scan_and_swap(idx + 1))
      {
        if(backtrack_MVR(idx + 1))
        {
          return true;
        }
      }

      undo_option(loc, SHF);
      stack[loc] &= ALL_BITS_ON;
    }
    return false;
  }

  [[nodiscard]] bool check_scan_and_swap(uint8_t beg)
  {
    auto best_it = beg;
    auto min_mvr = INT_MAX;

    for(auto i = beg; i < BOARD_SIZE; ++i)
    {
      auto val = std::popcount(*stack_view[i] & ALL_BITS_ON);

      if(val == 0) [[unlikely]]
      {
        return false; // dead end
      }

      if(val < min_mvr)
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

  void update_option(const uint8_t i, const uint32_t SHF)
  {
    for(const auto &j : pre_computed.PEERS[i])
    {
      if(stack[j] & SHF)
      {
        stack[j] &= ~SHF;
      }
    }
  }

  void update_option_undo(const uint8_t i, const uint32_t SHF)
  {
    const auto &peers = pre_computed.PEERS[i];
    for(uint32_t j = 0, K = SHF_ST; j < PEERS_SIZE; j++, K <<= 1U)
    {
      if(stack[peers[j]] & SHF)
      {
        stack[peers[j]] &= ~SHF;
        stack[i] |= K;
      }
    }
  }

  void undo_option(const uint8_t i, const uint16_t SHF)
  {
    const auto &peers = pre_computed.PEERS[i];
    for(uint32_t j = 0, K = SHF_ST; j < PEERS_SIZE; j++, K <<= 1U)
    {
      if(stack[i] & K)
      {
        stack[peers[j]] |= SHF;
      }
    }
  }
};
*/

/////////////////////////////////////////////////////////
///// ITERATIVE SOLUTION, NO FUNCTION RECURSION     /////
/////////////////////////////////////////////////////////

class Solution
{
private:
  const constexpr static uint8_t BOARD_SIZE = 81;
  const constexpr static uint8_t PEERS_SIZE = 20;
  const constexpr static uint8_t ROW_SIZE = 9;

  const constexpr static uint32_t ALL_BITS_ON = 0b0000'0000'0000'0000'0000'0001'1111'1111;
  const constexpr static uint32_t SHIFT_START = 0b0000'0000'0000'0000'0000'1000'0000'0000;

  const constexpr static uint16_t ONLY_CANDIDATES = 0b0000'0001'1111'1111;
  const constexpr static uint16_t S_OFFSET = 11;

  struct PreComputedBits
  {
    struct __attribute__((packed)) data
    {
      uint8_t row = 0;
      uint8_t column = 0;
      uint8_t box_peers[4] = { 0, 0, 0, 0 };
    };

    mutable data BitPeers[BOARD_SIZE];

    consteval PreComputedBits()
    {
      init_peers();
    }

  private:
    consteval void init_peers() const
    {
      for(uint16_t i = 0; i < BOARD_SIZE; ++i)
      {
        data this_loc;
        const uint16_t row = i / ROW_SIZE;
        const uint16_t col = i % ROW_SIZE;
        const uint16_t box_row = (row / 3) * 3;
        const uint16_t box_col = (col / 3) * 3;

        this_loc.row = static_cast<uint8_t>(row);
        this_loc.column = static_cast<uint8_t>(col); // <-- FIXED

        const auto pos_row = static_cast<uint8_t>(row - box_row);
        const auto pos_col = static_cast<uint8_t>(col - box_col);

        const uint8_t others[3][2] = { { 1, 2 }, { 0, 2 }, { 0, 1 } };

        uint8_t box_start = static_cast<uint8_t>(row * ROW_SIZE + col - pos_col - (ROW_SIZE * pos_row));

        const uint8_t r0 = others[pos_row][0];
        const uint8_t r1 = others[pos_row][1];
        const uint8_t c0 = others[pos_col][0];
        const uint8_t c1 = others[pos_col][1];

        this_loc.box_peers[0] = static_cast<uint8_t>(box_start + r0 * ROW_SIZE + c0);
        this_loc.box_peers[1] = static_cast<uint8_t>(box_start + r0 * ROW_SIZE + c1);
        this_loc.box_peers[2] = static_cast<uint8_t>(box_start + r1 * ROW_SIZE + c0);
        this_loc.box_peers[3] = static_cast<uint8_t>(box_start + r1 * ROW_SIZE + c1);

        BitPeers[i] = this_loc; // <-- store it
      }
    }
  };

private:
  const PreComputedBits pre_computed_bits{};

private: // custom data container
  uint32_t stack[BOARD_SIZE];

private: // pointers to stack to shuffle around and sort
  uint32_t *stack_view[BOARD_SIZE];

public:
  void solveSudoku(std::vector<std::vector<char>> &sudoku)
  {
    std::fill(&stack[0], &stack[BOARD_SIZE], ALL_BITS_ON);

    uint32_t i = 0;
    uint8_t ct;
    for(uint32_t y = 0; y < ROW_SIZE; ++y)
    {
      for(uint32_t x = 0; x < ROW_SIZE; ++x)
      {
        ct = sudoku[y][x] - '1';
        if(ct >= 0 && ct < ROW_SIZE)
        {
          const uint32_t SHF = 1U << ct;
          stack[i] = SHF;
          update_option(i, SHF);
        }
        stack_view[i] = &stack[i];
        i++;
      }
    }

    backtrack_MVR(0);

    i = 0;
    for(uint32_t y = 0; y < ROW_SIZE; ++y)
    {
      for(uint32_t x = 0; x < ROW_SIZE; ++x)
      {
        sudoku[y][x] = static_cast<char>(std::countr_zero(stack[i]) + '1');
        i++;
      }
    }
  }

private:
  bool backtrack_MVR(uint8_t idx)
  {
    uint16_t candidates[BOARD_SIZE];

    uint8_t loc = static_cast<uint8_t>(stack_view[idx] - &stack[0]);

    candidates[idx] = stack[loc] & ALL_BITS_ON;

    while(true)
    {
      if(idx >= BOARD_SIZE)
      {
        return true;
      }

      loc = static_cast<uint8_t>(stack_view[idx] - &stack[0]);

      // extract candidate-only bits
      uint16_t cand = candidates[idx] & ONLY_CANDIDATES;

      if(cand)
      {
        const uint8_t s = std::countr_zero(cand);
        const uint16_t SHF = static_cast<uint16_t>(1U << s);

        candidates[idx] = (cand ^ SHF) | (static_cast<uint16_t>(s) << S_OFFSET);

        update_option_undo(loc, SHF);

        if(check_scan_and_swap(idx + 1))
        {
          idx++;

          loc = static_cast<uint8_t>(stack_view[idx] - &stack[0]);
          candidates[idx] = stack[loc] & ALL_BITS_ON;

          continue;
        }

        // forward-check failed → undo immediately
        undo_option(loc, 1U << (candidates[idx] >> S_OFFSET));
        stack[loc] &= ALL_BITS_ON;
        continue;
      }

      if(idx == 0)
      {
        return false;
      }

      idx--;
      loc = static_cast<uint8_t>(stack_view[idx] - &stack[0]);

      undo_option(loc, 1U << (candidates[idx] >> S_OFFSET));
      stack[loc] &= ALL_BITS_ON;
    }
  }

  [[nodiscard]] bool check_scan_and_swap(uint8_t beg)
  {
    auto best_it = beg;
    auto min_mvr = INT_MAX;

    for(auto i = beg; i < BOARD_SIZE; ++i)
    {
      // count remaining candidates among low bits
      auto val = std::popcount(*stack_view[i] & ALL_BITS_ON);

      if(val == 0) [[unlikely]]
      {
        return false; // dead end
      }

      if(val < min_mvr)
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

  void update_option(const uint8_t &idx, const uint32_t &SHF)
  {
    const uint8_t &row = pre_computed_bits.BitPeers[idx].row;
    const uint8_t &col = pre_computed_bits.BitPeers[idx].column;
    const auto &indices = pre_computed_bits.BitPeers[idx].box_peers;
    const uint8_t row_start = row * ROW_SIZE;

    const auto cell = static_cast<uint8_t>(row_start + col);

    // row peers (same row, all columns)
    for(uint8_t i = row_start; i < row_start + ROW_SIZE; ++i)
    {
      if(i == cell)
      {
        continue;
      }
      stack[i] ^= SHF;
    }

    // column peers (same column, all rows)
    for(uint8_t i = col; i < BOARD_SIZE; i += ROW_SIZE)
    {
      if(i == cell)
      {
        continue;
      }
      stack[i] ^= SHF;
    }

    for(uint8_t i = 0; i < 4; ++i)
    {
      stack[indices[i]] ^= SHF;
    }
  }

  void update_option_undo(const uint8_t &idx, const uint32_t &SHF)
  {
    const uint8_t &row = pre_computed_bits.BitPeers[idx].row;
    const uint8_t &col = pre_computed_bits.BitPeers[idx].column;
    const auto &indices = pre_computed_bits.BitPeers[idx].box_peers;
    const uint8_t row_start = row * ROW_SIZE;

    const auto cell = static_cast<uint8_t>(row_start + col);

    uint32_t K = SHIFT_START;

    // row peers (same row, all columns)
    for(uint8_t i = row_start; i < row_start + ROW_SIZE; ++i, K <<= 1U)
    {
      if(i == cell)
      {
        K >>= 1U;
        continue;
      }

      if(stack[i] & SHF)
      {
        stack[i] ^= SHF;
        stack[idx] |= K;
      }
    }

    // column peers (same column, all rows)
    for(uint8_t i = col; i < BOARD_SIZE; i += ROW_SIZE, K <<= 1U)
    {
      if(i == cell)
      {
        K >>= 1U;
        continue;
      }

      if(stack[i] & SHF)
      {
        stack[i] ^= SHF;
        stack[idx] |= K;
      }
    }

    for(uint8_t i = 0; i < 4; ++i, K <<= 1U)
    {
      if(stack[indices[i]] & SHF)
      {
        stack[indices[i]] ^= SHF;
        stack[idx] |= K;
      }
    }
  }

  void undo_option(const uint8_t &idx, const uint16_t &SHF)
  {
    const uint8_t &row = pre_computed_bits.BitPeers[idx].row;
    const uint8_t &col = pre_computed_bits.BitPeers[idx].column;
    const auto &indices = pre_computed_bits.BitPeers[idx].box_peers;
    const uint8_t row_start = row * ROW_SIZE;

    const auto cell = static_cast<uint8_t>(row_start + col);

    uint32_t K = SHIFT_START;

    // row peers (same row, all columns)
    for(uint8_t i = row_start; i < row_start + ROW_SIZE; ++i, K <<= 1U)
    {
      if(i == cell)
      {
        K >>= 1U;
        continue;
      }

      if(stack[idx] & K)
      {
        stack[i] |= SHF;
      }
    }

    // column peers (same column, all rows)
    for(uint8_t i = col; i < BOARD_SIZE; i += ROW_SIZE, K <<= 1U)
    {
      if(i == cell)
      {
        K >>= 1U;
        continue;
      }

      if(stack[idx] & K)
      {
        stack[i] |= SHF;
      }
    }

    for(uint8_t i = 0; i < 4; ++i, K <<= 1U)
    {
      if(stack[idx] & K)
      {
        stack[indices[i]] |= SHF;
      }
    }
  }
};
