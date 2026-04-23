/*
 * @lc app=leetcode.cn id=28 lang=cpp
 *
 * [28] 找出字符串中第一个匹配项的下标
 */

// @lc code=start
class Solution {
public:
    int strStr(string haystack, string needle) {
        int n = haystack.size(), m = needle.size();
        for (int i = 0; i <= n - m; i++) {
            int j = 0;
            for (; j < m; j++) {
                if (haystack[i + j] != needle[j]) {
                    break;
                }
            }
            if (j == m) {
                return i;
            }
        }
        return -1;
    }
};
// @lc code=end

