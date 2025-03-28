#include <algorithm>
#include <array>
#include <bits/floatn-common.h>
#include <cassert>
#include <chrono>
#include <climits>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <iterator>
#include <vector>

// ANIMAL style brute force
/*
class Solution
{
  public:
    void
    solveSudoku (std::vector<std::vector<char> > &board)
    {
        using coordinates = std::pair<int, int>;
        using char_valid = std::unordered_map<char, bool>;

        std::vector<std::vector<char> > board_cpy = board;
        std::unordered_map<coordinates, char_valid, pair_hash> usable;
        std::vector<char> posibilities
            = { '1', '2', '3', '4', '5', '6', '7', '8', '9' };

        int size = board.size ();
        std::vector<std::string> buff3x3
            = { "", "", "", "", "", "", "", " ", "" };
        std::vector<std::string> buffx
            = { "", "", "", "", "", "", "", " ", "" };
        std::vector<std::string> buffy
            = { "", "", "", "", "", "", "", " ", "" };
        for (int i = 0; i < size; i++)
            {
                for (int j = 0; j < size; j++)
                    {
                        if (std::isdigit (board[i][j]))
                            {
                                buffx[i].push_back (board[i][j]);
                            }
                        if (std::isdigit (board[i][j]))
                            {
                                buffy[j].push_back (board[i][j]);
                            }
                    }
            }
        for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                    {
                        int loc = i * 3 + j;
                        for (int x = i * 3; x < (i * 3 + 3); x++)
                            {
                                for (int y = j * 3; y < (j * 3 + 3); y++)
                                    {
                                        if (std::isdigit (board[x][y]))
                                            buff3x3[loc].push_back (
                                                board[x][y]);
                                    }
                            }
                    }
            }
        for (int i = 0; i < size; i++)
            {
                for (int j = 0; j < size; j++)
                    {
                        if (board[i][j] == '.')
                            {
                                for (auto &chr : buffx[i])
                                    {
                                        usable[coordinates (i, j)][chr]
                                            = false;
                                    }
                                for (auto &chr : buffy[j])
                                    {
                                        usable[coordinates (i, j)][chr]
                                            = false;
                                    }
                            }
                    }
            }

        for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                    {
                        int loc = i * 3 + j;
                        for (int x = i * 3; x < (i * 3 + 3); x++)
                            {
                                for (int y = j * 3; y < (j * 3 + 3); y++)
                                    {
                                        if (board[x][y] == '.')
                                            {
                                                for (auto &chr : buff3x3[loc])
                                                    {
                                                        usable[coordinates (
                                                            x, y)][chr]
                                                            = false;
                                                    }
                                            }
                                    }
                            }
                    }
            }

        for (int i = 0; i < size; i++)
            {
                for (int j = 0; j < size; j++)
                    {
                        if (board[i][j] == '.')
                            {
                                for (auto &chr : posibilities)
                                    {
                                        if (usable[coordinates (i, j)].count (
                                                chr)
                                            == 0)
                                            {
                                                usable[coordinates (i, j)][chr]
                                                    = true;
                                            }
                                    }
                            }
                    }
            }

        std::vector<std::vector<int> > constraints[9];
        std::vector<std::vector<int> > permus[9];
        bool done[9] = { false, false, false, false, false,
                         false, false, false, false };
        bool non_1s = true;
        while (non_1s)
            {

                non_1s = false;

                for (int i = 0; i < 9; i++)
                    {
                        if (done[i])
                            continue;

                        constraints[i].clear ();
                    }

                std::vector<std::vector<int> > x_arrs;
                for (int i = 0; i < 9; i++)
                    {
                        x_arrs.push_back ({});
                        if (done[i])
                            continue;
                        for (int j = 0; j < 9; j++)
                            {
                                if (board[i][j] == '.')
                                    {
                                        constraints[i].push_back ({});
                                        for (const auto &[chr, boo] :
                                             usable[coordinates (i, j)])
                                            {
                                                if (boo)
                                                    {
                                                        if (std::find (
                                                                x_arrs[i]
                                                                    .begin (),
                                                                x_arrs[i]
                                                                    .end (),
                                                                chr)
                                                            == x_arrs[i]
                                                                   .end ())
                                                            x_arrs[i]
                                                                .push_back (
                                                                    chr);
                                                        constraints[i]
                                                            .back ()
                                                            .push_back (chr);
                                                    }
                                            }
                                    }
                            }
                    }

                long long combinatory = 0;
                for (int i = 0; i < 9; i++)
                    {
                        if (done[i])
                            continue;
                        permus[i] = generate_valid_permutations (
                            x_arrs[i], constraints[i]);
                        if (combinatory != 0)
                            combinatory *= permus[i].size ();
                        else
                            combinatory += permus[i].size ();
                    }

                std::cout << "\n\n\n\n NUMBER OF TOTAL PERMUTATIOSN IS : "
                          << combinatory << "\n\n\n\n\n";
                for (int i = 0; i < 9; i++)
                    {
                        if (done[i])
                            continue;
                        if (permus[i].size () == 1)
                            {
                                non_1s = true;
                                done[i] = true;
                                int x = 0;
                                for (int j = 0; j < size; j++)
                                    {
                                        if (board[i][j] == '.')
                                            {
                                                usable[coordinates (j, i)]
                                                      [permus[i][0][x]]
                                                    = false;
                                                x++;
                                            }
                                    }
                            }
                    }
            }

        std::vector<int> perm_lock (9, 0);

        std::vector<int> permu_sizes;
        for (int i = 0; i < 9; i++)
            {
                permu_sizes.push_back (permus[i].size ());
            }

        bool valid = isvalidsudoku (board_cpy);
        int fixed = 0;
        while (!valid)
            {
                for (int i = 0; i < size; i++)
                    {
                        int x = 0;
                        for (int j = 0; j < size; j++)
                            {
                                if (board[i][j] == '.')
                                    {
                                        board_cpy[i][j]
                                            = permus[i][perm_lock[i]][x];
                                        x++;
                                    }
                            }
                    }
                bool inc = increment_perm_lock (perm_lock, permu_sizes);

                if (!inc)
                    break;
                valid = isvalidsudoku (board_cpy);
            }

        board = board_cpy;
    }

  private:
    bool
    increment_perm_lock (std::vector<int> &perm_lock,
                         const std::vector<int> &sizes)
    {
        int n = perm_lock.size ();
        for (int pos = n - 1; pos >= 0; pos--)
            {
                if (perm_lock[pos] < sizes[pos] - 1)
                    {
                        perm_lock[pos]++;

                        for (int j = pos + 1; j < n; j++)
                            {
                                perm_lock[j] = 0;
                            }
                        return true;
                    }
            }
        return false;
    }
    bool
    is_valid_for_position (int value, int pos,
                           const std::vector<std::vector<int> > &constraints)
    {
        if (pos < constraints.size () && !constraints[pos].empty ())
            {
                for (int allowed_val : constraints[pos])
                    {
                        if (allowed_val == value)
                            return true;
                    }
                return false;
            }
        return true;
    }

    void
    backtrack (int pos, const std::vector<int> &array,
               const std::vector<std::vector<int> > &constraints,
               std::vector<bool> &used, std::vector<int> &current_permutation,
               std::vector<std::vector<int> > &valid_permutations)
    {
        if (pos == array.size ())
            {
                valid_permutations.push_back (current_permutation);
                return;
            }

        for (std::size_t i = 0; i < array.size (); i++)
            {
                if (!used[i])
                    {
                        int candidate = array[i];
                        if (is_valid_for_position (candidate, pos,
                                                   constraints))
                            {
                                used[i] = true;
                                current_permutation.push_back (candidate);
                                backtrack (pos + 1, array, constraints, used,
                                           current_permutation,
                                           valid_permutations);
                                current_permutation.pop_back ();
                                used[i] = false;
                            }
                    }
            }
    }

    std::vector<std::vector<int> >
    generate_valid_permutations (
        const std::vector<int> &array,
        const std::vector<std::vector<int> > &constraints)
    {
        std::vector<std::vector<int> > valid_permutations;
        std::vector<int> current_permutation;
        std::vector<bool> used (array.size (), false);
        backtrack (0, array, constraints, used, current_permutation,
                   valid_permutations);
        return valid_permutations;
    }

  private:
    struct pair_hash
    {
        template <class t1, class t2>
        std::size_t
        operator() (const std::pair<t1, t2> &p) const
        {
            return std::hash<t1>{}(p.first) ^ (std::hash<t2>{}(p.second) << 1);
        }
    };

  private:
    bool
    isvalidsudoku (std::vector<std::vector<char> > &board)
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
                        else
                            return false;

                        if (buffy.find (board[j][i]) != std::string::npos)
                            return false;
                        else if (std::isdigit (board[j][i]))
                            buffy.push_back (board[j][i]);
                        else
                            return false;
                    }
            }
        // no rows or columns with repeating numbers now on to the 3x3
        // boards

        for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                    {
                        std::string buff;
                        for (int x = i * 3; x < (i * 3 + 3); x++)
                            {
                                for (int y = j * 3; y < (j * 3 + 3); y++)
                                    {
                                        if (buff.find (board[x][y])
                                            != std::string::npos)
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
*/
/*
class Solution
{
  public:
    bool
    isValid (std::vector<std::vector<char> > &board, int row, int col,
             char num)
    {
        // Check row & column
        for (int i = 0; i < 9; i++)
            {
                if (board[row][i] == num || board[i][col] == num)
                    return false;
            }

        // Check 3x3 box
        int startRow = (row / 3) * 3, startCol = (col / 3) * 3;
        for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                    {
                        if (board[startRow + i][startCol + j] == num)
                            return false;
                    }
            }
        return true;
    }

    bool
    solve (std::vector<std::vector<char> > &board, int row, int col)
    {
        if (row == 9)
            return true; // Reached end

        if (board[row][col] != '.') // Skip pre-filled cells
            return solve (board, col == 8 ? row + 1 : row, (col + 1) % 9);

        for (char num = '1'; num <= '9'; num++)
            {
                if (isValid (board, row, col, num))
                    {
                        board[row][col] = num;
                        if (solve (board, col == 8 ? row + 1 : row,
                                   (col + 1) % 9))
                            return true;
                        board[row][col] = '.'; // Backtrack
                    }
            }
        return false;
    }

    void
    solveSudoku (std::vector<std::vector<char> > &board)
    {
        solve (board, 0, 0);
    }
};
*/

