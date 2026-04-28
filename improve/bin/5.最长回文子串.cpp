/*
 * @lc app=leetcode.cn id=5 lang=cpp
 *
 * [5] 最长回文子串
 */

// @lc code=start
class Solution {
public:
    pair<int, int> expandAroundCenter(const string& s, int left, int right) {
        while (left >= 0 && right < s.size() && s[left] == s[right]) {
            left--;
            right++;
        }
        return {left + 1, right - 1};
    }
    string longestPalindrome(string s) {
        int start = 0, maxLen = 1;
        for (int i = 0; i < s.size(); i++) {
            auto [left1, right1] = expandAroundCenter(s, i, i);
            auto [left2, right2] = expandAroundCenter(s, i, i + 1);
            int len1 = right1 - left1 + 1, len2 = right2 - left2 + 1;
            int len = max(len1, len2);
            if (len > maxLen) {
                maxLen = len;
                start = (len == len1) ? left1 : left2;
            }
        }
        return s.substr(start, maxLen);
    }
};
// @lc code=end

