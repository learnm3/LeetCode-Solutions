/*
 * @lc app=leetcode.cn id=24 lang=cpp
 *
 * [24] 两两交换链表中的节点
 *
 * ===== 游戏客户端开发面试情境题 =====
 *
 * 【情境】你正在开发一个回合制 RPG 游戏的"羁绊连携系统"。
 * 玩家队伍的出战顺序存储在一个链表中（如 [战士, 法师, 弓手, 牧师]）。
 * 当激活"双人连携"技能时，需要将每两个相邻角色交换位置，
 * 使得他们两两组成连携搭档。例如 [战士,法师,弓手,牧师] 变为 [法师,战士,牧师,弓手]。
 * 如果队伍人数为奇数，最后一人保持不变。
 * 这就是"两两交换链表中的节点"问题。
 *
 * 【题目】给定一个链表，两两交换相邻节点，返回交换后的链表头。
 * 你不能修改节点内部的值，只能修改节点指针（即必须真实交换节点位置）。
 * 示例：
 *   输入：head = [1,2,3,4]
 *   输出：[2,1,4,3]
 *   输入：head = [1]
 *   输出：[1]
 *
 * ===== 核心思维 =====
 *
 * 递归思路（最优雅）：
 *   把问题想象成"处理当前这一对，剩下的交给递归"。
 *
 *   链表：A → B → C → D → E → ...
 *   当前：A 和 B 是一对
 *   目标：B → A → (C,D 交换后的结果)
 *
 *   步骤：
 *   1. 记住 A 是 first，B 是 second
 *   2. first->next = 递归处理 second->next（即 C 开始的剩余部分）
 *   3. second->next = first
 *   4. 返回 second（它现在是新的头）
 *
 *   递归终止条件：链表为空或只剩一个节点，直接返回。
 *
 * 迭代思路（更直观）：
 *   用一个哑节点 dummy 指向 head，然后用 prev 指针追踪当前处理到哪。
 *
 *   初始：dummy → 1 → 2 → 3 → 4
 *   第一轮：dummy → 2 → 1 → 3 → 4（prev 从 0 移到 1）
 *   第二轮：dummy → 2 → 1 → 4 → 3（prev 从 1 移到 3）
 *
 *   每次迭代处理 prev 后面的两个节点：
 *   first = prev->next
 *   second = prev->next->next
 *   prev->next = second         // prev 指向 second
 *   first->next = second->next  // first 指向 second 后面的节点
 *   second->next = first        // second 指向 first
 *   prev = first                // 移动 prev 到 first（它现在是下一对的前一个）
 *
 * ===== 可迁移的解题模式 =====
 *
 * 链表节点交换的核心是"保存引用、调整指针"：
 *   1. 提前保存会被覆盖的指针
 *   2. 按依赖顺序调整（先调整不会被后续步骤用到的指针）
 *   3. 最后更新循环变量
 *
 * 同类问题：
 *   - 反转链表（同样的指针调整逻辑，只是模式不同）
 *   - K 个一组反转链表（本题的泛化版本）
 *   - 游戏技能队列中，每当两个连续技能可以合并施放时就交换
 *   - UI 列表中拖拽交换相邻元素
 */

// @lc code=start
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
public:
    ListNode* swapPairs(ListNode* head) {
        // 边界：空链表或只有一个节点
        if (!head || !head->next) {
            return head;
        }

        // 迭代法：使用哑节点简化头节点处理
        ListNode dummy(0);
        dummy.next = head;
        ListNode* prev = &dummy;

        // 每次处理 prev 后面的两个节点
        while (prev->next && prev->next->next) {
            ListNode* first = prev->next;        // 第一个节点
            ListNode* second = prev->next->next; // 第二个节点

            // 三步指针调整
            prev->next = second;          // prev 指向 second
            first->next = second->next;   // first 指向 second 后面的节点
            second->next = first;         // second 指向 first，完成交换

            // prev 移动到 first（它现在是新一对的前一个）
            prev = first;
        }

        return dummy.next;
    }
};
// @lc code=end
