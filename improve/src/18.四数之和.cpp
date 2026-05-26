/*
 * @lc app=leetcode.cn id=18 lang=cpp
 *
 * [18] 四数之和
 *
 * ===== 游戏客户端开发面试情境题 =====
 *
 * 【情境】你是游戏客户端程序员，负责开发"公会副本"的组队系统。团长需要
 * 从公会成员中选出 4 人组成攻坚小队，要求四人的战斗力之和恰好等于副本
 * 推荐战力。由于副本难度固定，战力"溢出"没有额外收益，"不足"则无法通关，
 * 所以要精确匹配。现在公会名册中每个人的战力都已记录在数组中，你需要
 * 找出所有可能的四人小队组合，且不能有重复组队方案。
 *
 * 【题目】给定一个包含 n 个整数的数组 nums 和一个目标值 target，判断 nums
 * 中是否存在四个元素 a, b, c, d，使得 a+b+c+d = target。找出所有满足条件
 * 且不重复的四元组。
 * 示例:
 *   输入: nums = [1,0,-1,0,-2,2], target = 0
 *   输出: [[-2,-1,1,2],[-2,0,0,2],[-1,0,0,1]]
 *
 * ===== 核心思维 =====
 *
 * 四数之和本质上是三数之和的"套娃"。回顾三数之和：
 *   固定一个数 → 在右侧用双指针找两数之和
 * 四数之和就是多套一层：
 *   固定第一个数 → 固定第二个数 → 在右侧用双指针找两数之和
 *
 * 复杂度从暴力 O(n^4) 降到 O(n^3)。
 *
 * 排序后: [-2, -1, 0, 0, 1, 2], target=0
 *
 * 固定 i=0 (a=-2), j=1 (b=-1):
 *   L=2, R=5, sum=-2+(-1)+0+2=-1 < 0 → L++
 *   L=3, R=5, sum=-2+(-1)+0+2=-1 < 0 → L++
 *   L=4, R=5, sum=-2+(-1)+1+2=0 → 找到 [-2,-1,1,2] ✓
 *
 * 【四重去重】比三数之和多一层：
 *   i 去重: if (i>0 && nums[i]==nums[i-1]) continue
 *   j 去重: if (j>i+1 && nums[j]==nums[j-1]) continue
 *   L 去重: while (L<R && nums[L]==nums[L+1]) L++
 *   R 去重: while (L<R && nums[R]==nums[R-1]) R--
 *
 * 【溢出陷阱】四个 int 相加可能溢出！需要用 long long 来存储 sum。
 * 比如 nums=[1000000000,1000000000,1000000000,1000000000]，
 * int 最多存 2147483647，但四数之和是 4000000000，直接溢出变负数。
 * 这才是面试中容易被忽略的细节。
 *
 * ===== 代码分步讲解 =====
 *
 * 第1步: 边界检查（少于4个数直接返回空）
 * 第2步: 排序
 * 第3步: 第一层循环（固定 i），去重
 * 第4步: 第二层循环（固定 j），去重
 * 第5步: 双指针在 j 右侧搜索，用 long long 防溢出
 * 第6步: 找到答案后双端去重，继续搜索
 *
 * ===== 可迁移模式 =====
 * 【N-Sum 模板】可以泛化到任意 N 数之和：
 *   - 2-Sum: 只有双指针
 *   - 3-Sum: 1重循环 + 双指针
 *   - 4-Sum: 2重循环 + 双指针
 *   - K-Sum: K-2重循环 + 双指针（或用递归实现）
 *   - 游戏应用：多人组队匹配、属性总和精确筛选、掉落物品的组合背包
 *   始终记住：排序保证单调性，去重保证结果不重复，long long 防溢出。
 */

#include <vector>
#include <algorithm>
using namespace std;

// @lc code=start
class Solution {
public:
    vector<vector<int>> fourSum(vector<int>& nums, int target) {
        vector<vector<int>> result;
        int n = nums.size();
        if (n < 4) return result;

        sort(nums.begin(), nums.end());

        for (int i = 0; i < n - 3; ++i) {
            if (i > 0 && nums[i] == nums[i - 1]) continue;     // i 去重

            for (int j = i + 1; j < n - 2; ++j) {
                if (j > i + 1 && nums[j] == nums[j - 1]) continue; // j 去重

                int L = j + 1, R = n - 1;
                while (L < R) {
                    // 用 long long 防止四个 int 相加溢出
                    long long sum = (long long)nums[i] + nums[j]
                                  + nums[L] + nums[R];

                    if (sum == target) {
                        result.push_back({nums[i], nums[j], nums[L], nums[R]});
                        // 跳过重复的 L 和 R
                        while (L < R && nums[L] == nums[L + 1]) L++;
                        while (L < R && nums[R] == nums[R - 1]) R--;
                        L++; R--;
                    } else if (sum < target) {
                        L++;
                    } else {
                        R--;
                    }
                }
            }
        }

        return result;
    }
};
// @lc code=end
