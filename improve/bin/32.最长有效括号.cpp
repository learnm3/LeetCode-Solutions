/*
 * @lc app=leetcode.cn id=32 lang=cpp
 *
 * [32] 最长有效括号
 */

// @lc code=start
class Solution {
public:
    int longestValidParentheses(string s) {
        int maxLength = 0;
        stack<int> indices;
        indices.push(-1); // Base index for valid substring

        for (int i = 0; i < s.length(); ++i) {
            if (s[i] == '(') {
                indices.push(i); // Push the index of the '('
            } else {
                indices.pop(); // Pop the last index

                if (indices.empty()) {
                    indices.push(i); // Push the current index as a base for future valid substrings
                } else {
                    maxLength = max(maxLength, i - indices.top()); // Update max length
                }
            }
        }

        return maxLength;
    }
};
// @lc code=end

