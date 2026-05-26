/*
 * @lc app=leetcode.cn id=17 lang=cpp
 *
 * [17] 电话号码的字母组合
 *
 * ===== 游戏客户端开发面试情境题 =====
 *
 * 【情境】你是游戏客户端程序员，负责开发"技能连招提示系统"。在格斗游戏中，
 * 玩家通过按键序列触发连招技能。每个按键（如 A/B/X/Y）对应一组可能的招式。
 * 当你按下第一个键 A 时，系统需要列出所有可能的连招路径。
 * 例如：按键 2 对应 "abc"，按键 3 对应 "def"，
 * 那么所有可能的组合就是 ["ad", "ae", "af", "bd", "be", "bf", "cd", "ce", "cf"]。
 * 这其实是一个"笛卡尔积"问题，用回溯法可以优雅解决。
 *
 * 【题目】给定一个仅包含数字 2-9 的字符串，返回所有它能表示的字母组合。
 * 数字到字母的映射与电话按键相同（2=abc, 3=def, 4=ghi, 5=jkl, 6=mno,
 * 7=pqrs, 8=tuv, 9=wxyz）。注意 1 不对应任何字母。
 * 示例:
 *   输入: digits = "23"
 *   输出: ["ad","ae","af","bd","be","bf","cd","ce","cf"]
 *   输入: digits = ""
 *   输出: []
 *
 * ===== 核心思维 =====
 *
 * 想象你在构建一棵"决策树"。树的每一层代表一个数字按键，
 * 这一层的每个分支代表该按键对应的一个字母选项。
 * 回溯法就是在这棵树上做 DFS（深度优先搜索），走到底就收集一条完整路径。
 *
 * 以 "23" 为例，构建的树：
 *
 *                   (空)
 *               /    |    \
 *              a     b     c      ← 第1层：数字2的三个选择
 *            /|\   /|\   /|\
 *           d e f d e f d e f    ← 第2层：数字3的三个选择
 *           ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓
 *          ad ae af bd be bf cd ce cf  ← 叶子节点：全部9种组合
 *
 * 回溯的三部曲：
 *   1. 做选择：把 current 字母加入路径
 *   2. 递归：下探到下一层（下一个数字）
 *   3. 撤销选择：把刚加的字母弹出（回溯），尝试下一个字母
 *
 * 这个"做选择 → 递归 → 撤销"的模式，就像游戏中"尝试一个分支，
 * 不行就回退再试另一个"——RPG 对话树、技能释放序列验证都会用到。
 *
 * ===== 代码分步讲解 =====
 *
 * 第1步: 特判空输入
 * 第2步: 建立数字→字母的映射表
 * 第3步: 回溯函数：如果走到尽头（index==digits.size()），收集结果
 * 第4步: 否则，获取当前数字对应的字母串，遍历每个字母
 * 第5步: 选择→递归→撤销，经典回溯三部曲
 *
 * ===== 可迁移模式 =====
 * 【回溯 = 做选择 + 递归 + 撤销】适用于：
 *   - 全排列、子集、组合问题（都是回溯的变体）
 *   - 技能连招序列枚举
 *   - 迷宫寻路（四方向选择）
 *   - 装备宝石镶嵌的所有可能搭配
 *   - 对话树中的所有可能结局路径
 *   框架就是：for each 选项 { 选它 → 递归 → 撤销它 }
 */

#include <vector>
#include <string>
using namespace std;

// @lc code=start
class Solution {
public:
    vector<string> letterCombinations(string digits) {
        if (digits.empty()) return {};

        vector<string> result;
        string path;                                   // 当前正在构建的组合
        backtrack(digits, 0, path, result);
        return result;
    }

private:
    // 数字到字母的映射表（0和1无意义，占位用）
    const vector<string> mapping = {
        "", "", "abc", "def", "ghi", "jkl",
        "mno", "pqrs", "tuv", "wxyz"
    };

    void backtrack(const string& digits, int idx,
                   string& path, vector<string>& result) {
        // 到达叶子节点：所有数字都处理完毕，收集结果
        if (idx == (int)digits.size()) {
            result.push_back(path);
            return;
        }

        int digit = digits[idx] - '0';
        for (char ch : mapping[digit]) {
            path.push_back(ch);                        // 1. 做选择
            backtrack(digits, idx + 1, path, result);  // 2. 下探下一层
            path.pop_back();                           // 3. 撤销选择（回溯）
        }
    }
};
// @lc code=end
