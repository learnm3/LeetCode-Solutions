/*
 * @lc app=leetcode.cn id=39 lang=cpp
 *
 * [39] 组合总和
 */

// @lc code=start
class Solution {
public:
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        vector<vector<int>> result;
        vector<int> combination;
        std::function<void(int, int)> backtrack = [&](int start, int sum) {
            if (sum == target) {
                result.push_back(combination);
                return;
            }
            for (size_t i = start; i < candidates.size(); ++i) {
                if (sum + candidates[i] > target) continue;
                combination.push_back(candidates[i]);
                backtrack(i, sum + candidates[i]);
                combination.pop_back();
            }
        };
        backtrack(0, 0);
        return result;
    }
};
// @lc code=end

