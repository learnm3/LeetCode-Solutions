/*
 * @lc app=leetcode.cn id=40 lang=cpp
 *
 * [40] 组合总和 II
 *
 * ============================================================
 * 游戏客户端开发面试情境题
 * ============================================================
 *
 * 【情境】
 * 延续上一题（39）的商店凑单，但策划改了规则：
 * 现在每种商品只有一件库存（不能重复购买），且可能有多件
 * 同价不同名的商品（比如"新手剑"和"木剑"都卖 3 金币）。
 * 你需要枚举所有"刚好花光金币"的组合，但相同价格的商品
 * 被视为等价——同价商品的排列组合要合并去重。
 *
 * 游戏开发中同样的场景：
 * - 装备栏位分配：每个装备只能用一次，同属性装备视为等价
 * - 限时活动兑换：每种奖励只能换一次，同类奖励去重
 * - 卡组构筑：每张卡只能用一次，同费卡视为等价
 * - 背包分解：每个物品分解一次，同价格物品去重
 * - 合成材料搭配：每种材料各有限，同属性材料视为等价
 *
 * ============================================================
 * 题目
 * ============================================================
 *
 * 和 39 的区别：
 *   - candidates 可能包含重复数字
 *   - 每个数字只能使用一次
 *   - 结果中不能有重复的组合
 *
 * 示例：
 *   输入: candidates = [10,1,2,7,6,1,5], target = 8
 *   输出: [[1,1,6],[1,2,5],[1,7],[2,6]]
 *
 * ============================================================
 * 核心思维：排序 + 同层去重
 * ============================================================
 *
 * 和 39 题有两个关键区别：
 *
 * 区别 1：每个数字只能用一次
 *   → 递归传 dfs(i+1, ...) 而非 dfs(i, ...)
 *
 * 区别 2：数组有重复元素，需去重
 *   → 先排序，再用 "同层跳过" 策略：
 *
 *   if (i > start && candidates[i] == candidates[i-1]) continue;
 *      ~~~~~~~~~~
 *      注意！这里是 i > start，不是 i > 0
 *
 * 为什么要用 i > start 而不是 i > 0？
 *
 * 排序后 candidates = [1a, 1b, 2, 5, 6, 7, 10]
 *
 *   情况 A：i > start → 同层兄弟节点去重
 *     start=0 时选 1a，递归 start=1。
 *     在 start=1 层，i=1 是 1b，i > start(1)=1 → 不触发跳过
 *     → 允许 [1a, 1b, ...] ✅ 这是合法的（两个不同的 1）
 *
 *   情况 B：i > 0 → 全局去重（错误！）
 *     在 start=1 层，i=1, candidates[1]=1b, candidates[0]=1a
 *     i>0 为 true，跳过 → ❌ 丢失了 [1a, 1b, ...] 这个解！
 *
 * 总结：
 *   - 排列去重（47 题）：用 used 数组，!used[i-1] → 同层跳过
 *   - 组合去重（40 题）：用 i > start，同层跳过
 *   两种去重的适用范围不同！
 *
 * ============================================================
 * 代码分步讲解
 * ============================================================
 */

#include <vector>
#include <algorithm>
#include <functional>
using namespace std;

// @lc code=start
class Solution {
public:
    vector<vector<int>> combinationSum2(vector<int>& candidates, int target) {
        // ---- ① 排序，让相同数字紧挨着（去重前提） ----
        sort(candidates.begin(), candidates.end());

        vector<vector<int>> result;
        vector<int> path;

        // ---- ② 回溯 ----
        function<void(int, int)> dfs = [&](int start, int sum) {
            if (sum == target) {
                result.push_back(path);
                return;
            }

            for (int i = start; i < candidates.size(); ++i) {
                // ---- ③ 同层去重：相同数字在本层只走第一个 ----
                if (i > start && candidates[i] == candidates[i - 1]) continue;

                // ---- ④ 剪枝 ----
                if (sum + candidates[i] > target) break;  // 排过序，后面更大

                path.push_back(candidates[i]);
                dfs(i + 1, sum + candidates[i]);  // i+1：每个数字只用一次
                path.pop_back();
            }
        };

        dfs(0, 0);
        return result;
    }
};
// @lc code=end