// CHAT GPTS SOLUTION AMAZING STUFF TBH
// Optimized Sudoku Solver using bitmask, MRV, and incremental propagation.
/*
class Solution
{
  public:
    // Public function to solve the board in-place.
    // The board is a 9x9 grid with digits '1'-'9' and '.' for empty cells.
    void
    solveSudoku (std::vector<std::vector<char> > &board)
    {
        // Initialize bitmasks for rows, columns, and blocks.
        rows.assign (9, 0);
        cols.assign (9, 0);
        blocks.assign (9, 0);
        empties.clear ();

        // Fill in the bitmasks and record positions of empty cells.
        for (int i = 0; i < 9; i++)
            {
                for (int j = 0; j < 9; j++)
                    {
                        if (board[i][j] == '.')
                            {
                                empties.push_back ({ i, j });
                            }
                        else
                            {
                                int d
                                    = board[i][j]
                                      - '1'; // convert char to 0-indexed digit
                                int mask = 1 << d;
                                rows[i] |= mask;
                                cols[j] |= mask;
                                blocks[blockIndex (i, j)] |= mask;
                            }
                    }
            }

        // Start the backtracking recursion.
        solve (board, 0);
    }

  private:
    std::vector<int> rows, cols,
        blocks; // Bitmasks for each row, column, block.
    std::vector<std::pair<int, int> >
        empties; // List of positions for empty cells.

    // Compute block index for cell (i, j).
    int
    blockIndex (int i, int j)
    {
        return (i / 3) * 3 + (j / 3);
    }

    // Recursive backtracking solver.
    bool
    solve (std::vector<std::vector<char> > &board, int pos)
    {
        // If we've filled all empty cells, we're done.
        if (pos == empties.size ())
            return true;

        // Apply the MRV heuristic: choose the empty cell with the fewest
        // candidates.
        int minPos = pos;
        int minCandidates = 10; // More than the maximum possible (9).
        for (int k = pos; k < empties.size (); k++)
            {
                int i = empties[k].first;
                int j = empties[k].second;
                // Allowed candidates are those not already in the row, col, or
                // block.
                int allowed
                    = ~(rows[i] | cols[j] | blocks[blockIndex (i, j)]) & 0x1FF;
                int count = __builtin_popcount (
                    allowed); // Count number of bits set.
                if (count < minCandidates)
                    {
                        minCandidates = count;
                        minPos = k;
                        if (count == 1)
                            break; // Best-case scenario.
                    }
            }
        std::swap (empties[pos], empties[minPos]);

        int i = empties[pos].first;
        int j = empties[pos].second;
        int blk = blockIndex (i, j);
        int allowed = ~(rows[i] | cols[j] | blocks[blk]) & 0x1FF;

        // Try each candidate digit.
        while (allowed)
            {
                int pick = allowed & -allowed; // Get least significant bit.
                allowed -= pick; // Remove candidate from the allowed mask.
                int digit = __builtin_ctz (
                    pick); // Convert bitmask to digit (0-indexed).

                board[i][j] = '1' + digit;
                // Update the bitmasks.
                rows[i] |= pick;
                cols[j] |= pick;
                blocks[blk] |= pick;

                // Recurse for the next empty cell.
                if (solve (board, pos + 1))
                    return true;

                // Backtrack: undo changes.
                rows[i] ^= pick;
                cols[j] ^= pick;
                blocks[blk] ^= pick;
                board[i][j] = '.';
            }

        // No candidate worked, so backtrack.
        std::swap (empties[pos], empties[minPos]); // (Optional) Restore order.
        return false;
    }
};
*/

