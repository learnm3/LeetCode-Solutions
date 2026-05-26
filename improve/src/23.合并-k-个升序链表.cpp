/*
 * @lc app=leetcode.cn id=23 lang=cpp
 *
 * [23] 合并 K 个升序链表
 *
 * ===== 游戏客户端开发面试情境题 =====
 *
 * 【情境】你在开发一个 MOBA 游戏的战斗统计系统。游戏服务器集群有 K 个节点，
 * 每个节点按时间顺序记录了该节点上发生的击杀事件日志（已按时间升序排列）。
 * 你需要将 K 条有序的击杀日志链表合并成一条全局的有序击杀时间线，
 * 以便在客户端回放时按顺序展示所有击杀公告。这就是"合并 K 个升序链表"问题。
 *
 * 【题目】给定 K 个升序链表，将它们合并为一条升序链表，返回合并后的链表头。
 * 示例：
 *   输入：lists = [[1,4,5],[1,3,4],[2,6]]
 *   输出：[1,1,2,3,4,4,5,6]
 *
 * ===== 核心思维 =====
 *
 * 有 2 种主流解法：
 *
 * 解法一：顺序两两合并（朴素法）
 *   每次取两条链表，用"合并两个有序链表"的办法合并。
 *   第一条和第二条合并得到一条新链表，再和第三条合并...
 *   时间复杂度：O(k * N)，其中 N 是所有节点总数。
 *   缺点：每条链表可能被重复遍历很多次。
 *
 * 解法二：最小堆（优先队列）—— 推荐
 *   想象你有 K 个水龙头，每个水龙头按顺序滴水（从小到大的数字）。
 *   你站在下面，每次伸手接最小的那一滴。
 *   - 把 K 个链表的头节点全部扔进一个"最小堆"
 *   - 每次弹出堆顶（当前最小的节点），接到结果链表尾部
 *   - 然后把弹出节点所在链表的下一个节点扔回堆里
 *   - 重复直到堆为空
 *   时间复杂度：O(N * log K)，每条链表只遍历一次。
 *
 * 类比理解：
 *   你在超市有 K 个收银台排队，你想按购物金额从小到大服务顾客。
 *   你站在所有队伍前面，每次扫一眼所有队伍最前面的人，挑金额最小的服务，
 *   然后那个队伍的下一个人走到最前面，你再扫一眼...这就是最小堆的逻辑。
 *
 *   每轮比较 K 个队首 → 取最小 → 该队下一个进堆
 *   堆帮你 O(log K) 完成"找最小"这一步。
 *
 * ===== 代码分步讲解 =====
 *
 * 1. 定义优先队列（最小堆），存储 (节点值, 链表索引) 对
 * 2. 将所有非空链表的头节点入堆
 * 3. 循环：弹出堆顶 → 接入结果链表 → 该链表下一个节点入堆
 * 4. 返回 dummy.next
 *
 * ===== 可迁移的解题模式 =====
 *
 * 这个"多路归并"模式的本质是：
 *   - 维护一个大小为 K 的候选集合
 *   - 每次从候选集合中取出"最优"元素
 *   - 从取出元素所在的数据源补充新的候选
 *
 * 同类问题：
 *   - 合并 K 个有序数组（用堆维护每个数组的当前位置）
 *   - 求多个有序数组中第 K 小的元素
 *   - 游戏匹配系统中，从多个等待队列中选出优先级最高的玩家
 *   - 多个战斗单位按攻击间隔轮流出手（每个单位的 CD 时间是一个有序序列）
 */

#include <vector>
#include <queue>
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
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        // 最小堆：存储 (节点值, 节点指针)
        // 自定义比较器，按值从小到大排
        auto cmp = [](ListNode* a, ListNode* b) { return a->val > b->val; };
        priority_queue<ListNode*, vector<ListNode*>, decltype(cmp)> pq(cmp);

        // 将所有非空链表的头节点入堆
        for (ListNode* head : lists) {
            if (head != nullptr) {
                pq.push(head);
            }
        }

        // 哑节点简化头节点处理
        ListNode dummy;
        ListNode* tail = &dummy;

        // 每次从堆中取出最小节点，接入结果链表
        while (!pq.empty()) {
            ListNode* node = pq.top();
            pq.pop();

            tail->next = node;
            tail = tail->next;

            // 如果该节点还有后继，将后继入堆
            if (node->next != nullptr) {
                pq.push(node->next);
            }
        }

        return dummy.next;
    }
};
// @lc code=end
