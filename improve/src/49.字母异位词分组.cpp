/*
 * @lc app=leetcode.cn id=49 lang=cpp
 *
 * [49] 字母异位词分组
 *
 * ============================================================
 * 游戏客户端开发面试情境题
 * ============================================================
 *
 * 【情境】
 * 聊天敏感词系统（延续第 44 题的场景）。
 * 玩家把 "fuck" 打成 "kufc" 想绕过检测。
 * 你需要把所有"用同一组字母拼出来的词"归到一类，
 * 不管顺序怎么变，都能命中同一个敏感词模式。
 *
 * 另一个场景：字谜/拼词小游戏。
 * 比如 "eat" "tea" "ate" 用同样的字母组成，分到同一组。
 *
 * 游戏开发中的场景：
 * - 物品搜索别名："生命药水" "药水生命" 都搜到同一物品
 * - 玩家起名检测：多个小号用相同字符排列当名字
 * - 成就系统：检测背包里是否有"任意排列的 N 种符文"
 * - 技能组合匹配：装备的符文无论顺序，视为同一套装
 *
 * ============================================================
 * 题目
 * ============================================================
 *
 * 异位词 = 字母完全相同，只是顺序不同的两个字符串。
 * 给一组字符串，把互为异位词的分到同一组。
 *
 * 示例：
 *   输入: ["eat", "tea", "tan", "ate", "nat", "bat"]
 *   输出: [["eat","tea","ate"], ["tan","nat"], ["bat"]]
 *
 * ============================================================
 * 核心思维：给每个字符串算一个"身份证号"
 * ============================================================
 *
 * 异位词的共同点：排完序之后，长得一模一样！
 *   "eat" 排序 → "aet"
 *   "tea" 排序 → "aet"
 *   "ate" 排序 → "aet"
 *   这三个词有相同的身份证号 = "aet"
 *
 * 步骤：
 *   ① 对每个单词，算出它排序后的字符串作为"身份证号"
 *   ② 身份证号相同的，扔进哈希表的同一组
 *   ③ 把哈希表里所有组倒出来
 *
 * 就像分班：每个学生报生日，同一天生日的分到同一个班。
 */

// ============================================================
//   C++ 零基础语法讲解
// ============================================================

/*
 * 【这次用到的头文件】
 *
 *   <vector>    → 动态数组。可以装任何类型，自动管理大小。
 *                  vector<int> = 装整数的数组
 *                  vector<string> = 装字符串的数组
 *                  vector<vector<string>> = 装"字符串数组"的数组（二维数组）
 *
 *   <string>    → C++ 的字符串类型。比 C 语言的 char* 好用得多：
 *                  可以直接 = 赋值，可以 + 拼接，可以 .size() 问长度。
 *
 *   <unordered_map> → 哈希表（字典）。键→值的快速映射。
 *
 *   <algorithm> → 算法工具箱。有一堆现成的算法函数，
 *                  本题用到 sort() 排序函数。
 */
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
using namespace std;

