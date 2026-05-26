/*
 * @lc app=leetcode.cn id=16 lang=cpp
 *
 * [16] 最接近的三数之和
 *
 * ===== 游戏客户端开发面试情境题 =====
 *
 * 【情境】你是游戏客户端程序员，正在开发"战斗评分系统"。玩家的最终评分
 * 由三个维度的分数加权求和决定（击杀分 + 助攻分 + 存活分）。策划想调
 * 整权重使总评分尽可能接近目标值 1000。现在你需要从候选的 n 个权重值中
 * 选出三个，使它们的和（即总分）最接近目标值。每种权重只能使用一次，
 * 但你只需要返回这个最接近的和，不需要返回具体是哪三个。
 *
 * 【题目】给定一个长度为 n 的整数数组 nums 和一个目标值 target。从 nums
 * 中选出三个整数，使它们的和与 target 最接近。返回这三个数的和。
 * 假定每组输入只存在恰好一个解。
 * 示例:
 *   输入: nums = [-1,2,1,-4], target = 1
 *   输出: 2
 *   解释: 最接近 1 的和是 2（-1+2+1=2），|2-1|=1 最小
 *
 * ===== 核心思维 =====
 *
 * 这道题和三数之和（#15）是孪生兄弟，解法几乎一模一样：
 * 同样是"排序 + 固定一个数 + 双指针"，区别只在于判断条件从
 * "是否等于0"变成了"是否更接近 target"。
 *
 * 排序后的数组: [-4, -1, 1, 2]  target=1
 *
 * 初始 closestSum = -4+(-1)+1 = -4  (先随便取一个)
 *
 * 固定 i=0, a=-4:
 *   L=-1, R=2, sum=-4+(-1)+2=-3, | -3-1|=4 < |-4-1|=5 → closestSum=-3
 *                     sum=-3 < target=1 → L++ (和太小)
 *   L=1,  R=2, sum=-4+1+2=-1,   | -1-1|=2 < | -3-1|=4 → closestSum=-1
 *                     sum=-1 < target=1 → L++
 *   L>R 结束
 *
 * 固定 i=1, a=-1:
 *   L=1, R=2, sum=-1+1+2=2,    |2-1|=1 < |-1-1|=2  → closestSum=2  ← 答案!
 *                     sum=2 = target? 不等, sum=2 > target=1 → R-- (和太大)
 *   L>R 结束
 *
 * 返回 2。
 *
 * ===== 代码分步讲解 =====
 *
 * 第1步: 排序
 * 第2步: 初始化 closestSum 为前三个数的和
 * 第3步: 外层循环固定第一个数，内层双指针扫描
 * 第4步: 每次更新 closestSum 如果当前 sum 更接近 target
 * 第5步: 根据 sum 与 target 的关系移动指针（和三数之和一样）
 * 第6步: 如果正好命中 target，直接返回（这就是最优解）
 *
 * ===== 可迁移模式 =====
 * 【最接近目标的搜索】同样基于排序+双指针：
 *   - 寻找最接近目标值的装备属性组合
 *   - 技能冷却时间搭配，使总冷却最接近某个值
 *   - 伤害数字浮动范围的近似匹配
 *   关键技巧：用绝对值差值来衡量"接近程度"，维护一个全局最优解。
 */

#include <vector>
#include <algorithm>
#include <cstdlib>
using namespace std;

// @lc code=start
class Solution {
public:
    int threeSumClosest(vector<int>& nums, int target) {
        sort(nums.begin(), nums.end());
        int n = nums.size();

        // 初始化答案为任意一个有效组合（前三个）
        int closestSum = nums[0] + nums[1] + nums[2];

        for (int i = 0; i < n - 2; ++i) {
            int L = i + 1, R = n - 1;

            while (L < R) {
                int sum = nums[i] + nums[L] + nums[R];

                // 更新最优解：谁离 target 更近就用谁
                if (abs(sum - target) < abs(closestSum - target)) {
                    closestSum = sum;
                }

                if (sum == target) {
                    return sum;                        // 完美命中，提前结束
                } else if (sum < target) {
                    L++;                               // 和太小，需要更大的数
                } else {
                    R--;                               // 和太大，需要更小的数
                }
            }
        }

        return closestSum;
    }
};
// @lc code=end
