/*
 * @lc app=leetcode.cn id=38 lang=cpp
 *
 * [38] 外观数列
 */

// @lc code=start
class Solution {
public:
    string countAndSay(int n) {
        if (n == 1) return "1";
        string prev = countAndSay(n - 1), result;
        for (size_t i = 0; i < prev.size();) {
            size_t j = i + 1;
            while (j < prev.size() && prev[j] == prev[i]) ++j;
            result += to_string(j - i) + prev[i];
            i = j;
        }
        return result;
    }
};
// @lc code=end