/*
class Solution
{
 public:
   void
   solveSudoku (std::vector<std::vector<char> > &board)
   {
      unsigned char options[9][9][9];
      char SEL = 0;

      memset (&options[0][0][0], 1, sizeof (unsigned char) * 729);

      unsigned char i, j, x, y;
      static const unsigned char ID[9] = { 0, 0, 0, 3, 3, 3, 6, 6, 6 };

      for (j = 0; j < 9; j++)
         {
            for (i = 0; i < 9; i++)
               {
                  if (board[j][i] != '.')
                     {
                        SEL = (board[j][i] - '1');
                        for (x = 0; x < 9; x++)
                           {
                              if (x == SEL)
                                 options[j][i][SEL] = 1;
                              else
                                 options[j][i][x] = 0;
                           }
                     }
                  else
                     {
                        for (x = 0; x < 9; x++)
                           {
                              if (x != i && board[j][x] != '.')
                                 {
                                    SEL = (board[j][x] - '1');
                                    options[j][i][SEL] = 0;
                                 }
                              if (x != j && board[x][i] != '.')
                                 {
                                    SEL = (board[x][i] - '1');
                                    options[j][i][SEL] = 0;
                                 }
                              // more pretty debuging
                           }
                        for (y = ID[j]; y < ID[j] + 3; y++)
                           {
                              for (x = ID[i]; x < ID[i] + 3; x++)
                                 {
                                    if (board[y][x] == '.')
                                       continue;
                                    SEL = (board[y][x] - '1');
                                    options[j][i][SEL] = 0;
                                 }
                           }
                     }
               }
         }

      backtrack (board, options, 0, 0);
   }

 private:
   bool
   backtrack (std::vector<std::vector<char> > &board,
              unsigned char (&options)[9][9][9], unsigned char p,
              unsigned char q)
   {
      if (p >= 9)
         return true; // Solved
      if (q >= 9)
         return backtrack (board, options, p + 1, 0);

      if (board[p][q] != '.')
         return backtrack (board, options, p, q + 1);

      for (unsigned char r = 0; r < 9; r++)
         {
            if (!options[p][q][r])
               continue;
            board[p][q] = '1' + r;

            if (validate (board, p, q))
               {
                  if (backtrack (board, options, p, q + 1))
                     {
                        return true;
                     }
               }
            board[p][q] = '.'; // Backtrack
         }
      return false;
   }

   bool
   validate (std::vector<std::vector<char> > &board, const unsigned char &p,
             const unsigned char &q)
   {
      char num = board[p][q];
      for (unsigned char i = 0; i < 9; i++)
         {
            if (i != q && board[p][i] == num)
               return false;
            if (i != p && board[i][q] == num)
               return false;
         }

      unsigned char startRow = p / 3 * 3;
      unsigned char startCol = q / 3 * 3;

      for (unsigned char y = startRow; y < startRow + 3; y++)
         {
            for (unsigned char x = startCol; x < startCol + 3; x++)
               {
                  if ((x != q || y != p) && board[y][x] == num)
                     return false;
               }
         }
      return true;
   }
};
*/

// ONLY WORKING ONE
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
/*
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
*/

// ONLY WORKING ONE EDITED 4 EFFICIENCY
// *wasnt that much more efficient*
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

                        [288780 ns,  298839 ns]

                        [288    μs,  298    μs]

 ************************************************/

/*
class Solution
{

 private:
   unsigned char i, j, x, y;
   static constexpr const unsigned char ID[9] = { 0, 0, 0, 3, 3, 3, 6, 6, 6 };
   static constexpr const unsigned char LI[9] = { 3, 3, 3, 6, 6, 6, 9, 9, 9 };

 private:
   unsigned short MASK = 0b0000000111111111, SEL;

 private:
   unsigned short options[81];
   char board[81];
   const unsigned short *op = &options[0];
   const char *bo = &board[0];
   // i << 3 ( j*8) + j + i === (j * 9 + i)
#define IDX(j, i) ((j << 3) + j + i)

 public:
   void
   solveSudoku (std::vector<std::vector<char> > &sudoku)
   {
      i = 0;
      for (y = 0; y < 9; y++)
         {
            for (x = 0; x < 9; x++)
               {
                  board[i] = sudoku[y][x];
                  i++;
               }
         }

      std::fill (&options[0], &options[0] + 81, MASK);

      for (j = 0; j < 9; j++)
         {
            for (i = 0; i < 9; i++)
               {
                  if (board[IDX (j, i)] != '.')
                     {
                        SEL = (board[IDX (j, i)] - '1');
                        options[IDX (j, i)] &= (1 << SEL);
                     }
                  else
                     {
                        for (x = 0; x < 9; x++)
                           {
                              if (x != i && board[IDX (j, x)] != '.')
                                 {
                                    SEL = (board[IDX (j, x)] - '1');
                                    options[IDX (j, i)] &= ~(1 << SEL);
                                 }
                              if (x != j && board[IDX (x, i)] != '.')
                                 {
                                    SEL = (board[IDX (x, i)] - '1');
                                    options[IDX (j, i)] &= ~(1 << SEL);
                                 }
                           }
                        for (y = ID[j]; y < LI[j]; y++)
                           {
                              for (x = ID[i]; x < LI[i]; x++)
                                 {
                                    if (board[IDX (y, x)] == '.')
                                       continue;

                                    SEL = (board[IDX (y, x)] - '1');
                                    options[IDX (j, i)] &= ~(1 << SEL);
                                 }
                           }
                     }
               }
         }
      backtrack (0, 0);
      for (j = 0; j < 9; j++)
         {
            for (i = 0; i < 9; i++)
               {
                  sudoku[j][i] = board[IDX (j, i)];
               }
         }
   }

 private:
   bool
   backtrack (unsigned char p, unsigned char q)
   {

      if (p >= 9)
         return true; // done
      if (q >= 9)
         return backtrack (p + 1, 0);
      if (board[IDX (p, q)] != '.')
         return backtrack (p, q + 1);

      for (unsigned char r = 0; r < 9; r++)
         {
            if (!(options[IDX (p, q)] & (1 << r)))
               continue;
            board[IDX (p, q)] = '1' + r;

            if (validate (p, q))
               {

                  if (backtrack (p, q + 1))
                     {
                        return true;
                     }
               }
            board[IDX (p, q)] = '.';
         }
      return false;
   }

   inline bool
   validate (const unsigned char p, const unsigned char q)
   {

      for (unsigned char i = 0; i < 9; i++)
         {
            if (i != q && board[IDX (p, i)] == board[IDX (p, q)])
               return false;
         }
      for (unsigned char j = 0; j < p; j++)
         {
            if (j != p && board[IDX (j, q)] == board[IDX (p, q)])
               return false;
         }
      for (unsigned char y = ID[p]; y < LI[p]; y++)
         {
            for (unsigned char x = ID[q]; x < LI[q]; x++)
               {
                  if (y == p && x == q)
                     continue;
                  if (board[IDX (y, x)] == board[IDX (p, q)])
                     {
                        return false;
                     }
               }
         }
      return true;
   }
};
*/

