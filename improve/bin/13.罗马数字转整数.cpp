/*
 * @lc app=leetcode.cn id=13 lang=cpp
 *
 * [13] 罗马数字转整数
 */

// @lc code=start
class Solution {
private:
    unordered_map<char, int> symbolValues = {
        {'I', 1},
        {'V', 5},
        {'X', 10},
        {'L', 50},
        {'C', 100},
        {'D', 500},
        {'M', 1000},
    };

public:
    int romanToInt(string s) {
        int total = 0;
        for (size_t i = 0; i < s.size(); ++i) {
            int value = symbolValues[s[i]];
            if (i + 1 < s.size() && value < symbolValues[s[i + 1]]) {
                total -= value;
            } else {
                total += value;
            }
        }
        return total;
    }
};
// @lc code=end

