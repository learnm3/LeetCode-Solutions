/*
 * @lc app=leetcode.cn id=25 lang=cpp
 *
 * [25] K 个一组翻转链表
 *
 * ===== 游戏客户端开发面试情境题 =====
 *
 * 【情境】你在开发一个格斗游戏的"连招编辑器"。玩家输入了一串指令序列
 * （链表），编辑器支持"段落反转"功能：选中连续的 K 个指令（如 K=3），
 * 按下一个快捷键即可反转这组指令的顺序（用于创建反向连招变体）。
 * 从头开始，每 K 个指令为一组进行反转，如果最后不足 K 个指令则保持原样。
 * 这就是"K 个一组翻转链表"问题。
 *
 * 另一个情境：音游的谱面编辑器中，每 K 个音符为一小节，
 * 编辑"镜像模式"时需要反转每小节内的音符顺序。
 *
 * 【题目】给定链表 head 和整数 k，从头开始每 k 个节点为一组翻转，
 * 不足 k 个的组不翻转，返回翻转后的链表头。
 * 示例：
 *   输入：head = [1,2,3,4,5], k = 2
 *   输出：[2,1,4,3,5]
 *   输入：head = [1,2,3,4,5], k = 3
 *   输出：[3,2,1,4,5]
 *
 * ===== 核心思维 =====
 *
 * 核心策略："检查 + 翻转 + 递归"
 *
 * 形象理解：
 *   把链表看成一条绳子，上面打了结，每 K 个结是一个小组。
 *   你从绳子头开始，先数 K 个结——如果能数够，就把这一段绳子倒过来拿；
 *   然后对剩下的绳子做同样的事。
 *
 * 步骤拆解：
 *   1. 从当前头节点开始，数 K 个节点。如果不够 K 个，直接返回当前头（不翻转）
 *   2. 如果够 K 个，翻转这 K 个节点
 *   3. 翻转后，原来的头变成了尾。把原来的尾（新尾）的 next 指向
 *      递归处理剩余链表的结果
 *   4. 返回翻转后的新头（原来是这组的最后一个节点）
 *
 * 形象图示：
 *   链表：[1 → 2 → 3] → [4 → 5 → 6] → [7 → 8]  (k=3)
 *
 *   第一组 [1,2,3] 翻转后：[3 → 2 → 1]
 *   第二组 [4,5,6] 翻转后：[6 → 5 → 4]
 *   第三组 [7,8] 不足 3 个，不翻转
 *
 *   连接：3→2→1 → 6→5→4 → 7→8
 *
 * 翻转 K 个节点的函数：
 *   prev = null, curr = head
 *   循环 K 次：
 *     next = curr.next  // 保存下一个
 *     curr.next = prev  // 翻转指针
 *     prev = curr       // prev 前移
 *     curr = next       // curr 前移
 *   返回 {newHead=prev, newTail=head}
 *
 * ===== 可迁移的解题模式 =====
 *
 * "分组处理 + 递归串联" 模式：
 *   1. 找出本组范围
 *   2. 对本组执行变换
 *   3. 递归处理剩余部分
 *   4. 串联本组结果和递归结果
 *
 * 同类问题：
 *   - 两两交换链表节点（k=2 的特例）
 *   - 反转链表（k=n 的特例）
 *   - 游戏动画序列的分组反转（每 N 帧反转一次）
 *   - 文本编辑器中对选定块执行"行反转"
 *   - 数据压缩中的分块处理（每个块独立变换后拼回）
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
    // 翻转 k 个节点，返回 {新头, 新尾}
    pair<ListNode*, ListNode*> reverseK(ListNode* head, int k) {
        ListNode* prev = nullptr;
        ListNode* curr = head;
        for (int i = 0; i < k; i++) {
            ListNode* nxt = curr->next;  // 先保存下一个节点
            curr->next = prev;           // 翻转当前节点的指针
            prev = curr;                 // prev 前进
            curr = nxt;                  // curr 前进
        }
        // 翻转后：prev 是新的头，head 是新的尾
        return {prev, head};
    }

    ListNode* reverseKGroup(ListNode* head, int k) {
        // 从 head 开始数 k 个节点，检查是否够 k 个
        ListNode* curr = head;
        int count = 0;
        while (curr && count < k) {
            curr = curr->next;
            count++;
        }

        // 够 k 个：翻转当前组
        if (count == k) {
            auto [newHead, newTail] = reverseK(head, k);
            // newTail 是原来的 head，将其 next 指向下一组的结果
            newTail->next = reverseKGroup(curr, k);
            return newHead;
        }

        // 不够 k 个：保持原样
        return head;
    }
};
// @lc code=end