// I have a function recursion dream
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

                        [215657 ns, 233043 ns]

                        [215    μs, 232    μs]

 ************************************************/

/*
class Solution
{
 private:
   int i, x, y, p, q;
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
   unsigned short MASK = 0b0000000111111111;

 private:
   unsigned short SEL;

 private:
   unsigned short options[81];
   char board[81];

 public:
   void
   solveSudoku (std::vector<std::vector<char> > &sudoku)
   {
      i = 0;
      for (y = 0; y < 9; y++)
         {
            for (x = 0; x < 9; x++)
               {
                  board[i] = sudoku[y][x];
                  i++;
               }
         }

      std::fill (&options[0], &options[0] + 81, MASK);

      for (i = 0; i < 81; i++)
         {
            if (board[i] != '.')
               {
                  SEL = board[i] - '1';
                  options[i] &= (1 << SEL);
               }
            else
               {
                  for (x = IDX[i]; x < IDX[i] + 9; x++)
                     {
                        if (x != i && board[x] != '.')
                           {
                              SEL = board[x] - '1';
                              options[i] &= ~(1 << SEL);
                           }
                     }

                  for (y = i - IDX[i]; y < 81; y += 9)
                     {
                        if (y != i && board[y] != '.')
                           {
                              SEL = board[y] - '1';
                              options[i] &= ~(1 << SEL);
                           }
                     }

                  for (y = IDG[i]; y < IDG[i] + 27; y += 9)
                     {
                        for (x = y; x < y + 3; x++)
                           {
                              if (x != i && board[x] != '.')
                                 {
                                    SEL = board[x] - '1';
                                    options[i] &= ~(1 << SEL);
                                 }
                           }
                     }
               }
         }

      backtrack (0);

      i = 0;
      for (y = 0; y < 9; y++)
         {
            for (x = 0; x < 9; x++)
               {
                  sudoku[y][x] = board[i];
                  i++;
               }
         }

      return;
   }

 private:
   bool
   backtrack (unsigned char idx)
   {
      if (idx >= 81)
         return true;

      if (board[idx] != '.')
         return backtrack (idx + 1);

      for (unsigned char r = 0; r < 9; r++)
         {
            if (!(options[idx] & (1 << r)))
               continue;

            board[idx] = '1' + r;

            if (validate (idx))
               {
                  if (backtrack (idx + 1))
                     return true;
               }

            board[idx] = '.';
         }

      return false;
   }
   inline bool
   validate (unsigned char &p)
   {
      for (i = IDX[p]; i < p; i++)
         {
            if (board[i] == board[p])
               return false;
         }

      for (i = p - IDX[p]; i < p; i += 9)
         {
            if (board[i] == board[p])
               return false;
         }

      for (y = IDG[p]; y < IDG[p] + 27; y += 9)
         {
            for (x = y; x < y + 3; x++)
               {
                  if (x >= p)
                     break;
                  if (board[x] == board[p])
                     return false;
               }
         }

      return true;
   }
};
*/

// I have a loop recursion dream
// This was accepted but took 87 ms
// HOW IS THAT EVEN POSSIBLE IDK
// here in my machine the cumulative sum of 13 testcases
// its about ~12 ms
// WTF
// ??????????????????????
// ??????????????????????
// ??????????????????????
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

/*
class Solution
{
 private: // constant ID's definiton
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

 private: // constant definiton
   constexpr const static unsigned short IS_CONST = 0b1000000000000000;

 private: // data definiton
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
      int i, x, y, p;
      bool v = true;
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
                  // SHOULDN'T be necesary since it will never be acessed
                  // but when i remove it breaks lol
                  board[i].stack = board[i].digit - '1';

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

                  v = true;
                  for (i = IDX[p]; i < p; i++)
                     {
                        if (board[i].digit == board[p].digit)
                           {
                              v = false;
                              break;
                           }
                     }

                  if (v)
                     {
                        for (i = p - IDX[p]; i < p; i += 9)
                           {
                              if (board[i].digit == board[p].digit)
                                 {
                                    v = false;
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
                                          v = false;
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
                  while (p != 0 && board[p].option & IS_CONST)
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
*/

//////////////////////////////////////////////////
///// FULLY FUNCTIONAL TESTED AND EVERYTHING /////
//////////////////////////////////////////////////
// By eliminating the cells that have a propositional
// table of size 1 (only 1 truth value) we reduce the
// number of permutations by n! (where n is the number)
// (of cells that have 1 truth value only)
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

