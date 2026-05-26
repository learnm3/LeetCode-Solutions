/*
 * @lc app=leetcode.cn id=8 lang=cpp
 *
 * [8] 字符串转换整数 (atoi)
 *
 * ===== 游戏客户端开发面试情境题 =====
 *
 * 【情境】聊天指令解析器 —— 从玩家输入中提取数值参数
 *
 * 你正在为 MMO 游戏开发聊天指令系统。玩家在聊天框输入指令来使用道具或技能，
 * 例如 "/use 42"（使用背包第 42 格物品）、"/buff +15"（给自己加 15 点 buff）。
 *
 * 但玩家的输入往往不规范：开头可能有空格（"  /use 42"），可能带正负号，
 * 数字后面可能跟了无关文字（"42 个药水"）。你需要鲁棒地从字符串中提取出
 * 首个有效整数，同时忽略前导空格、处理符号、忽略后续非数字字符，
 * 并对超出 32 位 int 范围的值做截断处理。
 *
 * 这个需求本质上就是实现 C 标准库的 atoi 函数，也是 LeetCode 第 8 题。
 *
 * 游戏开发中的类比场景：
 * - 配置文件解析：从 INI/JSON 字段中提取整数（容错处理）
 * - 控制台命令：解析 "set health 100" 中的 100
 * - UI 输入框：用户可能在数字前后误输入空格或字符
 * - 聊天过滤器：从消息中提取坐标 "@pos 128,256"
 *
 * 【题目】
 * 实现一个 myAtoi(s) 函数，将字符串转换为 32 位有符号整数。
 *
 * 算法步骤：
 *   1. 读入字符串并丢弃前导空格
 *   2. 检查下一个字符是 '+' 或 '-'（如果有），确定符号
 *   3. 读入数字字符直到遇到非数字或字符串结束
 *   4. 如果未读入任何数字，返回 0
 *   5. 如果结果超过 [-2^31, 2^31-1] 范围，截断到边界值
 *
 * 示例：
 *   输入: "42"              输出: 42
 *   输入: "   -42"          输出: -42
 *   输入: "4193 with words" 输出: 4193
 *   输入: "words and 987"   输出: 0
 *   输入: "-91283472332"    输出: -2147483648  （截断到 INT_MIN）
 *
 * ===== 核心思维 =====
 *
 * 一、为什么用有限状态机（FSM / Automaton）？
 *
 * 这个问题有清晰的状态转换逻辑：
 *   - start: 初始状态，读空格停留在 start，读到符号进入 signed，读到数字进入 in_number
 *   - signed: 读到符号后，读到数字进入 in_number，读其他进入 end
 *   - in_number: 读数过程中，读数字还在 in_number，读非数字进入 end
 *   - end: 终止状态，读到什么都不变
 *
 * 用 if-else 也能写，但状态机更清晰、更易维护。游戏开发中到处是状态机：
 *   - 角色状态（待机、行走、攻击、死亡）
 *   - UI 状态（登录、主菜单、游戏中、暂停）
 *   - 网络连接状态（断开、连接中、已连接、重连）
 *
 * 二、状态机设计
 *
 *   状态转移表（列：空格 | +/- | 数字 | 其他）：
 *
 *          空格     +/-      数字     其他
 *   start   start   signed  in_number  end
 *   signed  end     end    in_number  end
 *   in_num  end     end    in_number  end
 *   end     end     end    end        end
 *
 * 三、溢出处理技巧
 *
 * 使用 long long (64位) 存储中间结果，然后与 INT_MAX/INT_MIN 比较并截断。
 * 这避免了在 32 位范围内做复杂的提前判断。
 *
 * 更严谨的做法（不依赖 64 位）是在每次 ans*10 + digit 之前检测溢出，
 * 类似第 7 题。但本题使用 long long 更简单直观，面试中完全可接受。
 *
 * 四、复杂度分析
 *
 *   - 时间复杂度：O(n)，每个字符处理一次。
 *   - 空间复杂度：O(1)，状态表大小固定。
 *
 * 五、可迁移模式 —— 有限状态机 (FSM)
 *
 * 状态机是游戏开发中最常用的设计模式之一。核心要素：
 *   - 状态集合（states）
 *   - 事件/输入集合（columns）
 *   - 转移表（transition table）
 *   - 当前状态（current state）
 *   - 每个状态下的动作（actions）
 *
 * 适用于：
 *   - 角色动画状态机（Animator / Animation Blueprint）
 *   - AI 行为树/状态机（巡逻、追击、攻击、返回）
 *   - 对话系统（根据玩家选择跳转不同分支）
 *   - 游戏流程控制（启动画面 → 主菜单 → 加载 → 游戏中 → 结算）
 */

#include <string>
#include <unordered_map>
#include <vector>
#include <climits>
#include <cctype>
using namespace std;

// @lc code=start
class Automaton {
    string state = "start"; // 当前状态
    // 状态转移表：4 个状态 x 4 种字符类型
    unordered_map<string, vector<string>> table = {
        {"start",     {"start", "signed", "in_number", "end"}},
        {"signed",    {"end",   "end",    "in_number", "end"}},
        {"in_number", {"end",   "end",    "in_number", "end"}},
        {"end",       {"end",   "end",    "end",       "end"}}
    };

    // 根据字符类型返回列索引：0=空格, 1=+/-, 2=数字, 3=其他
    int get_col(char c) {
        if (c == ' ') return 0;       // 空格
        if (c == '+' || c == '-') return 1; // 符号
        if (isdigit(c)) return 2;     // 数字
        return 3;                     // 其他
    }

public:
    int sign = 1;          // 正负号，1 为正，-1 为负
    long long ans = 0;     // 使用 64 位存储避免中间溢出

    void get(char c) {
        state = table[state][get_col(c)]; // 状态转移
        if (state == "in_number") {
            ans = ans * 10 + (c - '0');
            // 溢出截断：利用 long long 与 INT_MAX/INT_MIN 比较
            ans = (sign == 1)
                ? min(ans, (long long)INT_MAX)
                : min(ans, -(long long)INT_MIN);
        } else if (state == "signed") {
            sign = (c == '+') ? 1 : -1;
        }
        // 其他状态（start, end）不做任何事
    }
};

class Solution {
public:
    int myAtoi(string s) {
        Automaton automaton;
        for (char c : s) {
            automaton.get(c);
        }
        return automaton.sign * automaton.ans;
    }
};
// @lc code=end
