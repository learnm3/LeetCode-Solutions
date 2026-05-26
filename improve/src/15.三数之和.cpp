/*
 * @lc app=leetcode.cn id=15 lang=cpp
 *
 * [15] 三数之和
 *
 * ===== 游戏客户端开发面试情境题 =====
 *
 * 【情境】你是游戏客户端程序员，负责开发"炼金术合成"系统。玩家可以将
 * 三种材料放入坩埚中，只有当三种材料的魔力值之和恰好为 0 时（正负魔力
 * 互相抵消），才能合成出完美药剂。现在你的背包里有 n 种材料（每种只能用
 * 一次），你需要找出所有可能的三材料组合。注意：不能出现重复的组合。
 *
 * 【题目】给定一个包含 n 个整数的数组 nums，判断 nums 中是否存在三个元素
 * a, b, c，使得 a + b + c = 0。请找出所有和为 0 且不重复的三元组。
 * 示例:
 *   输入: nums = [-1,0,1,2,-1,-4]
 *   输出: [[-1,-1,2],[-1,0,1]]
 *   解释: nums[0]+nums[1]+nums[2] = (-1)+0+1 = 0 ✓
 *         nums[1]+nums[2]+nums[4] = 0+1+(-1) = 0 ✓ (但与上面重复，不计)
 *         nums[0]+nums[3]+nums[4] = (-1)+2+(-1) = 0 ✓
 *
 * ===== 核心思维 =====
 *
 * 如果暴力三重循环，时间复杂度 O(n^3)，n=3000 时完全不可接受。
 * 聪明的做法是"排序 + 双指针"，将复杂度降至 O(n^2)。
 *
 * 核心技巧：先把数组排序，然后：
 *   固定第一个数 a = nums[i]
 *   在 a 的右边区域用双指针找 b 和 c，使得 b + c = -a
 *   这就把三数之和降维成了两数之和！
 *
 * 排序后的数组: [-4, -1, -1, 0, 1, 2]
 *               ^i  ^L     ^R
 *
 * 固定 i=0, a=-4, 需要 b+c=4
 *   L=-1, R=2, sum=-1+2=1 < 4  → L 右移
 *   L=-1, R=2, sum=-1+2=1 < 4  → L 右移
 *   L=0,  R=2, sum=0+2=2 < 4   → L 右移
 *   L=1,  R=2, sum=1+2=3 < 4   → L 右移
 *   L>R, 结束，没有找到和为4的组合
 *
 * 固定 i=1, a=-1, 需要 b+c=1
 *   L=-1, R=2, sum=-1+2=1 = 1  → 找到 [-1,-1,2] ✓
 *   L右移(跳过重复)，R左移(跳过重复)
 *   L=0, R=1, sum=0+1=1 = 1   → 找到 [-1,0,1] ✓
 *
 * 【去重是关键难点】三个位置都要去重：
 *   1. 外层 i 去重：if (i>0 && nums[i]==nums[i-1]) continue
 *   2. 找到答案后 L 去重：while (L<R && nums[L]==nums[L+1]) L++
 *   3. 找到答案后 R 去重：while (L<R && nums[R]==nums[R-1]) R--
 *
 * ===== 代码分步讲解 =====
 *
 * 第1步: 排序（这是双指针的前提）
 * 第2步: 外层循环固定第一个数，跳过重复的 i
 * 第3步: 双指针在 i 右侧搜索，根据 sum 与 0 的关系移动指针
 * 第4步: 找到答案后，跳过所有重复的 L 和 R，继续搜索
 *
 * ===== 可迁移模式 =====
 * 【排序 + 双指针降维】是处理"N数之和"类问题的通用模板：
 *   - 三数之和：1重循环 + 双指针，O(n^2)
 *   - 四数之和：2重循环 + 双指针，O(n^3)
 *   - 泛化到 K-Sum：可递归降维或用 K-2 重循环 + 双指针
 *   - 游戏应用：合成配方匹配、属性点数搭配、装备词条组合
 *   核心思想：排序后利用单调性控制指针移动方向。
 */

#include <vector>
#include <algorithm>
using namespace std;

// @lc code=start
class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        vector<vector<int>> result;
        int n = nums.size();
        if (n < 3) return result;

        sort(nums.begin(), nums.end());                // 排序是双指针的前提

        for (int i = 0; i < n - 2; ++i) {
            if (nums[i] > 0) break;                    // 最小数 > 0，不可能和为0
            if (i > 0 && nums[i] == nums[i - 1]) continue; // 跳过重复的 i

            int L = i + 1, R = n - 1;
            while (L < R) {
                int sum = nums[i] + nums[L] + nums[R];

                if (sum == 0) {
                    result.push_back({nums[i], nums[L], nums[R]});
                    // 跳过重复的 L 和 R
                    while (L < R && nums[L] == nums[L + 1]) L++;
                    while (L < R && nums[R] == nums[R - 1]) R--;
                    L++; R--;
                } else if (sum < 0) {
                    L++;                               // 和太小，左指针右移增大和
                } else {
                    R--;                               // 和太大，右指针左移减小和
                }
            }
        }

        return result;
    }
};
// @lc code=end
