/*
 * @lc app=leetcode.cn id=19 lang=cpp
 *
 * [19] 删除链表的倒数第 N 个结点
 *
 * ===== 游戏客户端开发面试情境题 =====
 *
 * 【情境】你是游戏客户端程序员，正在开发"操作回放/撤销系统"。玩家的操作
 * 记录按时间顺序存储在一个单向链表里。现在要支持一个新功能：撤销"倒数第 N
 * 个操作"。比如玩家说"撤回我最近第 3 步的操作"，你需要从操作链表中删除
 * 倒数第 3 个节点（注意：不是倒数第 N 步那个，而是要把那个操作记录本身删掉）。
 * 由于你不能事先知道链表有多长（玩家的操作次数是动态的），需要一种高效的
 * 一次遍历解法。
 *
 * 【题目】给定一个链表，删除链表的倒数第 n 个结点，并返回链表的头结点。
 * 示例:
 *   输入: head = [1,2,3,4,5], n = 2
 *   输出: [1,2,3,5]
 *   解释: 倒数第2个是4，删除后变成 [1,2,3,5]
 *
 *   输入: head = [1], n = 1
 *   输出: []
 *
 * ===== 核心思维 =====
 *
 * 最朴素的想法：先遍历一遍数出链表长度 L，再走 L-n 步找到目标节点删除。
 * 这需要两次遍历。但面试官通常会追问："能不能只遍历一次？"
 *
 * 答案就是经典的"快慢指针"（也叫"前后指针"或"双指针"）：
 *
 *   1. 让 fast 指针先走 n+1 步（比 slow 领先 n+1 个身位）
 *   2. 然后 fast 和 slow 同时出发，每次各走一步
 *   3. 当 fast 到达链表末尾（nullptr）时，slow 正好停在"倒数第 n+1 个"节点
 *   4. 此时 slow->next 就是倒数第 n 个节点，删掉它即可
 *
 * 为什么是 n+1 而不是 n？
 * 因为要删除一个节点，你需要它的前驱节点来修改 next 指针。
 * 所以我们需要 slow 停在待删节点的前一个位置。
 *
 * 图示（删除倒数第 2 个，即值为 4 的节点）:
 *
 *   初始：dummy → 1 → 2 → 3 → 4 → 5 → null
 *          ^sf
 *
 *   fast 先走 n+1=3 步:
 *   dummy → 1 → 2 → 3 → 4 → 5 → null
 *     s          f
 *
 *   fast 和 slow 一起走:
 *   dummy → 1 → 2 → 3 → 4 → 5 → null
 *               s          f
 *
 *   dummy → 1 → 2 → 3 → 4 → 5 → null
 *                    s               f (f==null, 停止)
 *
 *   slow->next 就是 4，删除它：slow->next = slow->next->next
 *
 * 【哑节点（dummy）的作用】处理边界情况：如果要删除的恰好是头节点，
 * 没有哑节点的话 slow 就无法指向头节点的前驱。加上哑节点后，
 * 所有节点的删除操作统一了。
 *
 * ===== 代码分步讲解 =====
 *
 * 第1步: 创建哑节点 dummy，指向 head
 * 第2步: fast 先走 n+1 步（拉开间距）
 * 第3步: fast 和 slow 同步移动，直到 fast 到末尾
 * 第4步: 此时 slow->next 就是要删除的节点，跳过它
 * 第5步: 释放被删除节点的内存（C++ 需要手动管理）
 * 第6步: 返回 dummy->next（即新链表的头）
 *
 * ===== 可迁移模式 =====
 * 【快慢指针 / 前后指针】是链表问题的万金油：
 *   - 找链表中点：fast 每次走2步，slow 每次走1步
 *   - 判断环形链表：fast 和 slow 是否相遇
 *   - 找倒数第 K 个：fast 先走 K 步
 *   - 游戏应用：操作队列的撤销、时间轴上的跳转、缓存的 LRU 淘汰
 *   - 哑节点技巧让头节点的处理不再特殊
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
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        // 哑节点：统一处理，避免头节点被删时的特殊情况
        ListNode* dummy = new ListNode(0, head);
        ListNode* fast = dummy;
        ListNode* slow = dummy;

        // fast 先走 n+1 步，拉开间距
        for (int i = 0; i <= n; ++i) {
            fast = fast->next;
        }

        // 同步移动，直到 fast 到达链表末尾
        while (fast != nullptr) {
            fast = fast->next;
            slow = slow->next;
        }

        // 此时 slow->next 就是倒数第 n 个节点，删除它
        ListNode* toDelete = slow->next;
        slow->next = slow->next->next;
        delete toDelete;

        ListNode* newHead = dummy->next;
        delete dummy;
        return newHead;
    }
};
// @lc code=end