/*
class Solution
{
 private: // constant ID's definiton
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

 private: // data definiton
   struct data
   {
      uint16_t option = 0b0000000111111111;
      uint16_t stack;
      int8_t digit;
   };

 public:
   void
   solveSudoku (std::vector<std::vector<char> > &sudoku)
   {
      int32_t i, x, y, p;
      int8_t s;
      data board[81];
      long prevhash = 0, hash = 0;

      i = 0;
      for (y = 0; y < 9; y++)
         {
            for (x = 0; x < 9; x++)
               {
                  board[i].digit = sudoku[y][x] - '1';
                  i++;
               }
         }

      for (i = 0; i < 81; i++)
         {
            if (board[i].digit >= 0)
               {
                  // SHOULDN'T be necesary since it will never be acessed
                  // but when i remove it breaks lol
                  board[i].option &= (1 << board[i].digit);
                  hash += __builtin_popcount (board[i].option);
               }
            else
               {
                  for (x = IDX[i]; x < IDX[i] + 9; x++)
                     {
                        if (x != i && board[x].digit >= 0)
                           {
                              board[i].option &= ~(1 << board[x].digit);
                           }
                     }

                  for (y = i - IDX[i]; y < 81; y += 9)
                     {
                        if (y != i && board[y].digit >= 0)
                           {
                              board[i].option &= ~(1 << board[y].digit);
                           }
                     }

                  for (y = IDG[i]; y < IDG[i] + 27; y += 9)
                     {
                        for (x = y; x < y + 3; x++)
                           {
                              if (x != i && board[x].digit >= 0)
                                 {
                                    board[i].option &= ~(1 << board[x].digit);
                                 }
                           }
                     }
                  hash += __builtin_popcount (board[i].option);
               }
         }

      while (prevhash != hash)
         {
            prevhash = hash;
            hash = 0;
            for (i = 0; i < 81; i++)
               {
                  y = __builtin_popcount (board[i].option);
                  /// if our options are reduced to 1 then we make it constant
                  /// since there is nothing to try there
                  if (y == 1)
                     {
                        // select our selection
                        p = __builtin_ctz (board[i].option);

                        // turn it off on row
                        for (x = IDX[i]; x < IDX[i] + 9; x++)
                           {
                              board[x].option &= ~(1 << p);
                           }

                        // turn it off on column
                        for (y = i - IDX[i]; y < 81; y += 9)
                           {
                              board[y].option &= ~(1 << p);
                           }

                        // turn off in 3x3
                        for (y = IDG[i]; y < IDG[i] + 27; y += 9)
                           {
                              for (x = y; x < y + 3; x++)
                                 {
                                    board[x].option &= ~(1 << p);
                                 }
                           }
                        // assert that its our only option

                        board[i].digit = p;
                        board[i].option = 1 << p;
                     }
                  hash += __builtin_popcount (board[i].option);
               }
         }
      for (i = 0; i < 81; i++)
         {
            board[i].stack = board[i].option;
         }

      backtrack (0, board, s);

      i = 0;
      for (y = 0; y < 9; y++)
         {
            for (x = 0; x < 9; x++)
               {
                  sudoku[y][x] = board[i].digit + '1';
                  i++;
               }
         }

      return;
   }

 private:
   bool
   backtrack (uint8_t idx, data (&board)[81], int8_t &s)
   {
      if (idx >= 81)
         return true;

      if (board[idx].digit >= 0)
         return backtrack (idx + 1, board, s);

      while (__builtin_popcount (board[idx].stack))
         {
            s = __builtin_ctz (board[idx].stack);
            board[idx].stack &= ~(1 << s);
            if (validate (idx, board, s))
               {
                  board[idx].digit = s;
                  if (backtrack (idx + 1, board, s))
                     return true;
               }
         }
      // failed all options backtrack
      board[idx].digit = -2;
      // restore the stack
      board[idx].stack = board[idx].option;

      return false;
   }
   inline bool
   validate (uint8_t &p, data (&board)[81], const uint8_t &s)
   {
      uint8_t i, x, y;
      for (i = IDX[p]; i < p; i++)
         {
            if (board[i].digit == s)
               return false;
         }

      for (i = p - IDX[p]; i < p; i += 9)
         {
            if (board[i].digit == s)
               return false;
         }

      for (y = IDG[p]; y < IDG[p] + 27; y += 9)
         {
            for (x = y; x < y + 3; x++)
               {
                  if (x >= p)
                     break;
                  if (board[x].digit == s)
                     return false;
               }
         }

      return true;
   }
};
*/

/////////////////////////////////////////////////
//
//    3 MS SOLUTION HELL YEAH!!!!!!!!!!!
//    beats 98.89%, in memory beats 28.98%
//
//////////////////////////////////////////////////
///
/// ATTEMPTING MVR
///
/// this sucks on empty boards, but 1 possible
/// solution ones it EXCELLS
///
// By eliminating the cells that have a propositional
// table of size 1 (only 1 truth value) we reduce the
// number of permutations by n! (where n is the number)
// (of cells that have 1 truth value only)
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

                        [59341 ns, 69497  ns]

                        [58    μs, 69     μs]

 ************************************************/

/*
class Solution
{
 private: // constant ID's definiton
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

 private: // data definiton
   struct data
   {
      uint16_t option;
      uint16_t stack;
      int8_t MVR;
      uint8_t LOC;

      bool
      operator< (const data &other) const
      {
         return MVR < other.MVR; // Ascending order
      }
   };
   uint8_t i, x, y, end;
   int8_t s, p;
   std::array<int8_t, 81> board;
   std::array<data, 81> stack;

 public:
   void
   solveSudoku (std::vector<std::vector<char> > &sudoku)
   {
      long prevhash = 0, hash = 0;

      i = 0;
      for (y = 0; y < 9; y++)
         {
            for (x = 0; x < 9; x++)
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
                  // SHOULDN'T be necesary since it will never be acessed
                  // but when i remove it breaks lol
                  stack[i].option &= (1 << board[i]);
                  stack[i].LOC = i;
                  stack[i].MVR = __builtin_popcount (stack[i].option);
                  hash += stack[i].MVR;
               }
            else
               {
                  for (x = IDX[i]; x < IDX[i] + 9; x++)
                     {
                        if (x != i && board[x] >= 0)
                           {
                              stack[i].option &= ~(1 << board[x]);
                           }
                     }

                  for (y = i - IDX[i]; y < 81; y += 9)
                     {
                        if (y != i && board[y] >= 0)
                           {
                              stack[i].option &= ~(1 << board[y]);
                           }
                     }

                  for (y = IDG[i]; y < IDG[i] + 27; y += 9)
                     {
                        for (x = y; x < y + 3; x++)
                           {
                              if (x != i && board[x] >= 0)
                                 {
                                    stack[i].option &= ~(1 << board[x]);
                                 }
                           }
                     }
                  stack[i].LOC = i;
                  stack[i].MVR = __builtin_popcount (stack[i].option);
                  hash += stack[i].MVR;
               }
         }

      while (prevhash != hash)
         {
            prevhash = hash;
            hash = 0;
            for (i = 0; i < 81; i++)
               {
                  y = __builtin_popcount (stack[i].option);
                  /// if our options are reduced to 1 then we make it constant
                  /// since there is nothing to try there
                  if (y == 1)
                     {
                        // select our selection
                        p = __builtin_ctz (stack[i].option);

                        update_option (i, p);

                        board[i] = p;
                        stack[i].option = 1 << p;
                     }
                  hash += __builtin_popcount (stack[i].option);
               }
         }

      for (i = 0; i < 81; i++)
         {
            stack[i].stack = stack[i].option;
            stack[i].MVR = __builtin_popcount (stack[i].option);
         }

      for (i = 0; i < 81; i++)
         {
            if (stack[i].MVR == 1)
               {
                  stack[i].MVR = 127;
               }
         }
      std::sort (stack.begin (), stack.end ());
      auto low_bd
          = std::lower_bound (stack.begin (), stack.end (), 127,
                              [] (const data &d, int e) { return d.MVR < e; });

      end = std::distance (stack.begin (), low_bd);

      backtrack_MVR (0);
      // backtrack (0, board, s);

      i = 0;
      for (y = 0; y < 9; y++)
         {
            for (x = 0; x < 9; x++)
               {
                  sudoku[y][x] = board[i] + '1';
                  i++;
               }
         }

      return;
   }

 private:
   inline void
   update_option (const uint8_t &i, const int8_t &p)
   {
      // turn it off on row
      for (x = IDX[i]; x < IDX[i] + 9; x++)
         {
            stack[x].option &= ~(1 << p);
         }

      // turn it off on column
      for (y = i - IDX[i]; y < 81; y += 9)
         {
            stack[y].option &= ~(1 << p);
         }

      // turn off in 3x3
      for (y = IDG[i]; y < IDG[i] + 27; y += 9)
         {
            for (x = y; x < y + 3; x++)
               {
                  stack[x].option &= ~(1 << p);
               }
         }
   }
   // Update backtrack_MVR call to pass 'end'
   bool
   backtrack_MVR (uint8_t idx)
   {
      if (idx >= end)
         return true;

      uint8_t loc = stack[idx].LOC;

      while (__builtin_popcount (stack[idx].stack))
         {
            s = __builtin_ctz (stack[idx].stack);
            stack[idx].stack &= ~(1 << s);

            if (validate (loc, s))
               {
                  board[loc] = s;
                  auto stack_copy = stack;

                  // Pass 'end' to update_stack
                  update_stack (idx, s);

                  // Re-sort remaining cells
                  std::sort (stack.begin () + idx + 1, stack.begin () + end);

                  if (backtrack_MVR (idx + 1))
                     return true;

                  board[loc] = -1;
                  stack = stack_copy;
               }
         }
      return false;
   }

   inline void
   update_stack (const uint8_t &i, const int8_t &p)
   {
      uint8_t loc = stack[i].LOC;

      const uint8_t row_id = IDX[loc];
      const uint8_t col_id = loc - row_id;
      const uint8_t box_id = IDG[loc];

      // Iterate through all cells in the backtracking list
      for (uint8_t idx = 0; idx < end; ++idx)
         {
            uint8_t curr_loc = stack[idx].LOC;

            // Check if current cell shares row, column, or box with loc
            if (IDX[curr_loc] == row_id || (curr_loc - IDX[curr_loc]) == col_id
                || IDG[curr_loc] == box_id)
               {
                  stack[idx].stack &= ~(1 << p);
                  stack[idx].MVR = __builtin_popcount (stack[idx].stack);
               }
         }
   }
   inline bool
   validate (const uint8_t &p, const int8_t &s)
   {
      for (i = IDX[p]; i < IDX[p] + 9; i++)
         {
            if (board[i] == s)
               return false;
         }

      for (i = p - IDX[p]; i < 81; i += 9)
         {
            if (board[i] == s)
               return false;
         }

      for (y = IDG[p]; y < IDG[p] + 27; y += 9)
         {
            for (x = y; x < y + 3; x++)
               {
                  if (x == p)
                     continue;
                  if (board[x] == s)
                     return false;
               }
         }

      return true;
   }
};
*/

