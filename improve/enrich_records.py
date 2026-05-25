#!/usr/bin/env python3
"""Generate detailed content for all 42 LeetCode problems and update Base records."""

import json
import subprocess
import time
import sys

# Base config
BASE_TOKEN = "SYTLbcUTRaPHxus7FFTcJ8z7nGb"
TABLE_ID = "tblmD0vBtdq8fqgy"

# Record IDs from the batch import result (in order of problem number)
RECORD_IDS = {
    2: "recvk5o65Pybrz", 3: "recvk5o65PxOb1", 4: "recvk5o65Pu0Fe",
    5: "recvk5o65Pmsxo", 6: "recvk5o65P432a", 7: "recvk5o65PbHPn",
    8: "recvk5o65P3Fvq", 9: "recvk5o65PFnwb", 10: "recvk5o65P1tdb",
    11: "recvk5o65PKxNt", 12: "recvk5o65PhJQP", 13: "recvk5o65PAgfy",
    14: "recvk5o65PWeTl", 15: "recvk5o65PvhUe", 16: "recvk5o65PMyDy",
    17: "recvk5o65PB0aN", 18: "recvk5o65Po8pJ", 19: "recvk5o65PLabq",
    20: "recvk5o65PL0Ul", 21: "recvk5o65Pomcv", 22: "recvk5o65PcF8K",
    23: "recvk5o65P6UbU", 24: "recvk5o65PPWuH", 25: "recvk5o65PgxGu",
    26: "recvk5o65PfIk1", 27: "recvk5o65P4FMr", 28: "recvk5o65PSehf",
    29: "recvk5o65PsmA6", 30: "recvk5o65P7qUY", 31: "recvk5o65PqqlK",
    32: "recvk5o65P8Awu", 33: "recvk5o65Pm33l", 34: "recvk5o65PVO8h",
    35: "recvk5o65P6BTP", 36: "recvk5o65PmQFs", 37: "recvk5o65Pl2ci",
    38: "recvk5o65PWiTR", 39: "recvk5o65PnbYp", 40: "recvk5o65PChjc",
    41: "recvk5o65Py52F", 42: "recvk5o65PKW54", 43: "recvk5o65PooZe",
}

# ── Content definitions ──
# Each entry: { 题目描述, 解题思路, 代码模板, 算法思维框架, 相似题目 }

