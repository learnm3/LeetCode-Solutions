/*
 * @lc app=leetcode.cn id=22 lang=cpp
 *
 * [22] 括号生成
 */

// @lc code=start
class Solution {
public:
    bool isValid(const string& s) {
        stack<char> stk;
        for (char ch : s) {
            if (ch == '(') {
                stk.push(ch);
            } else if (ch == ')') {
                if (stk.empty() || stk.top() != '(') {
                    return false;
                }
                stk.pop();
            }
        }
        return stk.empty();
    }

    void backtrack(string& current, int openCount, int closeCount, int n, vector<string>& result) {
        if (current.size() == 2 * n) {
            if (isValid(current)) {
                result.push_back(current);
            }
            return;
        }
        if (openCount < n) {
            current.push_back('(');
            backtrack(current, openCount + 1, closeCount, n, result);
            current.pop_back();
        }
        if (closeCount < n) {
            current.push_back(')');
            backtrack(current, openCount, closeCount + 1, n, result);
            current.pop_back();
        }
    }
    vector<string> generateParenthesis(int n) {
        vector<string> result;
        string current;
        backtrack(current, 0, 0, n, result);
        return result;
    }
};
// @lc code=end

