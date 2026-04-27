/*
 * @lc app=leetcode.cn id=36 lang=cpp
 *
 * [36] 有效的数独
 */

// @lc code=start
class Solution {
public:
    bool isValidSudoku(vector<vector<char>>& board) {
        // Check rows and columns
        for (int i = 0; i < 9; ++i) {
            unordered_set<char> rowSet;
            unordered_set<char> colSet;

            for (int j = 0; j < 9; ++j) {
                // Check row
                if (board[i][j] != '.') {
                    if (rowSet.count(board[i][j])) {
                        return false; // Duplicate in row
                    }
                    rowSet.insert(board[i][j]);
                }

                // Check column
                if (board[j][i] != '.') {
                    if (colSet.count(board[j][i])) {
                        return false; // Duplicate in column
                    }
                    colSet.insert(board[j][i]);
                }
            }
        }

        // Check 3x3 sub-boxes
        for (int boxRow = 0; boxRow < 3; ++boxRow) {
            for (int boxCol = 0; boxCol < 3; ++boxCol) {
                unordered_set<char> boxSet;

                for (int i = boxRow * 3; i < boxRow * 3 + 3; ++i) {
                    for (int j = boxCol * 3; j < boxCol * 3 + 3; ++j) {
                        if (board[i][j] != '.') {
                            if (boxSet.count(board[i][j])) {
                                return false; // Duplicate in box
                            }
                            boxSet.insert(board[i][j]);
                        }
                    }
                }
            }
        }

        return true; // All checks passed, the board is valid
    }
};
// @lc code=end