//////////////////////////////////////////////////
///
/// ATTEMPTING MVR PART 2 lets beat 3ms
///
/// this sucks on empty boards, but 1 possible
/// solution ones it EXCELLS
///
/// made it 3-9 microseconds faster
///
// By eliminating the cells that have a propositional
// table of size 1 (only 1 truth value) we reduce the
// number of permutations by n! (where n is the number)
// (of cells that have 1 truth value only)
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

                        [53038  ns, 54382  ns]

                        [52    μs, 54     μs]

 ************************************************/

/*
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
   std::array<int8_t, 81> board;
   std::array<data, 81> stack;
   std::array<uint16_t, 9> row_m, col_m, box_m;
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
                  stack[i].MVR = __builtin_popcount (stack[i].option);
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
                  hash += __builtin_popcount (stack[i].option);
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
      backtrack_MVR (0);
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
   bool
   backtrack_MVR (uint8_t idx)
   {
      if (idx >= end)
         return true;
      uint8_t loc = stack[idx].LOC;
      while (__builtin_popcount (stack[idx].stack))
         {
            int8_t s = __builtin_ctz (stack[idx].stack);
            stack[idx].stack &= ~(1 << s);
            if (validate (loc, s))
               {
                  board[loc] = s;
                  turn_on (loc, s);
                  auto stack_copy = stack;
                  update_stack (idx, s, end);
                  std::sort (stack.begin () + idx + 1, stack.begin () + end);
                  if (backtrack_MVR (idx + 1))
                     return true;
                  turn_off (loc, s);
                  board[loc] = -1;
                  stack = stack_copy;
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
      for (uint8_t idx = 0; idx < end; idx++)
         {
            uint8_t curr_loc = stack[idx].LOC;
            if (ROW[curr_loc] == r || COL[curr_loc] == c || BOX[curr_loc] == b)
               {
                  stack[idx].stack &= ~(1 << p);
                  stack[idx].MVR = __builtin_popcount (stack[idx].stack);
               }
         }
   }
};
*/

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

//////////////////////////////////////////////////
///
/// ATTEMPTING MVR PART 4 ILL BE BACK
///
///     GOT A 1ms solution!!!!!!!!
///
///     WILL GET 0 SOON
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

/*
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
*/

//////////////////////////////////////////////////
///
/// ATTEMPTING MVR PART 5 ILL BE BACK
/// MAYBE VECTOR IS FASTER
/// TARGET IS 0 MS
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

/*
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
   std::vector<data> stack;
   std::array<uint16_t, 9> row_m, col_m, box_m;

 private: // end
   uint8_t end;

 public:
   void
   solveSudoku (std::vector<std::vector<char> > &sudoku)
   {
      if (!stack.empty ())
         stack.clear ();
      stack.reserve (81);
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
                  data tmp;
                  tmp.option = 0b0000000111111111;
                  stack.push_back (tmp);
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
                  hash += __builtin_popcount (stack[i].option);
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

      stack.erase (low_bd, stack.end ());
      end = stack.size ();

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

 private: // helper for propositional table buildup
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

 private: // backtracking stuff
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
                  auto stack_copy = stack;
                  // bottleneck

                  update_stack (idx, s, end);
                  std::sort (stack.begin () + idx + 1, stack.end ());
                  if (backtrack_MVR (idx + 1))
                     return true;
                  turn_off (loc, s);
                  board[loc] = -1;
                  stack = stack_copy;
               }
         }
      return false;
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
*/

///////////////////////////////////////////////////////////////////
//////// debuging       //////////////////////////////////////////
///////////////////////////////////////////////////////////////////

/*
std::cout << "\033[1;34m\t1 \t 2\t 3\t 4\t 5\t 6\t 7\t 8\t 9\033[0m"
          << '\n';
for (j = 0; j < 9; j++)
   {
      for (i = 0; i < 9; i++)
         {
            std::cout << "\t@ i = " << IDX (j, i) << " aka [j,i] = ["
                      << j << ',' << i << "] \n"
                      << '\n';
            for (int x = 0; x < 9; x++)
               {
                  if ((options[IDX (j, i)] >> x) & 0b1)
                     {
                        std::cout << "\033[1;32m\tTRUE\033[0m";
                     }
                  else
                     {
                        std::cout << "\033[1;31m\tFALSE\033[0m";
                     }
               }
            std::cout << '\n';
         }
   }
   */
