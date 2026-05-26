/*
 * @lc app=leetcode.cn id=14 lang=cpp
 *
 * [14] 最长公共前缀
 *
 * ===== 游戏客户端开发面试情境题 =====
 *
 * 【情境】你是游戏客户端程序员，负责资源管理系统。项目中有成千上万个
 * 纹理贴图文件，它们的命名遵循一定的目录结构约定，例如：
 *   "Character/Hero/Warrior/Idle/Idle_01.png"
 *   "Character/Hero/Warrior/Idle/Idle_02.png"
 *   "Character/Hero/Warrior/Idle/Run_01.png"
 * 你需要找出这些资源路径的"公共前缀"，以便在 UI 中折叠显示，
 * 或者构建资源包的目录树。这就是最长公共前缀问题。
 *
 * 【题目】编写一个函数，查找字符串数组 strs 中的最长公共前缀。
 * 如果不存在公共前缀，返回空字符串 ""。
 * 示例:
 *   输入: strs = ["flower","flow","flight"]    输出: "fl"
 *   输入: strs = ["dog","racecar","car"]        输出: ""  (无公共前缀)
 *
 * ===== 核心思维 =====
 *
 * 思路非常直观：想象你在校对一排试卷上的名字 ——
 * 你先看所有人的第一个字是不是一样，再看第二个字，再看第三个...
 * 一旦发现有不一致的字母，就停下来，之前一致的部分就是公共前缀。
 *
 * 这就是"垂直扫描法"：逐列比较。
 *
 * 以 ["flower", "flow", "flight"] 为例：
 *
 *   列0: f, f, f  → 相同，加入前缀 → "f"
 *   列1: l, l, l  → 相同，加入前缀 → "fl"
 *   列2: o, o, i  → 不同！停止 → 返回 "fl"
 *
 * 再考虑 ["abc", "ab"]：
 *   列0: a, a → 相同 → "a"
 *   列1: b, b → 相同 → "ab"
 *   列2: c, ? → "ab"只有2个字符，越界！停止 → 返回 "ab"
 *
 * ===== 代码分步讲解 =====
 *
 * 第1步: 处理空数组输入
 * 第2步: 以第一个字符串的长度为最大可能前缀长度
 * 第3步: 再求所有字符串的最短长度（避免越界）
 * 第4步: 逐列扫描，比较第0个字符串与其他所有字符串在当前位置的字符
 * 第5步: 一旦发现不匹配，立即返回当前已积累的前缀
 * 第6步: 全匹配通过后将该字符追加到结果
 *
 * ===== 可迁移模式 =====
 * 【垂直扫描 / 逐列对比】适用于：
 *   - 文件路径的公共目录前缀
 *   - 比较版本号字符串的公共前缀（如 "1.2.3" 和 "1.2.4" → "1.2."）
 *   - 技能名称分组（"火球术_初级", "火球术_中级" → 公共前缀 "火球术_"）
 *   - 字典树（Trie）的基础思想
 *   时间复杂度 O(S)，S 为所有字符总数；空间复杂度 O(1)。
 */

#include <string>
#include <vector>
#include <algorithm>
using namespace std;

// @lc code=start
class Solution {
public:
    string longestCommonPrefix(vector<string>& strs) {
        if (strs.empty()) return "";

        // 前缀的长度不会超过最短的那个字符串
        int minLen = strs[0].size();
        for (int i = 1; i < (int)strs.size(); ++i) {
            minLen = min(minLen, (int)strs[i].size());
        }

        // 逐列扫描
        string result;
        for (int col = 0; col < minLen; ++col) {
            char c = strs[0][col];               // 以第一个字符串为基准
            for (int row = 1; row < (int)strs.size(); ++row) {
                if (strs[row][col] != c) {       // 出现不一致
                    return result;
                }
            }
            result.push_back(c);                 // 全列一致，加入前缀
        }

        return result;
    }
};
// @lc code=end