// @lc code=start
class Solution {
public:
    /*
     * 【vector<vector<string>> groupAnagrams(vector<string>& strs)】
     *
     *   逐段拆解：
     *
     *   返回值 vector<vector<string>>
     *     → 一个"装字符串数组的数组"，也就是二维的字符串表。
     *       例: [["eat","tea","ate"], ["tan","nat"], ["bat"]]
     *       外层的每个元素是一个 vector<string>（一组异位词）。
     *
     *   函数名 groupAnagrams
     *     group = 分组, Anagrams = 异位词
     *
     *   参数 vector<string>& strs
     *     vector<string> → 装字符串的动态数组（输入的全部单词）
     *     & → 引用。借用原数据，不复制。省内存。
     *     strs → 参数名（= strings 的缩写）
     */
    vector<vector<string>> groupAnagrams(vector<string>& strs) {

        /*
         * 【unordered_map<string, vector<string>> groups;】
         *   声明一个哈希表，名字叫 groups（= 分组）。
         *
         *   类型拆解：unordered_map<键的类型, 值的类型>
         *
         *   键 = string（排序后的身份证号，比如 "aet"）
         *   值 = vector<string>（拥有该身份证号的所有原始单词的集合）
         *
         *   逻辑结构（画成表）：
         *   ┌────────┬──────────────────────┐
         *   │ "aet"  │ ["eat", "tea", "ate"]│  ← 键是排序串，值是一组词
         *   ├────────┼──────────────────────┤
         *   │ "ant"  │ ["tan", "nat"]       │
         *   ├────────┼──────────────────────┤
         *   │ "abt"  │ ["bat"]              │
         *   └────────┴──────────────────────┘
         */
        unordered_map<string, vector<string>> groups;

        /*
         * 【for (string& s : strs) { ... }】
         *   这是 C++11 的"范围 for 循环"（range-based for loop）。
         *
         *   大白话："把 strs 里的每个字符串，依次拿出来，叫它 s，然后执行循环体。"
         *
         *   逐段拆解：
         *
         *   for (...)    → 循环关键字
         *
         *   string& s    → 声明一个 string 类型的变量 s。
         *                   & 表示"引用"——不复制字符串内容，
         *                   直接引用原数组里的那个字符串。
         *                   不加 & 的话每次循环会复制整个字符串，浪费性能。
         *
         *   :            → 分隔符，读作"在...之中"
         *
         *   strs         → 我们要遍历的那个 vector<string>（传进来的参数）
         *
         *   和传统 for 循环的对比：
         *     for (string& s : strs)           ← 新写法，简洁
         *     等价于：
         *     for (int i = 0; i < strs.size(); ++i) {
         *         string& s = strs[i];         ← 老写法
         *         ...
         *     }
         *
         *   执行流程（输入 ["eat","tea","tan"]）：
         *     第1轮: s = "eat"
         *     第2轮: s = "tea"
         *     第3轮: s = "tan"
         *     遍历完自动结束。
         */
        for (string& s : strs) {

            /*
             * 【string key = s;】
             *   声明一个 string 变量 key，初始值 = s（复制一份当前单词）。
             *
             *   逐段拆解：
             *   string  → 变量类型：字符串
             *   key     → 变量名（自己起的，= "钥匙/身份证号"）
             *   =       → 赋值。把右边的值复制到左边的变量里。
             *   s       → 当前正在处理的单词
             *
             *   为什么复制一份？
             *     因为我们要对它排序，不能改变原来的单词。
             *     复制一份后，对 key 排序不会影响 s。
             *
             *   例：s = "eat" → key = "eat"（此时还一样）
             */
            string key = s;

            /*
             * 【sort(key.begin(), key.end());】
             *   对字符串 key 的字符进行排序。
             *
             *   逐段拆解：
             *
             *   sort  → <algorithm> 头文件提供的排序函数。
     *           它会把范围内的元素按从小到大排列。
     *           对于字符串来说，"从小到大"就是字母序（a < b < c < ...）
     *
     *   key.begin() → 返回一个"迭代器"，指向 key 的第一个字符。
     *                 迭代器 = 可以理解为"指向容器内某位置的指针/游标"。
     *                 我们暂时不需要深入理解迭代器，只需知道：
     *                 .begin() = 字符串开头
     *                 .end()   = 字符串结尾
     *
     *   key.end()   → 返回一个迭代器，指向 key 的"最后一个字符的后面"。
     *                 （不是最后一个字符，是最后一个字符再往后一格）
     *                 [e][a][t][ ]
     *                  ↑        ↑
     *                begin()  end()（结束哨兵）
     *
     *   执行示例：
     *     排序前 key = "eat" → 字符是 {'e', 'a', 't'}
     *     sort 把它们按字母序排好 → {'a', 'e', 't'}
     *     排序后 key = "aet"
     *
     *   注意：这行末尾没有等号，没有返回值。
     *         sort 函数直接修改 key 本身（原地排序）。
     */
            sort(key.begin(), key.end());

            /*
             * 【groups[key].push_back(s);】
             *   把原始单词 s 放进哈希表中 key 对应的组里。
             *
             *   逐段拆解：
             *
             *   groups     → 我们声明的哈希表
             *
             *   [key]      → 用 key 作为键访问哈希表。
             *                 [] 的用法和问题1的两数之和里一样。
             *                 如果 key 不存在，unordered_map 会自动创建
             *                 一个默认值（空的 vector<string>），然后返回它。
             *                 如果 key 已存在，直接返回对应的 vector<string>。
             *
             *                 [key] 的返回值是一个 vector<string>&（数组引用）。
             *
             *   .push_back(s) → vector 的成员函数，把 s 追加到数组的末尾。
             *                    push = 推入, back = 后面。
             *                    如：arr.push_back("hello") → 数组最后多一个 "hello"
             *
             *   整句话的效果（以 s="eat", key="aet" 为例）：
             *     找到（或创建）键 "aet" 对应的那个数组，
             *     把 "eat" 追加到这个数组末尾。
             *
             *   执行过程：
             *     第1轮: s="eat", key="aet" → groups["aet"].push_back("eat")
             *           → groups = { "aet": ["eat"] }
             *
             *     第2轮: s="tea", key="aet" → groups["aet"].push_back("tea")
             *           → groups = { "aet": ["eat", "tea"] }
             *
             *     第3轮: s="tan", key="ant" → groups["ant"].push_back("tan")
             *           → groups = { "aet": ["eat", "tea"], "ant": ["tan"] }
             *
             *     以此类推...
             */
            groups[key].push_back(s);
        }

        /*
         * 【vector<vector<string>> result;】
         *   声明一个二维字符串数组 result，用来存放最终结果。
         *
         *   类型：vector<vector<string>>
         *     → 外层 vector：装好几个"组"
         *     → 内层 vector<string>：每组里的单词
         *
         *   目前是空的：[]（没有任何组）
         */
        vector<vector<string>> result;

        /*
         * 【for (auto& pair : groups) { ... }】
         *   遍历哈希表 groups 里的每一对键值对。
         *
         *   逐段拆解：
         *
         *   auto&    → auto 的意思是"自动推导类型"，让编译器帮你看。
         *               groups 是一个 unordered_map<string, vector<string>>，
         *               里面的每个元素是一个 pair（键值对），具体类型是：
         *               pair<const string, vector<string>>
         *
         *               写 auto 就不用写上面那一大长串了。
         *               编译器看到 groups 就知道 auto = pair<const string, vector<string>>。
         *
         *               & 表示引用（不复制，直接引用原数据）。
         *
         *   pair     → 变量名（自己起的，= "一对"，即一个键值对）
         *
         *   : groups → 在 groups 中遍历
         *
         *   什么是 pair？
         *     pair = 一对值，把两个东西绑在一起。
         *     pair.first  = 键（key，本题中是排序后的字符串如 "aet"）
         *     pair.second = 值（value，本题中是一组单词如 ["eat","tea","ate"]）
         *
         *   为什么不直接用 first/second 而用 pair？
         *     你也可以写成 for (auto& kv : groups)，然后 kv.first / kv.second。
         *     名字随便起，关键是 .first 和 .second 是固定的。
         */
        for (auto& pair : groups) {

            /*
             * 【result.push_back(pair.second);】
             *   把当前这一组单词（pair.second）放进结果数组。
             *
             *   逐段拆解：
             *   result       → 最终结果数组（二维）
             *   .push_back() → 追加到数组末尾
             *   pair.second  → 取值：当前键值对的"值"部分
             *                   也就是一个 vector<string>（一组异位词）
             *
             *   例：pair = ("aet", ["eat","tea","ate"])
             *       pair.first  = "aet"      （键，没用到）
             *       pair.second = ["eat","tea","ate"]  （值，push进result）
             *
             *   每一轮往 result 里加一个组：
             *     第1轮: result = [["eat","tea","ate"]]
             *     第2轮: result = [["eat","tea","ate"], ["tan","nat"]]
             *     第3轮: result = [["eat","tea","ate"], ["tan","nat"], ["bat"]]
             */
            result.push_back(pair.second);
        }

        /*
         * 【return result;】
         *   return → 结束函数，把 result 返回给调用者。
         *   result 的类型是 vector<vector<string>>，和函数声明一致。
         */
        return result;
    }
};
// @lc code=end
