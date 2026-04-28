/*
 * @lc app=leetcode.cn id=34 lang=cpp
 *
 * [34] 在排序数组中查找元素的第一个和最后一个位置
 */

// @lc code=start
class Solution {
public:
    vector<int> searchRange(vector<int>& nums, int target) {
        vector<int> result(2, -1); // Initialize result with -1
        int left = 0, right = nums.size() - 1;

        // Find the first occurrence of target
        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (nums[mid] < target) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }

        // Check if the target is found
        if (left < nums.size() && nums[left] == target) {
            result[0] = left; // First occurrence index
        } else {
            return result; // Target not found, return [-1, -1]
        }

        // Find the last occurrence of target
        right = nums.size() - 1; // Reset right pointer
        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (nums[mid] > target) {
                right = mid - 1;
            } else {
                left = mid + 1;
            }
        }

        result[1] = right; // Last occurrence index
        return result;
    }
};
// @lc code=end

