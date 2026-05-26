/*
 * @lc app=leetcode.cn id=21 lang=cpp
 *
 * [21] 合并两个有序链表
 *
 * ===== 游戏客户端开发面试情境题 =====
 *
 * 【情境】你是游戏客户端程序员，负责开发"PVP 排行榜"系统。服务器会分批次
 * 返回排名数据（比如好友榜和公会榜），每批数据都是按积分降序排列的。
 * 客户端需要将两批有序数据合并成一个完整的排行榜展示给玩家。由于数据量
 * 可能很大（万级玩家），合并过程必须高效，不能破坏原有的有序性。
 *
 * 【题目】将两个升序链表合并为一个新的升序链表并返回。新链表是通过拼接
 * 给定的两个链表的所有节点组成的。
 * 示例:
 *   输入: l1 = [1,2,4], l2 = [1,3,4]
 *   输出: [1,1,2,3,4,4]
 *
 *   输入: l1 = [], l2 = [0]
 *   输出: [0]
 *
 * ===== 核心思维 =====
 *
 * 有序链表的合并就像"两副已经排好序的扑克牌，你要把它们插成一叠"。
 *
 * 递归解法（最优雅）：
 *
 *   思路：每次比较两个链表的头节点，较小的那个"胜出"成为新链表的头，
 *   然后递归地合并它的 next 和另一个链表。
 *
 *   以 l1=[1,2,4], l2=[1,3,4] 为例：
 *
 *   比较 1 和 1 → l1.val = l2.val = 1，选 l1（或 l2 都可）
 *     l1.next = merge(l1.next=[2,4], l2=[1,3,4])
 *       比较 2 和 1 → l2.val=1 < 2，选 l2
 *         l2.next = merge(l1=[2,4], l2.next=[3,4])
 *           比较 2 和 3 → l1.val=2 < 3，选 l1
 *             l1.next = merge(l1.next=[4], l2=[3,4])
 *               比较 4 和 3 → l2.val=3 < 4，选 l2
 *                 l2.next = merge(l1=[4], l2.next=[4])
 *                   比较 4 和 4，选 l1
 *                     l1.next = merge(l1.next=null, l2=[4])
 *                       l1 为空 → 返回 l2=[4]
 *
 *   结果链: 1 → 1 → 2 → 3 → 4 → 4  ✓
 *
 * 递归的核心洞察：
 *   merge(A, B) = 较小的头 + merge(较小头的next, 另一个链表)
 *
 * ===== 代码分步讲解 =====
 *
 * 第1步: 递归终止条件 —— 任一个链表为空，直接返回另一个
 * 第2步: 比较两个头节点的值
 * 第3步: 较小值的节点"胜出"，它的 next 指向递归合并的结果
 * 第4步: 返回"胜出"的节点作为新链表的头
 *
 * ===== 可迁移模式 =====
 * 【归并逻辑】广泛用于"分治"类问题：
 *   - 归并排序（Merge Sort）的核心操作
 *   - 合并多个有序排行榜/日志流
 *   - K 路归并（用最小堆扩展）
 *   - 游戏应用：合并多个服务器的排行榜、合并任务列表、合并聊天消息流
 *   递归版本代码最简洁，但栈深度可能过大；迭代版本用哑节点更安全。
 */

#include <iostream>
using namespace std;

// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// @lc code=start
class Solution {
public:
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        // 终止条件：任一链表为空，直接返回另一条
        if (l1 == nullptr) return l2;
        if (l2 == nullptr) return l1;

        // 较小值的节点"胜出"，成为当前层的头节点
        if (l1->val < l2->val) {
            l1->next = mergeTwoLists(l1->next, l2);    // l1 胜出，递归合并剩余
            return l1;
        } else {
            l2->next = mergeTwoLists(l1, l2->next);    // l2 胜出，递归合并剩余
            return l2;
        }
    }
};
// @lc code=end
