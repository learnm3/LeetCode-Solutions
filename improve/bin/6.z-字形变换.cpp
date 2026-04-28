5 /*
 * @lc app=leetcode.cn id=6 lang=cpp
 *
 * [6] Z 字形变换
 */

// @lc code=start
class Solution {
public:
    string convert(string s, int numRows) {
        if(numRows == 1) return s;
        vector<string> rows(numRows);
        int i= 0,flag = -1;
        for(char c : s) {
            rows[i] += c;
            if(i == 0 || i == numRows - 1) flag = -flag;
            i += flag;
        }
        string result;
        for(const string& row : rows) {
            result += row;
        }
        return result;
    }
};
// @lc code=end

