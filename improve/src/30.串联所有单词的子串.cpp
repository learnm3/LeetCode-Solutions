/*
 * @lc app=leetcode.cn id=30 lang=cpp
 *
 * [30] 串联所有单词的子串
 *
 * ===== 游戏客户端开发面试情境题 =====
 *
 * 【情境】你在开发一款格斗游戏的"连招检测系统"。
 * 玩家的操作日志是一长串指令字符串（s），例如 "ABDCA" 代表按键序列。
 * 系统预定义了若干"基础招式"（words = ["AB","DC","A"]），
 * 你需要检测操作日志中是否存在一个子串，恰好由所有基础招式各出现一次
 * 串联而成（顺序任意）。找到所有满足条件的起始位置。
 * 这就是"串联所有单词的子串"问题。
 *
 * 另一个情境：聊天系统中检测玩家是否连续输入了一组特定关键词
 * （例如检测广告刷屏模式），或者音游中检测玩家的 Combo 是否命中
 * 了特定的音符序列组合。
 *
 * 【题目】给定字符串 s 和一个字符串数组 words（所有单词长度相同），
 * 找出 s 中所有"串联子串"的起始索引。
 * "串联子串"是指由 words 中所有单词以任意顺序连接而成的子串。
 * 示例：
 *   输入：s = "barfoothefoobarman", words = ["foo","bar"]
 *   输出：[0,9]（"barfoo" 和 "foobar" 都是有效的）
 *   输入：s = "wordgoodgoodgoodbestword", words = ["word","good","best","word"]
 *   输出：[]
 *
 * ===== 核心思维 =====
 *
 * 核心策略："滑动窗口 + 哈希表词频统计"
 *
 * 题目本质：在 s 中找长度为 totalLen = 单词长度 * 单词数量 的窗口，
 * 窗口内的子串恰好由 words 中所有单词组成。
 *
 * 如果逐位置滑动窗口，每次检查需要 O(totalLen)，总共 O(n * totalLen)。
 *
 * 优化：由于所有单词长度相同（设为 wLen），可以把 s 按"起始偏移"分成
 * wLen 组，每组内的单词边界天然对齐。
 *
 * 例如 s = "barfoothe...", wLen = 3：
 *   偏移 0 组：bar foo the ...（每个单词 3 个字符）
 *   偏移 1 组：arf oot ... （跳过 s[0]）
 *   偏移 2 组：rfo oth ... （跳过 s[0..1]）
 *
 * 每组内部使用滑动窗口：
 *   - 窗口维护 count 个有效单词
 *   - 右边界每次取一个新单词
 *     - 如果单词不在 wordMap 中：窗口重置
 *     - 如果单词有效但超出需求数：左边界收缩直到单词数合法
 *     - 如果 count == wordCount：找到一个解
 *
 * 图解（s="barfoothefoobarman", words=["foo","bar"]）：
 *   wordLen=3, totalLen=6
 *   偏移0组：bar foo the foo bar man
 *              [bar foo] ✓ 位置0
 *                   [foo the] ✗
 *                      [the foo] ✗
 *                           [foo bar] ✓ 位置9
 *
 * ===== 可迁移的解题模式 =====
 *
 * "滑动窗口 + 词频 / 字符频统计" 是一大类算法问题：
 *   1. 确认窗口大小（固定或可变）
 *   2. 维护窗口内的频率统计（通常用哈希表）
 *   3. 右扩时更新频率，左缩时恢复频率
 *   4. 用 count 变量追踪"已满足条件"的个数（避免每次遍历哈希表）
 *
 * 同类问题：
 *   - 最小覆盖子串（76）：可变窗口 + 字符频率
 *   - 找到字符串中所有字母异位词（438）：固定窗口 + 字母频率
 *   - 游戏中连招检测：固定窗口 + 招式频率
 *   - 聊天敏感词组合检测：滑动窗口 + 关键词频率
 */

#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

// @lc code=start
class Solution {
public:
    vector<int> findSubstring(string s, vector<string>& words) {
        vector<int> result;
        if (words.empty() || s.empty()) return result;

        int wordLen = words[0].size();   // 每个单词的长度
        int wordCount = words.size();     // 单词数量
        int totalLen = wordLen * wordCount;

        if (s.size() < totalLen) return result;

        // 统计目标单词的频率
        unordered_map<string, int> targetFreq;
        for (const string& w : words) targetFreq[w]++;

        // 按起始偏移分组，每组内做滑动窗口
        for (int offset = 0; offset < wordLen; offset++) {
            unordered_map<string, int> windowFreq; // 当前窗口内的单词频率
            int left = offset;     // 窗口左边界
            int matched = 0;       // 窗口内已匹配的有效单词数

            // 右边界每次移动一个单词
            for (int right = offset; right + wordLen <= s.size(); right += wordLen) {
                string curWord = s.substr(right, wordLen);

                // 情况1：单词有效（在 words 中）
                if (targetFreq.count(curWord)) {
                    windowFreq[curWord]++;
                    matched++;

                    // 如果某个单词出现次数超标，左边界收缩
                    while (windowFreq[curWord] > targetFreq[curWord]) {
                        string leftWord = s.substr(left, wordLen);
                        windowFreq[leftWord]--;
                        matched--;
                        left += wordLen;
                    }

                    // 窗口内单词数达到要求，记录结果
                    if (matched == wordCount) {
                        result.push_back(left);
                        // 左边界右移一个单词，继续搜索
                        string leftWord = s.substr(left, wordLen);
                        windowFreq[leftWord]--;
                        matched--;
                        left += wordLen;
                    }
                }
                // 情况2：单词无效，清空窗口
                else {
                    windowFreq.clear();
                    matched = 0;
                    left = right + wordLen;
                }
            }
        }

        return result;
    }
};
// @lc code=end
