/*
 * @lc app=leetcode.cn id=37 lang=cpp
 *
 * [37] 解数独
 */

// @lc code=start
class Solution {
public:
    void solveSudoku(vector<vector<char>>& board) {
        bool row[9][9] = {false}, col[9][9] = {false}, box[9][9] = {false};
        vector<pair<int, int>> emptyCells;
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                if (board[i][j] == '.') {
                    emptyCells.emplace_back(i, j);
                } else {
                    int num = board[i][j] - '1';
                    row[i][num] = col[j][num] = box[(i / 3) * 3 + j / 3][num] = true;
                }
            }
        }

        std::function<bool(int)> backtrack = [&](int idx) {
            if (idx == emptyCells.size()) return true;
            auto [i, j] = emptyCells[idx];
            for (int num = 0; num < 9; ++num) {
                if (!row[i][num] && !col[j][num] && !box[(i / 3) * 3 + j / 3][num]) {
                    board[i][j] = '1' + num;
                    row[i][num] = col[j][num] = box[(i / 3) * 3 + j / 3][num] = true;
                    if (backtrack(idx + 1)) return true;
                    board[i][j] = '.';
                    row[i][num] = col[j][num] = box[(i / 3) * 3 + j / 3][num] = false;
                }
            }
            return false;
        };
        backtrack(0);
    }
};
// @lc code=end

