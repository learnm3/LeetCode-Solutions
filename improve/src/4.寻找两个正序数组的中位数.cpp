/*
 * @lc app=leetcode.cn id=4 lang=cpp
 *
 * [4] 寻找两个正序数组的中位数
 *
 * ===== 游戏客户端开发面试情境题 =====
 *
 * 【情境】竞技游戏的天梯排名合并
 *
 * 你正在开发一款 MOBA 游戏的天梯排名系统。由于跨服合区，服务器 A 和服务器 B
 * 各自维护了一份玩家排位分的有序列表（都已按分数从低到高排序）。现在需要合并
 * 这两份列表，找出全体玩家的中位数排位分，作为新赛季的"基准分数线"。
 *
 * 服务器 A 有 10 万玩家，服务器 B 有 8 万玩家。如果直接合并两个数组再排序，
 * 时间复杂度 O((m+n)log(m+n))，在百万级数据量下需要数百毫秒。
 *
 * 但两个数组本身就是有序的——这提示我们可以用更高效的方法。
 *
 * 游戏开发中的类比场景：
 * - 伤害统计合并：两个副本队伍的输出数据已排序，找伤害中位数评估团本难度
 * - 帧率分析：两个测试场景的帧时间已排序，找中位数判断性能基准
 * - 装备属性池：两套装备库的属性值已排序，找中位数设计新副本掉落
 *
 * 【题目】
 * 给定两个大小分别为 m 和 n 的正序（从小到大）数组 nums1 和 nums2。
 * 找出这两个正序数组的中位数，要求时间复杂度 O(log(m+n))。
 *
 * 示例：
 *   输入: nums1 = [1, 3], nums2 = [2]
 *   输出: 2.0  （合并 = [1,2,3]，中位数 2）
 *
 *   输入: nums1 = [1, 2], nums2 = [3, 4]
 *   输出: 2.5  （合并 = [1,2,3,4]，中位数 (2+3)/2 = 2.5）
 *
 * ===== 核心思维 =====
 *
 * 一、问题转化：中位数 = 第 k 小的数
 *
 * 设总元素数 total = m + n。
 *   - 若 total 为奇数：中位数 = 第 (total/2 + 1) 小的元素
 *   - 若 total 为偶数：中位数 = (第 total/2 小的元素 + 第 total/2+1 小的元素) / 2
 *
 * 所以核心问题变为：如何在两个有序数组中高效地找到第 k 小的元素？
 *
 * 二、二分排除法：每次排除 k/2 个元素
 *
 * 关键洞察：如果我们在两个数组各取前 k/2 个元素（不足则取到末尾），
 * 比较这两个"候选"元素，较小的那个及其前面的所有元素一定不可能是第 k 小的。
 *
 * 可视化：
 *
 *   数组A: [1, 3, 5, 7, 9]      k=7, 各取 k/2=3 个
 *             ↑ k/2=3            A[2]=5
 *   数组B: [2, 4, 6, 8, 10]
 *             ↑ k/2=3            B[2]=6
 *
 *   A[2]=5 < B[2]=6，所以 A 的前 3 个元素 [1,3,5] 都不可能是第 7 小的。
 *   为什么？即使 B 的前 3 个 [2,4,6] 全部小于 5，5 前面最多只有 3+3=6 个元素，
 *   5 最多是第 7 小（实际可能更小）。而 5 本身在 A 中排第 3，所以 A[0..2]
 *   最多排到第 6，不可能是第 7 小。排除后 k 减为 7-3=4，继续在剩余元素中找。
 *
 * 三、递归/迭代过程
 *
 *   1. 每次比较 A[k/2-1] 和 B[k/2-1]（注意下标从 0 开始）
 *   2. 排除较小值所在数组的前 k/2 个元素
 *   3. k 减去排除的数量
 *   4. 边界情况：
 *      - 某个数组已空 → 直接从另一个数组取第 k 个
 *      - k=1 → 返回两个数组当前首元素的最小值
 *      - 某数组长度不足 k/2 → 取到末尾，排除数量按实际剩余算
 *
 * 四、复杂度分析
 *
 *   - 时间复杂度：O(log(m+n))。每次将 k 减半，k 初始约为 (m+n)/2。
 *   - 空间复杂度：O(1)。迭代版不使用递归栈。
 *
 * 五、可迁移模式 —— 二分排除（k-th element in sorted arrays）
 *
 * 这个模式适用于"在多个有序结构中找第 k 小"的场景：
 *   - 在多个有序链表中找中位数
 *   - 在两个有序矩阵中找第 k 小
 *   - 在有序数据流中维护中位数（配合两个堆）
 *
 * 相关题：
 *   - 378. 有序矩阵中第 K 小的元素
 *   - 668. 乘法表中第 K 小的数
 *   - 719. 找出第 K 小的距离对
 */

#include <vector>
#include <algorithm>
using namespace std;

// @lc code=start
class Solution {
public:
    // 在 nums1[start1..] 和 nums2[start2..] 中找第 k 小的元素（k 从 1 开始）
    int getKthElement(const vector<int>& nums1, const vector<int>& nums2, int k) {
        int m = nums1.size(), n = nums2.size();
        int idx1 = 0, idx2 = 0; // 两个数组的当前起始下标

        while (true) {
            // 边界情况1：nums1 已耗尽
            if (idx1 == m) return nums2[idx2 + k - 1];
            // 边界情况2：nums2 已耗尽
            if (idx2 == n) return nums1[idx1 + k - 1];
            // 边界情况3：找第 1 小的元素
            if (k == 1) return min(nums1[idx1], nums2[idx2]);

            // 取 k/2 个元素，但要防止越界
            int half = k / 2;
            int next1 = min(idx1 + half - 1, m - 1);
            int next2 = min(idx2 + half - 1, n - 1);
            int pivot1 = nums1[next1], pivot2 = nums2[next2];

            // 排除较小 pivot 所在数组的前半部分
            if (pivot1 <= pivot2) {
                k -= (next1 - idx1 + 1); // 实际排除的数量
                idx1 = next1 + 1;        // nums1 的起点后移
            } else {
                k -= (next2 - idx2 + 1);
                idx2 = next2 + 1;
            }
        }
    }

    double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
        int total = nums1.size() + nums2.size();
        if (total % 2 == 1) {
            // 奇数个元素：中位数 = 中间那个
            return getKthElement(nums1, nums2, total / 2 + 1);
        } else {
            // 偶数个元素：中位数 = 中间两个的平均值
            int left  = getKthElement(nums1, nums2, total / 2);
            int right = getKthElement(nums1, nums2, total / 2 + 1);
            return (left + right) / 2.0;
        }
    }
};
// @lc code=end
