/*
 * @lc app=leetcode.cn id=14 lang=cpp
 *
 * [14] 最长公共前缀
 */

// @lc code=start
class Solution {
public:
    string longestCommonPrefix(vector<string>& strs) {
        if(strs.empty()) {
            return "";
        }
        int length = strs[0].size();
        for (size_t i = 1; i < strs.size(); ++i) {
            length = min(length, (int)strs[i].size());
        }
        string prefix = "";
        for (int j = 0; j < length; ++j) {
            char c = strs[0][j];
            for (size_t i = 1; i < strs.size(); ++i) {
                if (strs[i][j] != c) {
                    return prefix;
                }
            }
            prefix += c;
        }
        return prefix;
    }
};
// @lc code=end