///////////////////////////////////////////////////////////////////
//////// debuging       //////////////////////////////////////////
///////////////////////////////////////////////////////////////////

/*
  std::cout << "\033[1;34m\t1 \t 2\t 3\t 4\t 5\t 6\t 7\t 8\t 9\033[0m"
            << '\n';
  for (j = 0; j < 9; j++)
     {
        for (i = 0; i < 9; i++)
           {
              std::cout << "\t@ i = " << IDX (j, i) << " aka [j,i] = ["
                        << j << ',' << i << "] \n"
                        << '\n';
              for (int x = 0; x < 9; x++)
                 {
                    if ((stack[IDX (j, i)] >> x) & 0b1)
                       {
                          std::cout << "\033[1;32m\tTRUE\033[0m";
                       }
                    else
                       {
                          std::cout << "\033[1;31m\tFALSE\033[0m";
                       }
                 }
              std::cout << '\n';
           }
     }
     */

/*
std::cout << "ENCODED INPUT Sudoku is:\n";
for (int x = 0; x < 81; x++)
   {
      if (x % 9 == 0)
         std::cout << '\n';
      std::cout << " " << ((stack[x] & D_MASK) >> 28) + 1;
   }
std::cout << "\n";
*/

///////////////////////////////////////////////////////////////////
//////// debuging       //////////////////////////////////////////
///////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                      CHAT GPTS ULTRA FAST SOLUTION
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

/*
class Solution
{
 public:
   // Public function to solve the board in-place.
   // The board is a 9x9 grid with digits '1'-'9' and '.' for empty cells.
   void
   solveSudoku (std::vector<std::vector<char> > &board)
   {
      // Initialize bitmasks for rows, columns, and blocks.
      rows.assign (9, 0);
      cols.assign (9, 0);
      blocks.assign (9, 0);
      empties.clear ();

      // Fill in the bitmasks and record positions of empty cells.
      for (int i = 0; i < 9; i++)
         {
            for (int j = 0; j < 9; j++)
               {
                  if (board[i][j] == '.')
                     {
                        empties.push_back ({ i, j });
                     }
                  else
                     {
                        int d = board[i][j]
                                - '1'; // convert char to 0-indexed digit
                        int mask = 1 << d;
                        rows[i] |= mask;
                        cols[j] |= mask;
                        blocks[blockIndex (i, j)] |= mask;
                     }
               }
         }

      // Start the backtracking recursion.
      solve (board, 0);
   }

 private:
   std::vector<int> rows, cols,
       blocks; // Bitmasks for each row, column, block.
   std::vector<std::pair<int, int> >
       empties; // List of positions for empty cells.

   // Compute block index for cell (i, j).
   int
   blockIndex (int i, int j)
   {
      return (i / 3) * 3 + (j / 3);
   }

   // Recursive backtracking solver.
   bool
   solve (std::vector<std::vector<char> > &board, int pos)
   {
      // If we've filled all empty cells, we're done.
      if (pos == empties.size ())
         return true;

      // Apply the MRV heuristic: choose the empty cell with the fewest
      // candidates.
      int minPos = pos;
      int minCandidates = 10; // More than the maximum possible (9).
      for (int k = pos; k < empties.size (); k++)
         {
            int i = empties[k].first;
            int j = empties[k].second;
            // Allowed candidates are those not already in the row, col, or
            // block.
            int allowed
                = ~(rows[i] | cols[j] | blocks[blockIndex (i, j)]) & 0x1FF;
            int count
                = __builtin_popcount (allowed); // Count number of bits set.
            if (count < minCandidates)
               {
                  minCandidates = count;
                  minPos = k;
                  if (count == 1)
                     break; // Best-case scenario.
               }
         }
      std::swap (empties[pos], empties[minPos]);

      int i = empties[pos].first;
      int j = empties[pos].second;
      int blk = blockIndex (i, j);
      int allowed = ~(rows[i] | cols[j] | blocks[blk]) & 0x1FF;

      // Try each candidate digit.
      while (allowed)
         {
            int pick = allowed & -allowed; // Get least significant bit.
            allowed -= pick; // Remove candidate from the allowed mask.
            int digit = __builtin_ctz (
                pick); // Convert bitmask to digit (0-indexed).

            board[i][j] = '1' + digit;
            // Update the bitmasks.
            rows[i] |= pick;
            cols[j] |= pick;
            blocks[blk] |= pick;

            // Recurse for the next empty cell.
            if (solve (board, pos + 1))
               return true;

            // Backtrack: undo changes.
            rows[i] ^= pick;
            cols[j] ^= pick;
            blocks[blk] ^= pick;
            board[i][j] = '.';
         }

      // No candidate worked, so backtrack.
      std::swap (empties[pos], empties[minPos]); // (Optional) Restore
order. return false;
   }
};
*/

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

inline bool
isValidSudokuwithempies (std::vector<std::vector<char> > &board)
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
// be aware edited so that if it finds a point it retuns false;
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

