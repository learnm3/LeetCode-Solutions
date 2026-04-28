/*
 * @lc app=leetcode.cn id=29 lang=cpp
 *
 * [29] 两数相除
 */

// @lc code=start
class Solution {
public:
    int divide(int dividend, int divisor) {
        if (divisor == 0) return INT_MAX; // 处理除数为0的情况
        if (dividend == INT_MIN && divisor == -1) return INT_MAX; // 处理溢出情况

        // 使用 long long 来避免溢出
        long long a = abs((long long)dividend);
        long long b = abs((long long)divisor);
        long long result = 0;

        while (a >= b) {
            long long temp = b, multiple = 1;
            while (a >= (temp << 1)) {
                temp <<= 1;
                multiple <<= 1;
            }
            a -= temp;
            result += multiple;
        }

        // 根据符号调整结果
        return ((dividend > 0) ^ (divisor > 0)) ? -result : result;
    }
};
// @lc code=end

