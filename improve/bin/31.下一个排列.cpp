/*
 * @lc app=leetcode.cn id=31 lang=cpp
 *
 * [31] 下一个排列
 */

// @lc code=start
class Solution {
public:
    void nextPermutation(vector<int>& nums) {
        int n = nums.size();
        if (n <= 1) return;

        // Step 1: Find the largest index k such that nums[k] < nums[k + 1]. 
        // If no such index exists, the permutation is the last permutation.
        int k = -1;
        for (int i = n - 2; i >= 0; --i) {
            if (nums[i] < nums[i + 1]) {
                k = i;
                break;
            }
        }

        if (k == -1) {
            // This is the last permutation, reverse to get the first permutation
            reverse(nums.begin(), nums.end());
            return;
        }

        // Step 2: Find the largest index l greater than k such that nums[k] < nums[l].
        int l = -1;
        for (int i = n - 1; i > k; --i) {
            if (nums[k] < nums[i]) {
                l = i;
                break;
            }
        }

        // Step 3: Swap the value of nums[k] with that of nums[l].
        swap(nums[k], nums[l]);

        // Step 4: Reverse the sequence from nums[k + 1] up to and including the final element nums[n-1].
        reverse(nums.begin() + k + 1, nums.end());
    }
};
// @lc code=end