std::vector<std::vector<std::vector<char> > > boards_gtp = { // testcase 0
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

void
test_n_times (size_t board_numb, bool prints)
{

   /////////////////////////////////////////////////////////////////////////
   /////////////////////////////////////////////////////////////////////////
   ///////////                                                     /////////
   ///////////        10k call TEST CASE STARTS                    /////////
   ///////////                                                     /////////
   /////////////////////////////////////////////////////////////////////////
   /////////////////////////////////////////////////////////////////////////
   /////////////////////////////////////////////////////////////////////////

   if (board_numb >= boards_gtp.size ())
      {
         std::cout << "bruh board_numb inside of test_commulative is > than "
                      "vect size";
         return;
      }
   auto board_sel = boards_gtp[board_numb];
   Solution solver;

   if (prints)
      {
         // Print input board
         std::cout << "input Sudoku in test_n_times for board selected is # "
                   << board_numb << " \n";
         for (int x = 0; x < 9; x++)
            {
               for (int y = 0; y < 9; y++)
                  {
                     std::cout << board_sel[x][y] << " ";
                     if (y == 2 || y == 5)
                        std::cout << "  ";
                  }
               std::cout << "\n";
               if (x == 2 || x == 5)
                  std::cout << "\n";
            }
         std::cout << '\n';
      }

   constexpr int iterations = 10000;
   auto total_duration_ns = std::chrono::nanoseconds (0);
   auto total_duration_micro = std::chrono::microseconds (0);

   for (int i = 0; i < iterations; ++i)
      {
         auto board = board_sel;
         auto start = std::chrono::high_resolution_clock::now ();
         solver.solveSudoku (board);
         auto stop = std::chrono::high_resolution_clock::now ();
         total_duration_ns += stop - start;
         total_duration_micro
             += std::chrono::duration_cast<std::chrono::microseconds> (
                 stop - start);
      }
   // outside loop so we can print it
   solver.solveSudoku (board_sel);

   auto average_duration_ns = total_duration_ns / iterations;
   auto average_duration_micro = total_duration_micro / iterations;
   auto average_duration_milli = total_duration_micro / iterations;
   std::cout << "Average Execution Time of the " << iterations
             << " iterations is : \n"
             << "\t "
             << std::chrono::duration_cast<std::chrono::nanoseconds> (
                    average_duration_ns)
                    .count ()
             << " ns\n"
             << "\t "
             << std::chrono::duration_cast<std::chrono::microseconds> (
                    average_duration_micro)
                    .count ()
             << " μs\n";
   std::cout << "\033[1;32m"
             << "\t"
             << std::chrono::duration_cast<std::chrono::milliseconds> (
                    average_duration_milli)
                    .count ()
             << " ms\n"
             << "\033[0m";

   if (prints)
      {
         // print output board
         std::cout << "OUTPUT Sudoku in test_n_times for board selected is # "
                   << board_numb << " \n";
         for (int x = 0; x < 9; x++)
            {
               for (int y = 0; y < 9; y++)
                  {
                     std::cout << board_sel[x][y] << " ";
                     if (y == 2 || y == 5)
                        std::cout << "  ";
                  }
               std::cout << "\n";
               if (x == 2 || x == 5)
                  std::cout << "\n";
            }
         std::cout << '\n';
      }

   // validate the outpout for correctness
   if (isValidSudoku (board_sel))
      {
         std::cout << "\033[1;32m PROPERLY SOLVED THE boards_gtp["
                   << board_numb
                   << "]\n"
                      "\033[0m";
      }
   else
      {
         std::cout << "\033[1;31m DIDN'T SOLVE THE boards_gtp[" << board_numb
                   << "] \n \033[0m";
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

void
test_commulative (bool prints)
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

   auto total_duration = std::chrono::microseconds (0);

   for (size_t i = 0; i < boards_gtp.size (); i++)
      {
         if (prints)
            {
               // Print input board
               std::cout << "input Sudoku # " << i << " \n";
               for (int x = 0; x < 9; x++)
                  {
                     for (int y = 0; y < 9; y++)
                        {
                           std::cout << boards_gtp[i][x][y] << " ";
                           if (y == 2 || y == 5)
                              std::cout << "  ";
                        }
                     std::cout << "\n";
                     if (x == 2 || x == 5)
                        std::cout << "\n";
                  }
               std::cout << '\n';
            }
         if (isValidSudokuwithempies (boards_gtp[i]))
            {
               std::cout << "\033[1;32m INPUT # " << i
                         << " IS VALID \n \033[0m";
            }
         else
            {
               std::cout << "\033[1;31m INPUT # " << i
                         << " ISN'T VALID \n \033[0m";
            }

         auto start = std::chrono::high_resolution_clock::now ();
         solver.solveSudoku (boards_gtp[i]);
         auto stop = std::chrono::high_resolution_clock::now ();
         total_duration
             += std::chrono::duration_cast<std::chrono::microseconds> (
                 stop - start);
         if (prints)
            {
               std::cout << "OUTPUT Sudoku # " << i << " \n";
               for (int x = 0; x < 9; x++)
                  {
                     for (int y = 0; y < 9; y++)
                        {
                           std::cout << boards_gtp[i][x][y] << " ";
                           if (y == 2 || y == 5)
                              std::cout << "  ";
                        }
                     std::cout << "\n";
                     if (x == 2 || x == 5)
                        std::cout << "\n";
                  }
               std::cout << '\n';
            }
         if (isValidSudoku (boards_gtp[i]))
            {
               std::cout << "\033[1;32m PROPERLY SOLVED THE BOARD # " << i
                         << "\033[0m" << '\n';
            }
         else
            {
               std::cout << "\033[1;31m DIDN'T SOLVE THE BOARD # " << i
                         << "\033[0m" << '\n';
            }
         std::cout << '\n';
      }

   std::cout << "TOTAL SUM OF EXECUTION Time of the "
             << boards_gtp.size () + 1 // 1 is random board
             << " GPT provided 1 solution boards is : \n";

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

void
test_individua (size_t test_num, bool prints)
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

   if (test_num >= boards_gtp.size ())
      {
         std::cout << "bruh board_numb inside of test_commulative is > than "
                      "vect size";
         return;
      }
   auto board_sel = boards_gtp[test_num];

   if (prints)
      {
         std::cout << "input Sudoku boards_gtp[" << test_num << "] \n";
         for (int x = 0; x < 9; x++)
            {
               for (int y = 0; y < 9; y++)
                  {
                     std::cout << board_sel[x][y] << " ";
                     if (y == 2 || y == 5)
                        std::cout << "  ";
                  }
               std::cout << "\n";
               if (x == 2 || x == 5)
                  std::cout << "\n";
            }
         std::cout << '\n';
      }

   auto start = std::chrono::high_resolution_clock::now ();
   solver.solveSudoku (board_sel);
   auto stop = std::chrono::high_resolution_clock::now ();

   auto times_ns
       = std::chrono::duration_cast<std::chrono::nanoseconds> (stop - start);
   auto times_mcs
       = std::chrono::duration_cast<std::chrono::microseconds> (stop - start);
   auto times_mils
       = std::chrono::duration_cast<std::chrono::milliseconds> (stop - start);

   if (prints)
      {
         std::cout << "OUTPUT Sudoku boards_gtp[" << test_num << "] \n";
         for (int x = 0; x < 9; x++)
            {
               for (int y = 0; y < 9; y++)
                  {
                     std::cout << board_sel[x][y] << " ";
                     if (y == 2 || y == 5)
                        std::cout << "  ";
                  }
               std::cout << "\n";
               if (x == 2 || x == 5)
                  std::cout << "\n";
            }
         std::cout << "\n";
      }

   std::cout << "\nExecuting solver.solveSudoku() took:\n";
   std::cout << "\t" << times_ns.count () << " ns\n";
   std::cout << "\t" << times_mcs.count () << " μs\n";
   std::cout << "\t" << times_mils.count () << " ms\n";

   if (isValidSudoku (board_sel))
      {
         std::cout << "\033[1;32m PROPERLY SOLVED THE boards_gtp[" << test_num
                   << "]\n"
                      "\033[0m";
      }
   else
      {
         std::cout << "\033[1;31m DIDN'T SOLVE THE boards_gtp[" << test_num
                   << "] \n \033[0m";
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

int
main ()
{
   test_individua (7, true);
   test_n_times (7, false);
   std::cout
       << "\033[1;31m "
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
   test_commulative (false);
   return 0;
};
