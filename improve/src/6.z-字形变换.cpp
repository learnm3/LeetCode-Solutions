/*
 * @lc app=leetcode.cn id=6 lang=cpp
 *
 * [6] Z 字形变换
 *
 * ===== 游戏客户端开发面试情境题 =====
 *
 * 【情境】技能轮盘 UI 的"蛇形"填充
 *
 * 你正在为动作游戏开发技能轮盘（Skill Wheel）。轮盘上有 4 排技能槽，
 * 总共有 N 个技能图标需要按特定顺序填入：从左上角开始，先向下填充第一列，
 * 到底后折返向上填充第二列，形成"之"字形（Zigzag）轨迹。
 *
 * 技能列表是按解锁顺序排列的：[火球, 冰箭, 闪电, 治疗, 护盾, 疾跑, ...]，
 * 你需要计算出它们在轮盘上的最终排列顺序（按行读取）。
 *
 * 例如 4 排轮盘，技能序列 "FIREBALL"：
 *
 *   列0  列1  列2
 *   F    A    L    ← 第0行
 *   I    B    L    ← 第1行
 *   R    空格 空格  ← 第2行（Z 字折返）
 *   E    空格 空格  ← 第3行
 *
 * 按行读出的顺序：F A L I B L R E → "FALIBLRE"
 *
 * 游戏开发中的类比场景：
 * - 装备栏排序：N 排装备槽，蛇形填入装备图标
 * - 动画关键帧序列：Z 形扫描关键帧用于特效插值
 * - 小地图瓦片：Z 形扫描将 2D 地图像素序列化成 1D 流
 *
 * 【题目】
 * 将一个给定字符串 s 根据给定的行数 numRows，以从上往下、从左到右进行
 * Z 字形排列，然后按行读出返回。
 *
 * 示例：
 *   输入: s = "PAYPALISHIRING", numRows = 3
 *   排列:
 *     P   A   H   N
 *     A P L S I I G
 *     Y   I   R
 *   按行输出: "PAHNAPLSIIGYIR"
 *
 *   输入: s = "PAYPALISHIRING", numRows = 4
 *   排列:
 *     P     I    N
 *     A   L S  I G
 *     Y A   H R
 *     P     I
 *   按行输出: "PINALSIGYAHRPI"
 *
 * ===== 核心思维 =====
 *
 * 一、模拟"之"字行走
 *
 * 想象你在一个 numRows 行的网格上行走，拿着字符串的字符依次放置：
 *   - 起点：第 0 行
 *   - 向下走：行号递增（i=0,1,2,...,numRows-1），每走一步放一个字符
 *   - 碰到底部（最后一行）→ 掉头向上走
 *   - 向上走：行号递减（i=numRows-2,...,1），每走一步放一个字符
 *   - 碰到顶部（第 0 行）→ 掉头向下走
 *   - 重复直到字符用完
 *
 * 用一个方向变量 flag 控制"向下(+1)还是向上(-1)"：
 *   flag = 1  → 向下走
 *   flag = -1 → 向上走
 *   当 i==0 或 i==numRows-1 时，flag 取反
 *
 * 二、逐行拼接
 *
 * 我们不需要真的建立一个 2D 网格。只需为每一行维护一个字符串，
 * 把字符追加到对应行的末尾。最后把所有行的字符串拼接起来即可。
 *
 *   初始化: rows = ["", "", "", ""]  (4行)
 *
 *   字符 'P' → i=0 → rows = ["P", "", "", ""]     i 向下(+1) → i=1
 *   字符 'A' → i=1 → rows = ["P", "A", "", ""]    i 向下(+1) → i=2
 *   字符 'Y' → i=2 → rows = ["P", "A", "Y", ""]   i 向下(+1) → i=3
 *   字符 'P' → i=3 → rows = ["P", "A", "Y", "P"]  碰底! flag 反转 → i=2
 *   字符 'A' → i=2 → rows = ["P", "A", "YA", "P"] i 向上(-1) → i=1
 *   ...
 *
 * 三、特殊情况
 *
 *   numRows == 1：直接返回原字符串（一行没有"之"字形可言）
 *
 * 四、复杂度分析
 *
 *   - 时间复杂度：O(n)，其中 n 是字符串长度。每个字符访问一次。
 *   - 空间复杂度：O(n)。存储每行的字符串总计 n 个字符。
 *
 * 五、可迁移模式 —— 方向标志变量控制往返运动
 *
 * 这个 flag 反转的技巧在很多游戏逻辑中用到：
 *   - 巡逻 AI：敌人在两个点之间来回移动
 *   - 菜单滚动：光标在列表中上下循环移动
 *   - 呼吸灯效果：亮度在 0~1 之间往返变化
 *   - 弹幕路径：弹幕在屏幕边缘之间反弹
 *
 * 通用模板：
 *   int i = 0, flag = 1; // 初始位置和方向
 *   for (auto item : items) {
 *       处理 item 在位置 i 的放置;
 *       if (i == 边界下界 || i == 边界上界) flag = -flag;
 *       i += flag;
 *   }
 */

#include <string>
#include <vector>
using namespace std;

// @lc code=start
class Solution {
public:
    string convert(string s, int numRows) {
        // 只有一行时，"之"字形退化为直线，直接返回
        if (numRows == 1) return s;

        // rows[i] 存储最终落在第 i 行的所有字符（按先后顺序）
        vector<string> rows(numRows);
        int i = 0;       // 当前行号
        int flag = -1;   // 方向：+1 向下，-1 向上。初始为 -1，第一次碰到 0 时会翻转为 +1

        for (char c : s) {
            rows[i] += c; // 将当前字符追加到对应行
            // 到达顶部或底部 → 反转方向
            if (i == 0 || i == numRows - 1) flag = -flag;
            i += flag;    // 移动到下一行
        }

        // 按行拼接结果
        string result;
        for (const string& row : rows) {
            result += row;
        }
        return result;
    }
};
// @lc code=end
