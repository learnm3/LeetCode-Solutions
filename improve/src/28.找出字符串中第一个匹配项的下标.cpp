/*
 * @lc app=leetcode.cn id=28 lang=cpp
 *
 * [28] 找出字符串中第一个匹配项的下标
 *
 * ===== 游戏客户端开发面试情境题 =====
 *
 * 【情境】你在开发一款网游的"聊天敏感词过滤系统"。
 * 玩家发送的聊天消息是一段长字符串（haystack），你需要检测其中是否包含
 * 特定的关键词（needle），并返回关键词首次出现的位置以便替换为星号。
 * 这就是经典的字符串匹配问题——实现 indexOf / strStr 函数。
 *
 * 另一个情境：游戏资源路径中查找特定的目录名前缀，
 * 或者文本冒险游戏中识别玩家的输入指令是否包含某个命令关键词。
 *
 * 【题目】给定两个字符串 haystack 和 needle，返回 needle 在 haystack 中
 * 第一次出现的下标，如果 needle 不是 haystack 的一部分则返回 -1。
 * 示例：
 *   输入：haystack = "sadbutsad", needle = "sad"
 *   输出：0
 *   输入：haystack = "leetcode", needle = "leeto"
 *   输出：-1
 *
 * ===== 核心思维 =====
 *
 * 解法一：暴力匹配（朴素法）—— 简单但慢
 *   从 haystack 的每个位置开始，尝试匹配 needle 的每一个字符。
 *   如果全部匹配成功，返回当前位置；如果中途失败，换下一个位置。
 *   时间复杂度：O(n * m)，n 是 haystack 长度，m 是 needle 长度。
 *
 * 解法二：KMP 算法 —— 快速但复杂
 *   利用 needle 自身的重复模式构建"部分匹配表"（也叫 next 数组/前缀表），
 *   匹配失败时不必从头开始，而是跳到已匹配前缀的最长相等前后缀位置。
 *   时间复杂度：O(n + m)。
 *
 * 对于面试：先讲暴力匹配的思路（清晰），然后提 KMP 优化（展示深度）。
 * 对于游戏开发场景：暴力匹配在短关键词（通常 < 20 字符）下足够快，
 * KMP 适用于长关键词或批量匹配场景。
 *
 * 暴力匹配图解（haystack = "hello", needle = "ll"）：
 *   从位置 0 开始：'h' != 'l'，失败
 *   从位置 1 开始：'e' != 'l'，失败
 *   从位置 2 开始：'l'=='l','l'=='l'，成功！返回 2
 *
 * KMP 核心思想图解（needle = "ababc"）：
 *   前缀表（next 数组）的含义：
 *     next[i] = needle[0..i] 的最长相等前后缀长度
 *     needle = a  b  a  b  c
 *     next   = 0  0  1  2  0
 *
 *   例如 next[3]=2 表示 "abab" 的前缀 "ab" 和后缀 "ab" 相等。
 *   当匹配到 needle[4]='c' 失败时，不需要回到开头，
 *   而是跳到 needle[next[3]] = needle[2] = 'a' 继续匹配。
 *
 * ===== 可迁移的解题模式 =====
 *
 * 字符串匹配的进阶路线：
 *   暴力 O(n*m) → KMP O(n+m) → Boyer-Moore（实践中常最快）→ Rabin-Karp（哈希）
 *
 * 在游戏中：
 *   - 聊天敏感词过滤：暴力或 Trie 树（多模式匹配用 AC 自动机）
 *   - 玩家 ID 搜索：模糊匹配用编辑距离
 *   - 资源路径解析：直接 API，很少手写
 *   - 脚本解析器中的 token 匹配：状态机比字符串匹配更常用
 */

#include <string>
#include <vector>
using namespace std;

// @lc code=start
class Solution {
public:
    int strStr(string haystack, string needle) {
        int n = haystack.size(), m = needle.size();
        if (m == 0) return 0;          // 空 needle 约定返回 0
        if (n < m) return -1;           // haystack 比 needle 还短

        // 从 haystack 的每个可能起始位置尝试匹配
        for (int i = 0; i <= n - m; i++) {
            int j = 0;
            // 逐字符匹配 needle
            while (j < m && haystack[i + j] == needle[j]) {
                j++;
            }
            if (j == m) return i;       // 全部匹配成功
        }

        return -1; // 未找到
    }
};
// @lc code=end