data = {
    2: {
        "题目描述": "你是战斗系统开发者。主武器和副武器的各段伤害值以链表存储（低位在前，如 2→4→3 表示伤害值342）。需要合并两条伤害链表，计算总伤害用于飘字展示。\n注意：每位数字0-9，链表非空，数字不以0开头（除非就是0本身）。",
        "解题思路": "【核心】模拟竖式加法，处理进位。\n\n1. 暴力思路：还原整数→相加→再建链表 ❌ 会溢出（链表可表示超大数）\n2. 正确思路：同时遍历两条链表，逐位相加 + 进位carry\n   - dummy节点简化头节点处理\n   - 每次 sum = a->val + b->val + carry\n   - 新节点值 = sum % 10，carry = sum / 10\n   - 遍历完若carry=1，补一个值为1的节点\n3. 边界：两条链表长度不同时，短链表视为0\n4. 易错：忘记最后的进位节点",
        "代码模板": "ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {\n    ListNode dummy(0);  // 哑节点\n    ListNode* cur = &dummy;\n    int carry = 0;       // 进位\n    while (l1 || l2 || carry) {\n        int a = l1 ? l1->val : 0;\n        int b = l2 ? l2->val : 0;\n        int sum = a + b + carry;\n        carry = sum / 10;\n        cur->next = new ListNode(sum % 10);\n        cur = cur->next;\n        if (l1) l1 = l1->next;\n        if (l2) l2 = l2->next;\n    }\n    return dummy.next;\n}\n// 关键：dummy简化头节点 | while条件含carry | 短链表补0",
        "算法思维框架": "判断依据：两个链表 → 逐元素合并计算 → 进位传递\n链表选型：dummy节点是链表题的万能保险(避免空头判断)\n复杂度：O(max(m,n))时间 O(max(m,n))空间\n易错点：\n- ❌ 先转成整数再算 — 链表长度可达100位，64位整数溢出\n- ❌ 忘记最后carry=1时补节点\n- ❌ dummy.next返回到dummy本身",
        "相似题目": "#21 合并有序链表(同步遍历) | #445 两数相加II(高位在前→先反转) | #67 二进制求和(同思路但用字符串)"
    },
    3: {
        "题目描述": "你是格斗游戏的输入系统开发者。玩家输入序列是一串按键字符，需要找到最长的一段连续输入，其中没有任何按键重复出现。这用于验证连招的唯一性——连招内的每个按键都必须是独特的动作。\n输入：字符串 s（每个字符代表一个按键）\n输出：最长无重复按键子串的长度\n例：\"abcabcbb\" → 最长\"abc\"长度3；\"pwwkew\" → 最长\"wke\"长度3",
        "解题思路": "【核心】滑动窗口 = 右指针扩张纳入新元素 + 左指针收缩排除重复\n\n1. 暴力O(n²)：枚举每个起点，扫描到重复为止 ❌\n2. 滑动窗口O(n)：\n   - 维护窗口[l, r]，哈希表记录窗口内字符的最后出现位置\n   - r每步右移一位，纳入s[r]\n   - 若s[r]已在窗口内(map[s[r]]>=l)，l跳到map[s[r]]+1\n   - 更新map[s[r]]=r，ans=max(ans, r-l+1)\n3. 本质：l和r各走n步，每个字符进窗口一次出窗口一次\n4. 易错：l只能右移不能左退(所以用max(l, map[s[r]]+1))",
        "代码模板": "int lengthOfLongestSubstring(string s) {\n    unordered_map<char, int> pos; // 字符→最后出现位置\n    int ans = 0;\n    for (int l = 0, r = 0; r < s.size(); r++) {\n        if (pos.count(s[r]) && pos[s[r]] >= l) {\n            l = pos[s[r]] + 1;  // 跳到重复字符的下一位\n        }\n        pos[s[r]] = r;\n        ans = max(ans, r - l + 1);\n    }\n    return ans;\n}\n// 变体：若字符集只有ASCII，用int[128]代替map更快",
        "算法思维框架": "触发条件：\n- 看到\"子串\"+\"最长/最短\"+\"无重复/包含所有\" → 滑动窗口\n- 输入是序列（字符串或数组）\n- 需要窗口内元素的某种计数/状态\n\n思维链路：\n1. 确定窗口内容含义（本题：无重复字符的区间）\n2. 设计扩张条件（r无条件右移）\n3. 设计收缩条件（出现重复时l右移）\n4. 何时更新答案（每次扩张后）\n\n易错点：\n- ❌ 窗口收缩后忘记更新哈希表\n- ❌ l=pos[s[r]]+1写成l=pos[s[r]]（差一位）\n- ❌ 用set而非map，找不到重复位置",
        "相似题目": "#159 至多包含两个不同字符的最长子串 | #30 串联所有单词的子串(多词滑动窗口)"
    },
    4: {
        "题目描述": "你是匹配系统的开发者。两个排位池(青铜池和白银池)各自的玩家MMR已排序。需要快速找到合并后所有玩家的中位数MMR，用于将玩家公平地分配到红蓝两队。\n要求时间复杂度O(log(min(m,n)))。\n\n例：nums1=[1,3], nums2=[2] → 合并[1,2,3]中位数2.0\n    nums1=[1,2], nums2=[3,4] → 合并[1,2,3,4]中位数(2+3)/2=2.5",
        "解题思路": "【核心】二分较短数组的切分位置，使左半部分最大值≤右半部分最小值\n\n1. 暴力O(m+n)：合并两个数组取中位数 ❌ 不满足log要求\n2. 二分O(log(min(m,n)))：\n   - 将问题转化为：在A的i处和B的j=(m+n+1)/2-i处切分\n   - 需满足：A[i-1]≤B[j] 且 B[j-1]≤A[i]\n   - 在较短数组上二分i，根据条件调整区间\n3. 边界处理：i=0或i=m时视为-∞/+∞\n4. 奇偶处理：总数为奇→左半最大值；总数为偶→(左半最大+右半最小)/2\n5. 关键：partition的语义——左半部分始终比右半部分多一个元素（应对奇数情况）",
        "代码模板": "double findMedianSortedArrays(vector<int>& A, vector<int>& B) {\n    if (A.size() > B.size()) swap(A, B); // A是较短数组\n    int m = A.size(), n = B.size();\n    int half = (m + n + 1) / 2;\n    int l = 0, r = m;\n    while (l <= r) {\n        int i = l + (r - l) / 2;   // A的切分位置\n        int j = half - i;           // B的切分位置\n        int A_left = (i == 0) ? INT_MIN : A[i-1];\n        int A_right = (i == m) ? INT_MAX : A[i];\n        int B_left = (j == 0) ? INT_MIN : B[j-1];\n        int B_right = (j == n) ? INT_MAX : B[j];\n        if (A_left <= B_right && B_left <= A_right) {\n            if ((m + n) % 2) return max(A_left, B_left);\n            return (max(A_left, B_left) + min(A_right, B_right)) / 2.0;\n        }\n        if (A_left > B_right) r = i - 1;\n        else l = i + 1;\n    }\n    return 0.0;\n}\n// 关键：在较短数组上二分保证log(min(m,n)) | 边界用INT_MIN/MAX",
        "算法思维框架": "判断依据：两个有序数组 + 找第k小/中位数 + O(log)要求\n\n核心转换：不直接找中位数，而是找\"切割位置\"使左右均衡\n\n复杂度口诀：二分的数组选短的 → log(min(m,n))\n\n易错点：\n- ❌ 没swap让A是较短数组 → 复杂度变成log(max(m,n))\n- ❌ 边界i=0/m时忘记判空 → 数组越界\n- ❌ 奇数偶数情况混在一起处理\n- ❌ half=(m+n+1)/2的分母错误",
        "相似题目": "#33 搜索旋转排序数组(变体二分) | #34 找首末位置(两次二分) | #35 搜索插入位置(标准二分)"
    },
    5: {
        "题目描述": "你是镜像谜题关卡的设计者。玩家的移动序列是一串方向字符，在镜像关卡中，有效的移动序列必须是回文的——正着读和反着读完全一样（如\"左右右左\"）。给定一个移动序列字符串，找到其中最长的回文子序列（不需要连续，类似最长回文子串）。\n\n示例：\"babad\" → \"bab\"或\"aba\"均可，返回长度即可",
        "解题思路": "【核心】中心扩展法：以每个字符(或间隙)为中心，向两侧扩展找最长回文\n\n1. 暴力O(n³)：枚举所有子串+判断回文 ❌\n2. 中心扩展O(n²)：\n   - 回文有两种中心：单字符(\"aba\"以b为中心) 和 双字符间隙(\"abba\"以bb间隙为中心)\n   - 对每个中心，while(left==right) left--; right++\n   - 记录最长长度及起始位置\n3. DP也可O(n²)：dp[i][j]=s[i]==s[j] && dp[i+1][j-1]\n4. 实战建议：中心扩展法空间O(1)更优，DP更容易理解和变体\n5. 易错：回文有两种中心类型，容易漏掉双字符中心",
        "代码模板": "string longestPalindrome(string s) {\n    int start = 0, maxlen = 0;\n    auto expand = [&](int l, int r) {\n        while (l >= 0 && r < s.size() && s[l] == s[r]) {\n            l--; r++;\n        }\n        int len = r - l - 1;\n        if (len > maxlen) { maxlen = len; start = l + 1; }\n    };\n    for (int i = 0; i < s.size(); i++) {\n        expand(i, i);     // 奇数长度回文\n        expand(i, i + 1); // 偶数长度回文\n    }\n    return s.substr(start, maxlen);\n}\n// 关键：两种中心都要展开 | r-l-1因为while多移了一步",
        "算法思维框架": "判断依据：回文/对称 + 最长/计数 → 中心扩展 或 DP\n\n中心扩展 = 遍历中心点 + 双指针向两侧扩散\nDP = 区间dp[i][j]依赖dp[i+1][j-1]\n\n选择策略：\n- 只求最长长度→中心扩展就够了\n- 需要计数/统计/打印所有→DP\n\n复杂度：O(n²)时间 O(1)空间(中心扩展) O(n²)空间(DP)\n\n易错点：\n- ❌ 只处理奇数长度回文（只expand(i,i)）→ 漏掉\"abba\"\n- ❌ expand后len计算错误（r-l+1 vs r-l-1）\n- ❌ DP遍历顺序错误（必须从短到长,i从n-1到0,j从i到n-1）",
        "相似题目": "#647 回文子串计数(同思路统计数量) | #516 最长回文子序列(DP) | #131 分割回文串(回溯+回文判断)"
    },
    6: {
        "题目描述": "你是魔法咒语特效的美术程序员。对话文本需要沿波浪路径渲染成Z字形，用于RPG中魔法咒语的吟唱特效。给定字符串和行数，按Z字形排列后逐行读取输出。\n\n例：s=\"PAYPALISHIRING\", numRows=4\nP     I    N\nA   L S  I G\nY A   H R\nP     I\n→ \"PINALSIGYAHRPI\"",
        "解题思路": "【核心】模拟Z字形填写过程，使用行索引在0和numRows-1之间往返\n\n1. 暴力思路：创建二维矩阵填字符 ❌ 浪费空间\n2. 优化O(n)：\n   - 使用numRows个字符串builder存储每行\n   - 维护row和step：row向下走(+1)，触底则step=-1向上走\n   - row=0时step=1(向下)，row=numRows-1时step=-1(向上)\n3. 边界：numRows=1直接返回原串\n4. 本质：找规律——每个Z周期长度为2*numRows-2",
        "代码模板": "string convert(string s, int numRows) {\n    if (numRows == 1) return s;\n    vector<string> rows(min(numRows, (int)s.size()));\n    int row = 0;\n    int step = 1;  // 1=向下, -1=向上\n    for (char c : s) {\n        rows[row] += c;\n        if (row == 0) step = 1;           // 触顶→向下\n        else if (row == numRows - 1) step = -1; // 触底→向上\n        row += step;\n    }\n    string ans;\n    for (auto& r : rows) ans += r;\n    return ans;\n}\n// 关键：step控制方向反转 | numRows=1特殊处理",
        "算法思维框架": "判断依据：字符串 + 特殊排列规则 → 模拟索引变化\n\n复杂度的关键：不要真创建矩阵，用string builder直接拼接\n\n易错点：\n- ❌ numRows=1时step逻辑死循环\n- ❌ row的边界判断顺序（先触顶还是先触底）\n- ❌ 忘记rows.resize（当s比numRows短时）",
        "相似题目": "#54 螺旋矩阵(同模拟方向变化) | #59 螺旋矩阵II"
    },
    7: {
        "题目描述": "你是含时间操纵机制的谜题游戏开发者。玩家可以反转时间值（一个32位有符号整数）。但反转后的时间值必须在游戏的有效时间范围内（[-2³¹, 2³¹-1]即int范围）。如果反转后溢出，返回0表示触发时间悖论保护。\n\n例：123→321；-123→-321；120→21",
        "解题思路": "【核心】逐位取模+重建，溢出前检测\n\n1. 循环取出x的最后一位digit=x%10，x/=10\n2. ans = ans*10 + digit 逐步重建反转数\n3. 溢出检测：在ans*10之前判断：\n   if (ans > INT_MAX/10 || (ans == INT_MAX/10 && digit > 7)) return 0;\n   if (ans < INT_MIN/10 || (ans == INT_MIN/10 && digit < -8)) return 0;\n4. 不需要单独处理符号，负数取模在C++中结果也是负的（C++11起）",
        "代码模板": "int reverse(int x) {\n    int ans = 0;\n    while (x != 0) {\n        int digit = x % 10;\n        x /= 10;\n        // 溢出预判\n        if (ans > INT_MAX / 10 || (ans == INT_MAX / 10 && digit > 7))\n            return 0;\n        if (ans < INT_MIN / 10 || (ans == INT_MIN / 10 && digit < -8))\n            return 0;\n        ans = ans * 10 + digit;\n    }\n    return ans;\n}\n// 关键：溢出检测在乘法之前 | INT_MAX末位7 INT_MIN末位-8",
        "算法思维框架": "判断依据：数字反转 + 有范围限制 → 逐位提取 + 预判溢出\n\n核心技巧：在 ans*10 之前做溢出判断，而不是等溢出后再检测\n\n易错点：\n- ❌ 用long long暂存绕过溢出 → 面试中可能被禁\n- ❌ 忘记INT_MAX末位是7不是9\n- ❌ C++中负数%10的结果是负的（-123%10=-3）",
        "相似题目": "#8 字符串转整数(同溢出处理模式) | #9 回文数(反转一半比较)"
    },
    8: {
        "题目描述": "你是游戏控制台的开发者。玩家可以在控制台输入命令来修改游戏参数，如 '/set gravity 9.8'。你需要安全地将命令参数字符串解析为整数，处理前导空格、正负号、非法字符和溢出。\n\n规则：1.丢弃前导空格 2.可选±号 3.读取数字直到非数字字符 4.溢出则返回INT_MAX或INT_MIN",
        "解题思路": "【核心】状态机解析：跳过空格→处理符号→逐位读数字→溢出检测\n\n1. 跳过前导空格：while(i<n && s[i]==' ') i++\n2. 处理符号：检查s[i]是否为'+'或'-'，记录sign\n3. 逐位解析：while(i<n && isdigit(s[i]))\n   - digit = s[i] - '0'\n   - 溢出预判（同#7的检测逻辑）\n   - ans = ans*10 + digit * sign …不对，应该先算正数再乘符号\n4. 边界：空串、纯空格、纯符号后无数字的处理",
        "代码模板": "int myAtoi(string s) {\n    int i = 0, n = s.size();\n    while (i < n && s[i] == ' ') i++;  // 跳空格\n    if (i == n) return 0;\n    int sign = 1;\n    if (s[i] == '+' || s[i] == '-') {\n        sign = (s[i] == '-') ? -1 : 1;\n        i++;\n    }\n    long ans = 0;  // 用long简化溢出处理\n    while (i < n && isdigit(s[i])) {\n        ans = ans * 10 + (s[i] - '0');\n        if (ans * sign > INT_MAX) return INT_MAX;\n        if (ans * sign < INT_MIN) return INT_MIN;\n        i++;\n    }\n    return (int)(ans * sign);\n}\n// 简化版用了long；面试严格版参考#7的预判写法",
        "算法思维框架": "判断依据：字符串→数字 + 需处理边界和异常\n\n标准流程：去空白 → 判符号 → 逐位解析 → 溢出检测\n\n易错点：\n- ❌ 忘记处理前导空格\n- ❌ \"+-12\"应返回0（连续两个符号非法）\n- ❌ 溢出后应返回边界值而非0\n- ❌ 纯空白串应返回0",
        "相似题目": "#7 整数反转(同溢出检测) | #65 有效数字(更复杂的状态机解析)"
    },
    9: {
        "题目描述": "你是镜像维度系统的开发者。在镜像世界机制中，玩家只能在回文数坐标点进行维度穿越——一个数字正向读和反向读相同即为回文数。判断给定的维度坐标x是否允许穿越。\n要求：不能将整数转为字符串。\n\n例：121→true（从左读和从右读都是121）；-121→false（负号破坏对称）",
        "解题思路": "【核心】反转一半数字，比较前后半是否相等\n\n1. 负数直接false，末尾为0的非零数直接false（如10→反转后是1）\n2. 反转一半：while(x > half)\n   half = half * 10 + x % 10\n   x /= 10\n3. 循环结束：x == half（偶数位）或 x == half/10（奇数位去掉中间位）\n4. 为什么反转一半更快：O(log₁₀(n)/2)且不会溢出（只反转到一半）",
        "代码模板": "bool isPalindrome(int x) {\n    if (x < 0 || (x % 10 == 0 && x != 0)) return false;\n    int half = 0;\n    while (x > half) {\n        half = half * 10 + x % 10;\n        x /= 10;\n    }\n    return x == half || x == half / 10; // 偶数位/奇数位\n}\n// 关键：反转一半避免溢出 | 10的倍数特判 | x>half是停止条件",
        "算法思维框架": "判断依据：回文判断 + 不能转字符串 → 反转一半数字\n\n核心洞察：只需反转到x<=half，比较前后是否相同\n\n易错点：\n- ❌ 10的倍数（100, 1000）会被误判 → 需要用(x%10==0 && x!=0)过滤\n- ❌ 奇数位忘记除以10去掉中间位\n- ❌ 循环条件写成x>=half导致多反转一位",
        "相似题目": "#7 整数反转(完整反转+溢出) | #234 回文链表(快慢指针找中点+反转后半)"
    },
    10: {
        "题目描述": "你是聊天过滤系统的开发者。需要实现一个支持通配符的模式匹配引擎：'.' 匹配任意单个字符，'*' 匹配零个或多个前面的字符。这用于过滤玩家聊天中的敏感词模式，也用于游戏控制台命令的模式匹配。\n\n例：s=\"aab\", p=\"c*a*b\" → true（c*匹配0个c，a*匹配两个a，b匹配b）\n    s=\"mississippi\", p=\"mis*is*p*.\" → false",
        "解题思路": "【核心】DP: dp[i][j] = s的前i个字符能否被p的前j个字符匹配\n\n1. 初始化：dp[0][0]=true; dp[0][j]=dp[0][j-2]（当p[j-1]=='*') 因为*可以消掉前一字符\n2. 转移：\n   - p[j-1] ≠ '*'：dp[i][j] = dp[i-1][j-1] && match(s[i-1], p[j-1])\n   - p[j-1] = '*'：\n     a. 匹配0次：dp[i][j] = dp[i][j-2]\n     b. 匹配≥1次：dp[i][j] = dp[i-1][j] && match(s[i-1], p[j-2])\n3. 时间复杂度O(mn)空间可优化到O(n)但面试写二维更清晰",
        "代码模板": "bool isMatch(string s, string p) {\n    int m = s.size(), n = p.size();\n    vector<vector<bool>> dp(m+1, vector<bool>(n+1, false));\n    dp[0][0] = true;\n    // 初始化：a*b*c*...可匹配空串\n    for (int j = 2; j <= n; j += 2)\n        if (p[j-1] == '*') dp[0][j] = dp[0][j-2];\n        else break;\n    for (int i = 1; i <= m; i++) {\n        for (int j = 1; j <= n; j++) {\n            if (p[j-1] != '*') {\n                dp[i][j] = dp[i-1][j-1] &&\n                    (s[i-1] == p[j-1] || p[j-1] == '.');\n            } else {\n                dp[i][j] = dp[i][j-2]; // 匹配0次\n                if (s[i-1] == p[j-2] || p[j-2] == '.')\n                    dp[i][j] = dp[i][j] || dp[i-1][j];\n            }\n        }\n    }\n    return dp[m][n];\n}\n// 关键：*的处理分两种情况 | 初始化处理空串匹配",
        "算法思维框架": "判断依据：字符串匹配 + 通配符 + 需要回溯 → DP\n\ndp[i][j]通常表示\"...的前i个...的前j个\"是否可行\n\n核心技巧：\n- dp维度比字符串长度多1（空串状态）\n- 遇到*时考虑用0次和用1次两种情况\n\n易错点：\n- ❌ 忘记初始化dp[0][j]（空串可被a*b*c*匹配）\n- ❌ *的匹配逻辑混淆（是匹配p[j-2]不是p[j-1]）\n- ❌ 下标混乱：dp的ij vs s/p的索引差1",
        "相似题目": "#44 通配符匹配('?'和'*'，更简单) | #32 最长有效括号(DP)"
    },
}

# Content for remaining problems... (too large to fit in one file, split into batches)
print("Content for problems 2-10 defined. Need to extend for 11-43.")
print(f"Base token: {BASE_TOKEN}, Table: {TABLE_ID}")
print("Record IDs loaded for problems:", sorted(RECORD_IDS.keys()))

# Verify we have all 42 record IDs
print(f"Total record IDs: {len(RECORD_IDS)}")
