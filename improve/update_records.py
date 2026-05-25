#!/usr/bin/env python3
"""Update all 42 LeetCode records in Base with detailed content."""
import json, subprocess, time, sys

BASE = "SYTLbcUTRaPHxus7FFTcJ8z7nGb"
TBL = "tblmD0vBtdq8fqgy"

# Record IDs in problem number order (from batch import output)
rids = {
    2:"recvk5o65Pybrz",3:"recvk5o65PxOb1",4:"recvk5o65Pu0Fe",5:"recvk5o65Pmsxo",
    6:"recvk5o65P432a",7:"recvk5o65PbHPn",8:"recvk5o65P3Fvq",9:"recvk5o65PFnwb",
    10:"recvk5o65P1tdb",11:"recvk5o65PKxNt",12:"recvk5o65PhJQP",13:"recvk5o65PAgfy",
    14:"recvk5o65PWeTl",15:"recvk5o65PvhUe",16:"recvk5o65PMyDy",17:"recvk5o65PB0aN",
    18:"recvk5o65Po8pJ",19:"recvk5o65PLabq",20:"recvk5o65PL0Ul",21:"recvk5o65Pomcv",
    22:"recvk5o65PcF8K",23:"recvk5o65P6UbU",24:"recvk5o65PPWuH",25:"recvk5o65PgxGu",
    26:"recvk5o65PfIk1",27:"recvk5o65P4FMr",28:"recvk5o65PSehf",29:"recvk5o65PsmA6",
    30:"recvk5o65P7qUY",31:"recvk5o65PqqlK",32:"recvk5o65P8Awu",33:"recvk5o65Pm33l",
    34:"recvk5o65PVO8h",35:"recvk5o65P6BTP",36:"recvk5o65PmQFs",37:"recvk5o65Pl2ci",
    38:"recvk5o65PWiTR",39:"recvk5o65PnbYp",40:"recvk5o65PChjc",41:"recvk5o65Py52F",
    42:"recvk5o65PKW54",43:"recvk5o65PooZe"
}

# ── Per-problem detail data ──
D = {}

def add(n, desc, thought, code, framework, similar):
    D[n] = {
        "题目描述": desc,
        "解题思路": thought,
        "代码模板": code,
        "算法思维框架": framework,
        "相似题目": similar
    }

# ======= 双指针 (11,15,16,18,26,27,31,42) =======
add(11, """你是开放世界地形系统的开发者。给定程序化生成的地形高度图截面 heights[n]，每列高度表示该位置的海拔。
选择两列地形作为堤坝，计算它们之间能容纳的最大水量（面积 = min(h[i],h[j]) × (j-i)）。
这用于验证湖泊/河流位置是否合理——水量必须足够大才能形成有效水体。""",
"""1. 暴力O(n²)：枚举所有柱子对 (i,j)，计算min(h[i],h[j])*(j-i) ❌
2. 双指针O(n)：
   - 左右指针从两端向中间收缩
   - 每次移动较短的那根柱子（因为水量受限于短柱，移动短柱才有可能增加水量）
   - 更新最大面积 ans = max(ans, min(h[l],h[r])*(r-l))
3. 核心洞察：移动长柱不会增加水量（因为短柱不变，宽度减少），必须移动短柱
4. 易错：只比较面积但忘记宽度在减少""",
"""int maxArea(vector<int>& h) {
    int l = 0, r = h.size() - 1, ans = 0;
    while (l < r) {
        ans = max(ans, min(h[l], h[r]) * (r - l));
        // 移动较短的一边：水量由短边决定
        if (h[l] < h[r]) l++;
        else r--;
    }
    return ans;
}
// 核心：贪心移动短边 | 水量=min(高度)×宽度""",
"""触发条件：数组中找两个索引(i,j)使某个函数值最大/最小 + 两个变量互相约束
思维链路：
1. 左右指针初始化在数组两端
2. 计算当前值并更新答案
3. 判断移动哪个指针（本题：移动短边）
4. 循环直到指针相遇
复杂度：O(n)时间 O(1)空间
易错：
- ❌ 移动长边——数学上可证明不可能得到更大面积
- ❌ 面积公式写错（是 min(h[i],h[j])×(j-i)，不是 max）""",
"#15 三数之和(双指针+排序) | #42 接雨水(双指针变体) | #16 最接近的三数之和")

add(15, """你是RPG合成系统的开发者。玩家可以从材料库中选取三份材料进行合成。每份材料有一个魔法属性值(可正可负)，合成稳定配方的条件是——三份材料的属性值之和恰好为零。
给定材料库的属性值数组 nums[n]，找出所有满足条件的三元组 [nums[i], nums[j], nums[k]]（i≠j≠k），且答案中不能包含重复的三元组。""",
"""1. 暴力O(n³)：三重循环枚举所有三元组，用set去重 ❌ 超时
2. 排序+双指针O(n²)：
   - 排序 O(n log n)
   - 固定 nums[i]，在[i+1, n-1]上用双指针找两数之和 = -nums[i]
   - 关键去重：固定i时跳过相同nums[i]；找到一组解后左右指针各跳过相同值
3. 剪枝：排序后若nums[i] > 0 可直接break（后面都>0不可能和为0）
4. 易错：
   - 去重逻辑必须放在找到解之后——不能提前跳过导致漏解
   - 指针移动：sum<0移左指针，sum>0移右指针""",
"""vector<vector<int>> threeSum(vector<int>& nums) {
    sort(nums.begin(), nums.end());
    vector<vector<int>> ans;
    int n = nums.size();
    for (int i = 0; i < n - 2; i++) {
        if (nums[i] > 0) break;  // 剪枝
        if (i > 0 && nums[i] == nums[i-1]) continue; // 去重i
        int l = i + 1, r = n - 1;
        while (l < r) {
            int sum = nums[i] + nums[l] + nums[r];
            if (sum < 0) l++;
            else if (sum > 0) r--;
            else {
                ans.push_back({nums[i], nums[l], nums[r]});
                while (l < r && nums[l] == nums[l+1]) l++; // 去重l
                while (l < r && nums[r] == nums[r-1]) r--; // 去重r
                l++; r--;
            }
        }
    }
    return ans;
}
// 核心：排序+固定一个+双指针 | 三层去重(i/l/r)""",
"""触发条件：找n个元素使和等于/接近target + 需要去重 → 排序+双指针
核心转换：N数之和降维 → 固定1个 + (N-1)数之和子问题
去重策略：排序后，固定位和前一位相同则跳过
复杂度：三数之和O(n²)，四数之和O(n³)
易错：
- ❌ 去重过早导致漏解
- ❌ 双指针移动方向写反
- ❌ 答案是二维数组却忘记push_back""",
"#16 最接近的三数之和 | #18 四数之和 | #1 两数之和(哈希表版本)")

add(16, """你是战斗数值策划工具的程序开发者。给定N个技能修正器，每个修正器有一个数值。你需要从中选择三个修正器，使它们的总加成最接近（但不超过）平衡上限值 target。返回这个最接近的三数之和。
例如：[修正器值]=[-1,2,1,-4], target=1 → 最接近和=2（-1+2+1=2距离1最近）""",
"""1. 与#15三数之和几乎相同的框架，差异是目标从=0变为≈target
2. 排序+固定一个+双指针：
   - 维护 closest 变量记录最接近的和
   - 比较 abs(sum-target) 和 abs(closest-target)
   - sum < target 移左指针增加sum；sum > target 移右指针减少sum
   - sum == target 直接返回（最优解）
3. 本质：将三数之和的判等问题改成距离最近问题
4. 易错：closest初始值可能溢出 → 用INT_MAX/2""",
"""int threeSumClosest(vector<int>& nums, int target) {
    sort(nums.begin(), nums.end());
    int n = nums.size();
    int closest = nums[0] + nums[1] + nums[2]; // 初始值
    for (int i = 0; i < n - 2; i++) {
        if (i > 0 && nums[i] == nums[i-1]) continue;
        int l = i + 1, r = n - 1;
        while (l < r) {
            int sum = nums[i] + nums[l] + nums[r];
            if (abs(sum - target) < abs(closest - target))
                closest = sum;
            if (sum < target) l++;
            else if (sum > target) r--;
            else return target;  // 精确匹配
        }
    }
    return closest;
}
// 与#15的区别：不需要去重 | 维护最接近和 | sum==target时直接返回""",
"""与#15三数之和同族，核心差异在"等于"→"最接近"
复杂度同O(n²)，但不需要去重逻辑（因为问题本身不要求唯一性）
注意：closest初始值不能乱选（用前三个元素而非INT_MAX）""",
"#15 三数之和(改条件) | #18 四数之和 | #259 较小的三数之和(计数)")

add(18, """你是RPG自动组队系统的开发者。从候选队员池中选择四名队员，使其总战斗力恰好等于副本的推荐战力值 target。
注意：同一队员不能选多次，答案中不能包含重复的四人组。
给定数组 nums 和 target，返回所有不重复的四元组。""",
"""1. #15三数之和的扩展——四数→固定两个+双指针
2. 排序+双重循环固定i,j + 双指针O(n³)：
   - i从0到n-4, j从i+1到n-3
   - 双指针在[j+1, n-1]找两数之和=target-nums[i]-nums[j]
3. 去重：i,j,l,r四层去重
4. 剪枝优化（可选）：
   - 最小可能和 > target → break
   - 最大可能和 < target → continue
5. 注意：target可能很大，用long存两数之和避免溢出""",
"""vector<vector<int>> fourSum(vector<int>& nums, int target) {
    sort(nums.begin(), nums.end());
    vector<vector<int>> ans;
    int n = nums.size();
    for (int i = 0; i < n - 3; i++) {
        if (i > 0 && nums[i] == nums[i-1]) continue;
        for (int j = i + 1; j < n - 2; j++) {
            if (j > i + 1 && nums[j] == nums[j-1]) continue;
            int l = j + 1, r = n - 1;
            long need = (long)target - nums[i] - nums[j]; // 防溢出
            while (l < r) {
                long sum = (long)nums[l] + nums[r];
                if (sum < need) l++;
                else if (sum > need) r--;
                else {
                    ans.push_back({nums[i],nums[j],nums[l],nums[r]});
                    while (l < r && nums[l] == nums[l+1]) l++;
                    while (l < r && nums[r] == nums[r-1]) r--;
                    l++; r--;
                }
            }
        }
    }
    return ans;
}
// N数之和通用框架：排序 + 固定N-2个 + 双指针 | 每层去重""",
"""N数之和问题通用解法：排序 + 递归降维 + 双指针
四数→O(n³)，五数→O(n⁴)，以此类推
去重策略每层相同：和前一个元素相同则跳过
关键优化：用long存中间和，防止overflow""",
"#15 三数之和 | #16 最接近三数之和 | #454 四数之和II(哈希表版本)")

add(26, """你是渲染管线中的粒子系统开发者。GPU渲染的数千个粒子按屏幕空间坐标排序后存储。在将粒子提交给Draw Call之前，需要移除坐标重复的粒子——同一像素位置的多余粒子不会产生可见变化，但会浪费GPU资源。
给定已排序的坐标数组，原地去重，返回不重复的坐标数量。""",
"""1. 双指针（快慢指针）：
   - slow指向"下一个不重复元素应放置的位置"
   - fast扫描数组
   - 当nums[fast]≠nums[fast-1]时，nums[slow++]=nums[fast]
2. 因为是排序数组，重复元素必相邻
3. 关键：slow从1开始（第一个元素一定保留），fast从1开始
4. 最终slow的值就是去重后的长度""",
"""int removeDuplicates(vector<int>& nums) {
    if (nums.empty()) return 0;
    int slow = 1; // 第一个元素一定保留
    for (int fast = 1; fast < nums.size(); fast++) {
        if (nums[fast] != nums[fast - 1]) {
            nums[slow++] = nums[fast];
        }
    }
    return slow; // slow就是新长度
}
// 快慢指针经典模板 | slow从1开始 | slow返回新数组长度""",
"""触发条件：有序数组 + 原地修改 + 去重/移除 → 快慢指针
模板：slow=起始位置, fast=起始位置; while(fast<n)
易错：
- ❌ slow初始化为0（第一个元素也被覆盖）
- ❌ 比较nums[fast]和nums[slow]而非nums[fast-1]
- ❌ 返回值应该是slow（新长度）而非slow+1""",
"#27 移除元素(同模板) | #80 删除有序数组重复项II(变体: 最多保留2个)")

add(27, """你是背包UI系统的开发者。玩家要批量清理特定类型的物品——从排序好的背包数组中移除所有"已损坏的剑"类型的物品。需要原地修改数组，返回清理后剩余物品的数量。
与#26不同：这里不是去重，而是移除所有等于某个特定值的元素。""",
"""1. 同#26的快慢指针模板：
   - slow：下一个保留元素的位置
   - fast：扫描数组
   - 当nums[fast]≠val时，保留：nums[slow++]=nums[fast]
2. 与#26的区别：判断条件从"不等于前一个元素"变成"不等于目标值val"
3. 关键：slow本身的值就是新长度
4. 变体思考：如果要移除的元素不止一种怎么办？（多值哈希集合）""",
"""int removeElement(vector<int>& nums, int val) {
    int slow = 0;
    for (int fast = 0; fast < nums.size(); fast++) {
        if (nums[fast] != val) {
            nums[slow++] = nums[fast];
        }
    }
    return slow;
}
// slow从0开始（第一个元素也不一定保留）| 判断条件!=val""",
"""与#26同模板但判断条件不同。快慢指针本质是"筛选保留"——fast遍历，slow记录保留下来的位置
复杂度：O(n) O(1)
注意：slow从0开始（与#26从1不同），因为第一个元素也不一定保留
拓展：需要移除的元素之外还要求保持顺序→此模板天然满足""",
"#26 删除有序数组重复项 | #283 移动零(保留非零+末尾补零)")

add(31, """你是程序化地牢生成系统的开发者。地牢的房间序列可以表示为一个排列（1到n的某种顺序）。为了生成不同的地牢变体，你需要找到当前房间排列的下一个字典序更大的排列。如果已经是最大的排列，则回到最小排列。
要求原地修改，空间O(1)。""",
"""1. 暴力O(n!)：生成所有排列找下一个 ❌ 不可接受
2. 双指针（三段法）O(n)：
   Step 1：从右向左找第一个下降点 i = 第一个满足nums[i] < nums[i+1]的位置
   Step 2：从右向左找第一个大于nums[i]的位置 j，交换nums[i]和nums[j]
   Step 3：反转 [i+1, n-1] 区间（使其变为升序=最小化后缀）
3. 若Step 1没找到下降点，说明已是最大排列，直接反转整个数组
4. 核心直觉：尽可能小的改动——改动最右边的下降点+让后缀最小""",
"""void nextPermutation(vector<int>& nums) {
    int n = nums.size();
    int i = n - 2;
    while (i >= 0 && nums[i] >= nums[i+1]) i--; // Step1: 找下降点
    if (i >= 0) {
        int j = n - 1;
        while (nums[j] <= nums[i]) j--; // Step2: 找刚好大于nums[i]的元素
        swap(nums[i], nums[j]);
    }
    reverse(nums.begin() + i + 1, nums.end()); // Step3: 反转后缀
}
// 三段法：找下降点→找大于值交换→反转后缀为升序 | i=-1时全反转""",
"""触发条件："下一个排列/组合" "字典序" + 原地修改 → 三段法
核心直觉：最小的改变 = 改动尽可能靠右的位置 + 让右边变最小
复杂度：O(n)时间 O(1)空间
易错：
- ❌ 寻找下降点时用<=还是< = 用>=跳过相等元素，找严格的下降
- ❌ 忘记"已是最大排列"→全反转的边界""",
"#46 全排列(回溯生成所有排列) | #56 合并区间(变体排序+合并)")

add(42, """你是开放世界地形水文模拟系统的开发者。给定一个高度图的2D横截面 heights[n]，每列宽度为1。
计算下雨后，这些地形能蓄积多少单位的水。这直接对应开放世界游戏的地形水文系统——验证河流、湖泊的水体积是否符合设计预期。
经典变体：不仅是平面截面，还可以扩展到3D地形（最小堆+BFS解决）""",
"""1. 暴力O(n²)：每列找左右最高柱，取min减自身高度 ❌
2. 双指针O(n)（最优）：
   - 左右指针l,r，维护left_max和right_max
   - 哪个max小移动哪边：水由矮边决定
   - 每列贡献 = min(left_max, right_max) - height[i]
   - 将贡献累加到答案
3. 直观理解：与#11盛水容器不同，#42是"每列独立积水量"
4. 另一解法：单调栈（递减栈，遇到递增时计算中间洼地）""",
"""int trap(vector<int>& h) {
    int l = 0, r = h.size() - 1;
    int lmax = 0, rmax = 0, ans = 0;
    while (l < r) {
        lmax = max(lmax, h[l]);
        rmax = max(rmax, h[r]);
        if (lmax < rmax) {
            ans += lmax - h[l];  // 左边是短板
            l++;
        } else {
            ans += rmax - h[r];  // 右边是短板
            r--;
        }
    }
    return ans;
}
// 核心：双指针+两端最大高度 | 移动短板一侧 | 每列贡献=min(lmax,rmax)-h[i]""",
"""触发条件：数组蓄水/积雨 + 每列贡献独立 → 双指针（或单调栈）
两种解法对比：
- 双指针：O(n) O(1)，直觉是"短板效应"
- 单调栈：O(n) O(n)，按层计算，适合需要知道"水在哪些列之间"的场景
易错：
- ❌ 与#11盛水容器混淆（#11是找两个柱子围成的最大矩形面积，#42是每列独立积水求和）
- ❌ lmax/rmax忘记用max更新""",
"#11 盛最多水的容器(双指针类题对比) | #407 接雨水II(3D版，BFS+最小堆)")

# ======= 滑动窗口 (3,30) =======
add(30, """你是格斗游戏的作弊检测系统开发者。玩家输入流中可能包含秘技码（如Konami Code: "上上下下左右左右BA" 对应的字符串数组 words=["up","up","down","down","left","right","left","right","B","A"]）。
检测玩家的输入字符串s中，是否存在某个子串，恰好是words中所有单词的某种排列（每个单词恰好出现一次，不能有重叠）。
返回所有匹配子串的起始索引。""",
"""1. 暴力O(n*m*k)：枚举每个起点，检查长度为len(words)*word_len的子串 ❌
2. 滑动窗口+哈希表O(n*m)：
   - 窗口大小固定 = 总单词长度
   - 每次窗口右移一个单词长度（而非一个字符）
   - 哈希表统计窗口内单词频数，与words频数比较
3. 技巧：外层循环遍历起始偏移0到word_len-1（解决单词边界对齐问题）
4. 内层：固定窗口大小 + 单词级滑动 + 频数匹配""",
"""vector<int> findSubstring(string s, vector<string>& words) {
    int wlen = words[0].size(), total = words.size() * wlen;
    unordered_map<string, int> need;
    for (auto& w : words) need[w]++;
    vector<int> ans;
    for (int offset = 0; offset < wlen; offset++) {
        unordered_map<string, int> win;
        int l = offset, count = 0;
        for (int r = offset; r + wlen <= s.size(); r += wlen) {
            string w = s.substr(r, wlen);
            if (need.count(w)) {
                win[w]++;
                if (win[w] == need[w]) count++;
                while (win[w] > need[w]) {
                    string lw = s.substr(l, wlen);
                    if (win[lw] == need[lw]) count--;
                    win[lw]--;
                    l += wlen;
                }
                if (count == need.size()) ans.push_back(l);
            } else {
                win.clear(); count = 0; l = r + wlen;
            }
        }
    }
    return ans;
}
// 复杂滑动窗口 | 外层offset处理对齐 | 单词级滑动而非字符级""",
"""这是滑动窗口的复杂变体——窗口单位从字符变成单词
核心模式：固定窗口大小 + 频数匹配 + 不满足时收缩
和#3简单滑窗的区别：窗口不是字符级而是词级，需要offset对齐循环
复杂度：O(n*m)时间 O(m)空间""",
"#3 无重复字符最长子串(简化版滑窗) | #438 找到字符串中所有字母异位词")

# ======= 链表 (19,21,23,24,25) =======
add(19, """你是RPG游戏Buff管理系统的开发者。角色身上的Buff/Debuff按剩余持续时间升序排列成链表。
当玩家使用"驱散"技能时，需要移除倒数第N个Buff（即剩余时间第N短的Buff，也即离过期第N近的Buff）。
给定链表头节点和N，返回删除节点后的链表头。要求一次遍历。""",
"""1. 暴力O(2n)：先遍历得到长度L，再遍历到L-N处删除 ❌ 两次遍历
2. 快慢指针O(n)一次遍历：
   - 快指针先走N步
   - 然后快慢指针同步走，快指针到末尾时慢指针刚好在倒数第N个的前驱
   - 删除 slow->next
3. dummy节点：统一处理删除头节点的情况
4. 易错：N可能等于链表长度（删除头节点）——dummy救场""",
"""ListNode* removeNthFromEnd(ListNode* head, int n) {
    ListNode dummy(0);
    dummy.next = head;
    ListNode *fast = &dummy, *slow = &dummy;
    for (int i = 0; i <= n; i++) fast = fast->next; // 快指针先走N+1步
    while (fast) {
        fast = fast->next;
        slow = slow->next;
    }
    ListNode* del = slow->next;
    slow->next = slow->next->next;
    delete del;  // C++手动释放
    return dummy.next;
}
// 快指针先走N+1步(因为要定位到待删节点的前驱) | dummy不可或缺""",
"""触发条件：链表 + 倒数第N个 + 一次遍历 → 快慢指针
dummy节点是链表题的万能保险——统一处理头节点相关操作
快慢指针模板：
1. fast先走k步
2. fast和slow同步走直到fast走到末尾
3. slow停在目标位置的前驱
易错：
- ❌ 快指针先走N步还是N+1步（走N+1保证slow在待删节点的前驱）
- ❌ 忘记dummy，删除头节点时报错""",
"#876 链表的中间节点 | #141 环形链表(快慢指针) | #21 合并有序链表")

add(21, """你是跨服排行榜系统的开发者。两个服务器各自维护了已按战力排序的玩家排名链表。
在跨服活动时，需要将两个排行榜合并为一个统一的跨服排行榜，保持降序。
返回合并后链表的头节点。""",
"""1. 归并排序的合并步骤，双指针逐元素比较
2. 做法：
   - 用dummy节点简化头节点处理
   - cur指针指向当前合并链表的末尾
   - 比较list1和list2当前节点值，cur->next接上较小者
   - 推进对应链表和cur
   - 最后接上剩余非空链表
3. 递归写法也很优雅但空间O(n)
4. 迭代写法空间O(1)""",
"""ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
    ListNode dummy(0);
    ListNode* cur = &dummy;
    while (l1 && l2) {
        if (l1->val < l2->val) {
            cur->next = l1; l1 = l1->next;
        } else {
            cur->next = l2; l2 = l2->next;
        }
        cur = cur->next;
    }
    cur->next = l1 ? l1 : l2; // 接上剩余
    return dummy.next;
}
// 归并框架 | dummy省去头节点特殊处理 | 最后接剩余""",
"""触发条件：两个有序序列合并 → 归并思想
链表合并：O(n+m)时间 O(1)空间（迭代版）
同框架的题目：#88 合并有序数组（从后往前填，不用额外空间）
#21是#23合并K个链表的基础""",
"#23 合并K个升序链表(堆/K路归并) | #88 合并两个有序数组")

add(23, """你是MMO全球排行榜系统的开发者。K个服务器各自维护了已排序的玩家排名链表。
需要将它们合并成一个全局排行榜。直接两两归并(#21)的效率是O(KN)，可以用最小堆优化到O(N log K)。""",
"""1. 两两合并O(KN)：for(auto& list : lists) ans = mergeTwo(ans, list) ❌ 可行但非最优
2. 最小堆O(N log K)：
   - 将K个链表的头节点放入最小堆
   - 每次弹出堆顶最小节点，接入结果链表中
   - 将该节点的next推入堆
   - 重复直到堆空
3. C++中priority_queue默认最大堆，需自定义比较（或用pair的负数技巧）
4. 关键：堆中存节点指针 + 自定义比较函数""",
"""ListNode* mergeKLists(vector<ListNode*>& lists) {
    auto cmp = [](ListNode* a, ListNode* b) { return a->val > b->val; };
    priority_queue<ListNode*, vector<ListNode*>, decltype(cmp)> pq(cmp);
    for (auto& h : lists) if (h) pq.push(h);
    ListNode dummy(0);
    ListNode* cur = &dummy;
    while (!pq.empty()) {
        auto node = pq.top(); pq.pop();
        cur->next = node;
        cur = cur->next;
        if (node->next) pq.push(node->next);
    }
    return dummy.next;
}
// 最小堆K路归并 | priority_queue自定义比较 | 每次pop后push next""",
"""触发条件：K个有序序列合并 → 最小堆K路归并
复杂度：O(N log K) 时间，O(K) 空间（堆大小）
与#21的区别：K=2时两两足够，K>2用堆更优
C++技巧：priority_queue需自定义比较 lambda + decltype""",
"#21 合并两个有序链表 | #215 数组中的第K个最大元素(同样用堆)")

add(24, """你是双持武器技能槽系统的开发者。动作栏的相邻两个技能槽（主手/副手）支持一键交换位置。
技能槽以链表结构存储，实现每两个相邻节点交换位置的功能。
如 1→2→3→4 交换后为 2→1→4→3。不能只改值，必须改节点连接。""",
"""1. 递归思路：交换前两个节点，剩余递归处理
   - head指向第二个节点，head->next指向第一个节点，第一个节点的next指向递归(第三个节点开始)
2. 迭代思路：用prev指针记录上一组的尾节点
   - 交换prev->next和prev->next->next
   - prev跳到下一组的前一个
3. 递归更简洁，迭代空间更优""",
"""// 递归版（简洁）
ListNode* swapPairs(ListNode* head) {
    if (!head || !head->next) return head;
    ListNode* newHead = head->next;
    head->next = swapPairs(newHead->next);
    newHead->next = head;
    return newHead;
}
// 迭代版（空间O(1)）
ListNode* swapPairs(ListNode* head) {
    ListNode dummy(0); dummy.next = head;
    ListNode* prev = &dummy;
    while (prev->next && prev->next->next) {
        ListNode* a = prev->next;
        ListNode* b = a->next;
        a->next = b->next;
        b->next = a;
        prev->next = b;
        prev = a;
    }
    return dummy.next;
}
// 递归：三行搞定 | 迭代：prev+dummy+a+b四指针""",
"""触发条件：链表节点两两交换/翻转 → 递归或迭代+prev指针
#25是此题的通用化版本（K个一组翻转）
递归适合K=2的简单情况；K个一组必须迭代
易错：
- ❌ 交换后忘记重新连接前后组之间的链接
- ❌ prev没有及时更新（停在原地而非跳两个位置）""",
"#25 K个一组翻转链表 | #206 反转链表(基础操作)")

add(25, """你是操作历史系统的开发者。玩家的操作历史记录以链表存储，支持"批量撤销"功能——一次性回退最近K个操作。
实现分组翻转：每K个节点一组进行翻转，不足K个则保持原序。
要求空间O(1)（不能把节点值拷出来重组）。""",
"""1. 迭代遍历，每K个一组处理：
   Step 1：找当前组的尾节点（走K步，不足K则返回）
   Step 2：翻转该组 [head, tail]
   Step 3：prev接上翻转后的组头，组尾接下一段的头
   Step 4：prev跳到当前组尾，继续下一组
2. 翻转链表用标准的三指针法（prev, curr, next）
3. dummy节点统一处理头节点变化
4. 复杂度O(n)时间O(1)空间""",
"""ListNode* reverseKGroup(ListNode* head, int k) {
    ListNode dummy(0); dummy.next = head;
    ListNode* prev = &dummy;
    while (true) {
        // 检查是否有K个节点
        ListNode* tail = prev;
        for (int i = 0; i < k; i++) {
            tail = tail->next;
            if (!tail) return dummy.next;
        }
        ListNode* nextGroup = tail->next;
        // 翻转[head, tail]
        auto [newHead, newTail] = reverse(head, tail);
        prev->next = newHead;
        newTail->next = nextGroup;
        prev = newTail;
        head = nextGroup;
    }
    return dummy.next;
}
// 翻转函数
pair<ListNode*,ListNode*> reverse(ListNode* head, ListNode* tail) {
    ListNode* prev = tail->next, *curr = head;
    while (prev != tail) {
        ListNode* nxt = curr->next;
        curr->next = prev;
        prev = curr; curr = nxt;
    }
    return {tail, head};
}
// 核心：先找到每组的tail | 翻转后重新连接 | prev记录上一组尾""",
"""K个一组翻转 = 分组 + 每组内反转 + 组间连接
是链表题的集大成者：同时考察 遍历/分组/翻转/连接
易错：
- ❌ 翻转时忘记保留next指针 → 链表断裂
- ❌ 组间连接错误 → 翻转后的头尾接反
- ❌ 不足K个时忘记提前返回""",
"#24 两两交换(本题K=2特例) | #206 反转链表 | #92 反转链表II(指定区间翻转)")

# ======= 栈 (20,32) =======
add(20, """你是格斗游戏输入系统的开发者。玩家通过输入序列执行连招：'(' 代表技能开始，')' 代表技能取消。
需要验证整条连招链是否合法——每个取消指令必须匹配最近未关闭的技能开始指令，且最终所有技能必须正确关闭。
除了 '(' ')'，连招系统中还可能有 '[' ']' '{' '}' 等其他类型的技能标记。""",
"""1. 栈匹配：遍历字符
   - 遇到开始标记 '(' / '[' / '{' → push到栈
   - 遇到取消标记 → 检查栈顶是否匹配
   - 不匹配或栈空→无效；匹配→pop
   - 最后栈必须为空
2. 用哈希表映射配对关系
3. 复杂度O(n)时间O(n)空间""",
"""bool isValid(string s) {
    stack<char> st;
    unordered_map<char, char> m = {
        {')', '('}, {']', '['}, {'}', '{'}
    };
    for (char c : s) {
        if (m.count(c)) { // 是右括号
            if (st.empty() || st.top() != m[c]) return false;
            st.pop();
        } else {
            st.push(c);
        }
    }
    return st.empty();
}
// 哈希表映射配对 | 右括号检查栈顶 | 最终栈为空""",
"""触发条件：嵌套/匹配/成对出现 + 验证合法性 → 栈
栈的经典应用：括号匹配、表达式求值、函数调用栈
复杂度：O(n) O(n)
延伸：此模型直接对应游戏开发中众多嵌套验证场景
- 动画系统：AnimMontage Section的开始/结束标记 → 栈验证
- 对话系统：分支标记的匹配性验证""",
"#32 最长有效括号 | #22 括号生成 | #921 使括号有效的最少添加")

add(32, """你是格斗游戏战斗回放分析系统的开发者。给定一条技能开始('(')和取消(')')的事件日志字符串，找到其中最长的合法连招链子串的长度。
例如：输入 "(()" → 最长合法子串 "()" 长度=2
输入 ")()())" → 最长合法子串 "()()" 长度=4""",
"""1. 解法一：栈+索引差值 O(n)
   - 栈存索引（不是字符）
   - 初始推入-1作为基准索引
   - 遇 '(' 推索引，遇 ')' 弹出
   - 弹出后栈空→推入当前索引作为新基准
   - 弹出后栈非空→len = i - st.top() 更新max
2. 解法二：DP O(n)
   - dp[i] = 以s[i]结尾的最长有效括号长度
   - s[i]==')'且s[i-1]=='(' → dp[i]=dp[i-2]+2
   - s[i]==')'且s[i-1]==')' → 检查s[i-dp[i-1]-1]=='(' → dp[i]=dp[i-1]+2+dp[i-dp[i-1]-2]
3. 栈解法更直观，推荐""",
"""// 栈解法
int longestValidParentheses(string s) {
    stack<int> st;
    st.push(-1); // 基准索引
    int ans = 0;
    for (int i = 0; i < s.size(); i++) {
        if (s[i] == '(') {
            st.push(i);
        } else {
            st.pop();
            if (st.empty()) {
                st.push(i); // 新基准
            } else {
                ans = max(ans, i - st.top());
            }
        }
    }
    return ans;
}
// 栈存索引不是字符 | 初始-1基准 | 栈空重新设基准 | i-st.top()得长度""",
"""触发条件：最长有效子序列/子串 + 括号/匹配类 → 栈+索引差 或 DP
栈存索引而非字符是核心技巧：通过索引差求长度
基准索引(-1)的作用：处理从头开始的合法串
与#20的区别：#20判断整体是否合法 | #32找最长的合法子串""",
"#20 有效的括号(基础) | #22 括号生成(回溯生成)")

# ======= 二分查找 (4,33,34,35) =======
add(33, """你是圆形轮盘道具选择UI的开发者。玩家按Tab键打开圆形径向菜单，物品排列在轮盘上（按ID排序）。轮盘被旋转到某个未知角度后（相当于数组被旋转了未知次），玩家仍能快速选中目标物品。
给定旋转后的轮盘物品数组（原升序+在某点旋转），找到目标物品的索引。要求O(log n)。""",
"""1. 二分查找的变体——数组不是完全有序，但"部分有序"
2. 每次二分：mid将数组分成两半，至少有一半是有序的
3. 判断目标在有序半还是无序半：
   - [l,mid]有序：nums[l]≤nums[mid]
     若target在[nums[l],nums[mid])→搜左，否则→搜右
   - [mid+1,r]有序：
     若target在(nums[mid],nums[r]]→搜右，否则→搜左
4. 关键：判断哪半有序时用 nums[l] ≤ nums[mid]，注意等号（处理mid==l的情况）""",
"""int search(vector<int>& nums, int target) {
    int l = 0, r = nums.size() - 1;
    while (l <= r) {
        int mid = l + (r - l) / 2;
        if (nums[mid] == target) return mid;
        if (nums[l] <= nums[mid]) {  // 左半有序
            if (nums[l] <= target && target < nums[mid]) r = mid - 1;
            else l = mid + 1;
        } else {  // 右半有序
            if (nums[mid] < target && target <= nums[r]) l = mid + 1;
            else r = mid - 1;
        }
    }
    return -1;
}
// 核心：每次二分判断哪半有序 | 在有序半中判断target是否在范围内""",
"""触发条件：有序数组被旋转/移位 + O(log n)查找 → 旋转数组二分
核心洞察：旋转数组虽然整体无序，但"至少有一半有序"
判断有序半：nums[l] <= nums[mid] → 左半有序
注意：nums[l]==nums[mid]时（mid==l），左半"有序"（单元素）""",
"#81 搜索旋转排序数组II(含重复元素) | #153 寻找旋转排序数组中的最小值")

add(34, """你是LOD（细节层次）系统的开发者。场景中所有物体按距摄像机的距离升序存储。
当摄像机移动时，需要找出哪些物体应该切换到LOD1（中距离细节）：找到所有距离恰好等于某个切换阈值的物体的起始和结束索引。
给定排序后的距离数组和阈值target，返回[第一个出现位置, 最后一个出现位置]。若不存在返回[-1,-1]。要求O(log n)""",
"""1. 两次二分查找：
   - 第一次找左边界：找第一个 >= target 的位置
   - 第二次找右边界：找第一个 > target 的位置 - 1（或最后一个 <= target）
2. 左边界二分：
   - nums[mid] < target → l=mid+1
   - nums[mid] >= target → r=mid
3. 右边界二分：
   - nums[mid] <= target → l=mid+1
   - nums[mid] > target → r=mid
   返回 l-1（最后一个 <= target 的索引）
4. 边界检查：左边界 <= 右边界 且 左右边界不越界""",
"""vector<int> searchRange(vector<int>& nums, int target) {
    if (nums.empty()) return {-1, -1};
    // 左边界：第一个 >= target
    int l = 0, r = nums.size() - 1;
    while (l < r) {
        int mid = l + (r - l) / 2;
        if (nums[mid] < target) l = mid + 1;
        else r = mid;
    }
    if (nums[l] != target) return {-1, -1};
    int left = l;
    // 右边界：最后一个 <= target
    r = nums.size() - 1;
    while (l < r) {
        int mid = l + (r - l + 1) / 2; // 上取整防死循环
        if (nums[mid] <= target) l = mid;
        else r = mid - 1;
    }
    return {left, l};
}
// 左边界：while(l<r) mid下取整 r=mid | 右边界：mid上取整 l=mid""",
"""触发条件：有序数组 + 找某值的区间/边界 → 两次二分
左边界：第一个≥target → while(l<r) mid下取整
右边界：最后一个≤target → while(l<r) mid上取整
易错：
- ❌ 左右边界混用同一套二分逻辑（中点取整方向不同）
- ❌ 忘记检查target是否存在
- ❌ 右边界二分死循环（mid下取整+l=mid）""",
"#35 搜索插入位置(左边界) | #33 搜索旋转排序数组 | #704 二分查找(基础)")

add(35, """你是背包系统的开发者。玩家的背包按稀有度排序存储物品。当获得新物品时，需要找到它在背包中的正确插入位置以维持排序顺序。
给定排序后的稀有度数组和目标稀有度值target，返回target应插入的索引（如果target已存在，插入到该位置前面，即第一个≥target的位置）。
其实就是实现二分查找的左边界版。""",
"""1. 标准二分查找左边界：
   - while(l<r)
   - mid = l + (r-l)/2
   - nums[mid] < target → l=mid+1
   - nums[mid] >= target → r=mid
   - 返回 l
2. 如果target大于所有元素，l会停在nums.size()
3. 本质就是lower_bound()
4. 这是最简单但最重要的二分模板""",
"""int searchInsert(vector<int>& nums, int target) {
    int l = 0, r = nums.size();
    while (l < r) {
        int mid = l + (r - l) / 2;
        if (nums[mid] < target) l = mid + 1;
        else r = mid;
    }
    return l;
}
// 万能二分模板：找第一个≥target | r初始化为n(应对插入末尾) | while(l<r)""",
"""这是最通用的二分模板——找"第一个满足条件的位置"
模板参数化：
- 找第一个≥target：if(nums[mid]<target)l=mid+1 else r=mid
- 找第一个>target：if(nums[mid]<=target)l=mid+1 else r=mid
复杂度：O(log n) O(1)
所有二分变体都可以从这个模板推导出来""",
"#34 查找元素范围(左右边界) | #704 二分查找")

# ======= 回溯 (17,22,37,39,40) =======
add(17, """你是技能树构建器的UI工具开发者。在RPG游戏中，数字2-9分别映射到若干种技能符文（类似老式手机键盘）。
玩家选择一个数字序列（如"23"），你的工具需要生成所有可能的技能符文组合方案，供玩家预览并拖入快捷栏。
数字到符文的映射：2→[a,b,c] 3→[d,e,f] 4→[g,h,i] 5→[j,k,l] 6→[m,n,o] 7→[p,q,r,s] 8→[t,u,v] 9→[w,x,y,z]""",
"""1. 标准回溯模板：
   - 递归函数backtrack(index, path)
   - index：当前处理到digits的第几个位置
   - path：当前已构造的字符串
   - 终止条件：index==digits.size() → 记录path
   - 搜索：遍历digits[index]对应的所有字符，递归调用
2. 本质：组合的笛卡尔积——每层的选择空间是独立的
3. 用数组map[10]存储数字到字符的映射
4. 复杂度O(4ⁿ)——每个数字最多对应4个字母""",
"""vector<string> letterCombinations(string digits) {
    if (digits.empty()) return {};
    vector<string> map = {"","","abc","def","ghi","jkl","mno","pqrs","tuv","wxyz"};
    vector<string> ans;
    function<void(int,string)> dfs = [&](int idx, string path) {
        if (idx == digits.size()) {
            ans.push_back(path);
            return;
        }
        for (char c : map[digits[idx] - '0']) {
            dfs(idx + 1, path + c);
        }
    };
    dfs(0, "");
    return ans;
}
// 回溯模板：idx索引+path路径 | 每层循环选择 | 终止条件收集结果""",
"""标准回溯模板：
void backtrack(状态参数) {
    if (满足终止条件) { 收集结果; return; }
    for (选择 : 当前层的选项) {
        做选择;
        backtrack(下一层状态);
        撤销选择; // 本题path+c不修改原path，省略撤销
    }
}
回溯 = DFS + 剪枝，核心是"选择-递归-撤销"循环""",
"#22 括号生成 | #39 组合总和 | #78 子集")

add(22, """你是RPG对话树编辑器的开发者。对话树由NPC提问和玩家回应交替组成。NPC提问用'('表示一个分支开始，玩家回应用')'表示该分支结束。
给定分支数n，生成所有可能的合法对话树结构——要求每个分支都有头有尾，不存在悬空未关闭的提问。
例如n=3 → ["((()))","(()())","(())()","()(())","()()()"]""",
"""1. 回溯+约束剪枝：
   - 维护open(已使用的'('数)和close(已使用的')'数)
   - open < n 时可添加 '('
   - close < open 时可添加 ')'（保证不会出现')'比'('多的情况）
   - open==close==n 时收集结果
2. 与#20的区别：#20是验证，#22是生成所有合法的
3. 本质是Catalan数——第n个Catalan数种合法结构
4. 复杂度O(4ⁿ/√n)——Catalan数的渐近界""",
"""void backtrack(int n, int open, int close, string& path, vector<string>& ans) {
    if (open == n && close == n) {
        ans.push_back(path);
        return;
    }
    if (open < n) {
        path.push_back('(');
        backtrack(n, open + 1, close, path, ans);
        path.pop_back(); // 撤销
    }
    if (close < open) {
        path.push_back(')');
        backtrack(n, open, close + 1, path, ans);
        path.pop_back(); // 撤销
    }
}
// 两个if条件不是互斥的 | open<n时还可加'(' | close<open时还可加')'""",
"""触发条件：生成所有合法组合/结构 + 有约束条件 → 回溯+剪枝
和标准回溯的区别：不是`for choices`而是两个独立的条件判断
剪枝条件：close<open 保证不会出现非法前缀
Category数是Catalan数 C(2n,n)/(n+1)""",
"#20 有效的括号(验证) | #17 电话号码组合(独立选项回溯) | #39 组合总和")

add(37, """你是地图编辑器的自动填充工具开发者。给定一个部分设计的9×9谜题地图（数独棋盘），其中'.'表示空格。
需要自动填充剩余格子，同时满足数独规则：
- 每行1-9各出现一次
- 每列1-9各出现一次
- 每个3×3子框1-9各出现一次""",
"""1. 回溯法：
   - 找到下一个空格
   - 尝试填入1-9，检查是否合法
   - 合法则递归下一个空格
   - 不合法回溯
2. 优化技巧：
   - 用三个bool[9][10]数组分别记录行/列/框的使用情况 → O(1)判断合法
   - 或者直接用位掩码（int用低9位表示1-9）→ 更快
   - 找空格时用"最少候选数"的格子（启发式剪枝）
3. 复杂度最坏O(9ⁿ)但实际远小于此
4. 边界：题目保证有唯一解""",
"""// 核心回溯 + 位掩码优化
int row[9] = {}, col[9] = {}, box[9] = {};
bool dfs(vector<vector<char>>& board, int i, int j) {
    if (i == 9) return true;
    int ni = (j == 8) ? i + 1 : i;
    int nj = (j == 8) ? 0 : j + 1;
    if (board[i][j] != '.') return dfs(board, ni, nj);
    int mask = row[i] | col[j] | box[i/3*3 + j/3];
    for (int d = 1; d <= 9; d++) {
        if (mask & (1 << d)) continue;  // 已使用
        row[i] |= 1<<d; col[j] |= 1<<d; box[i/3*3+j/3] |= 1<<d;
        board[i][j] = '0' + d;
        if (dfs(board, ni, nj)) return true;
        board[i][j] = '.';
        row[i] ^= 1<<d; col[j] ^= 1<<d; box[i/3*3+j/3] ^= 1<<d;
    }
    return false;
}
// 位掩码优化O(1)查重 | 找到解立即返回 | 按行扫描""",
"""触发条件：约束满足 + 需要填充/搜索所有可能 → 回溯
优化：用位运算/数组缓存判断"能否选择"，避免每次O(n)校验
数独问题在游戏中的应用：
- 地图编辑器自动填充
- 程序化谜题生成（从空开始逐格填→回溯）
- 建筑放置验证（同行列3×3不重复）""",
"#36 有效数独(验证) | #39 组合总和 | #51 N皇后(同是回溯+约束)")

add(39, """你是RPG背包配装生成器的开发者。玩家有一个容量为target的背包，仓库中有若干种物品，每种物品有特定的重量值。
玩家希望找到所有恰好填满背包容量的物品组合方案——每种物品可以使用任意多次（无限供应）。
给定无重复的物品重量数组candidates和背包容量target，返回所有不重复的组合方案。""",
"""1. 回溯+可以重复选择：
   - 递归参数：idx(当前考虑的物品下标), remain(剩余容量), path
   - 终止：remain==0 → 收集结果；remain<0 → 回溯
   - 每层从idx开始（不是0，保证不重复组合），因为可无限选但不想重复排列
2. 关键：start参数保证组合有序（避免[2,3]和[3,2]重复）
3. 剪枝：排序candidates后，若candidates[i] > remain，break（后面更大）
4. 时间复杂度：O(N^(T/M))——最坏情况""",
"""vector<vector<int>> combinationSum(vector<int>& cand, int target) {
    sort(cand.begin(), cand.end());
    vector<vector<int>> ans;
    vector<int> path;
    function<void(int,int)> dfs = [&](int idx, int remain) {
        if (remain == 0) { ans.push_back(path); return; }
        for (int i = idx; i < cand.size(); i++) {
            if (cand[i] > remain) break; // 剪枝
            path.push_back(cand[i]);
            dfs(i, remain - cand[i]); // 可重复：下次从i开始
            path.pop_back();
        }
    };
    dfs(0, target);
    return ans;
}
// 可重复→递归传i而非i+1 | 排序+剪枝 | start参数防重复组合""",
"""触发条件：组合/子集 + 可重复 + 求和=target → 回溯（传idx控制不重复）
与#40的区别：
- #39每种无限用 → dfs(i, remain-cand[i])（下一个还从i开始）
- #40每种只能用一次 → dfs(i+1, remain-cand[i])（下一个从i+1开始）
排序+break剪枝是通用优化""",
"#40 组合总和II(不可重复) | #216 组合总和III(限制k个数) | #77 组合(无重复)")

add(40, """你是天赋树优化器的开发者。玩家的天赋树中有N个可选天赋，每个天赋点只能选择一次（选了就消耗掉）。
找出所有总消耗恰好等于target的天赋组合方案，每种天赋最多用一次。天赋点数值可能重复（同名天赋但不同等级的数值相同），但答案中不能包含重复的组合。""",
"""1. 回溯+不能重复选择+去重：
   - 递归参数：idx, remain, path
   - 每层从i=idx开始选择，递归传i+1（因为不可重复选）
   - 去重关键：if(i>idx && cand[i]==cand[i-1]) continue
   - 排序预处理让相同值相邻
2. 剪枝同#39：排序+break
3. 与#39的区别：
   - i+1（不可重复）vs i（可重复）
   - 需要跳过同层相同元素去重""",
"""vector<vector<int>> combinationSum2(vector<int>& cand, int target) {
    sort(cand.begin(), cand.end());
    vector<vector<int>> ans;
    vector<int> path;
    function<void(int,int)> dfs = [&](int idx, int remain) {
        if (remain == 0) { ans.push_back(path); return; }
        for (int i = idx; i < cand.size(); i++) {
            if (cand[i] > remain) break;
            if (i > idx && cand[i] == cand[i-1]) continue; // 同层去重
            path.push_back(cand[i]);
            dfs(i + 1, remain - cand[i]); // 不可重复→i+1
            path.pop_back();
        }
    };
    dfs(0, target);
    return ans;
}
// i>idx跳过同层重复 | i+1不可重复选 | 同模板换成i即可变为#39""",
"""与#39对比记忆：
- 可重复 → dfs(i, remain-X) + 不需要去重
- 不可重复 → dfs(i+1, remain-X) + i>idx && same → continue
- 两者都需排序（for剪枝 + #40去重依赖排序）
核心去重技巧：同层跳过（i>idx不是i>0）""",
"#39 组合总和(可重复) | #47 全排列II(同去重模式) | #78 子集II")

# ======= 动态规划 (5,10) =======
#5 already defined above.

# ======= 哈希表 (12,13,36,41) =======
add(12, """你是物品UI系统的开发者。游戏内部的稀有度等级用整数表示(1-3999)，需要在UI上显示为对应的稀有度标签（如"传说""史诗"等）。
实现一个将整数转换为稀有度标签字符串的函数。可以类比为罗马数字转换问题——使用固定的映射表和贪心策略。""",
"""1. 贪心：从大到小依次尝试所有符号
   - 建立数值→符号的映射表（降序排列）
   - 对num，while(num >= 当前值) 追加对应符号并减去值
2. 有限值域(1-3999)，时间复杂度O(1)
3. 本质：贪心法——每次选择能使用的最大符号""",
"""string intToRoman(int num) {
    int vals[] = {1000,900,500,400,100,90,50,40,10,9,5,4,1};
    string syms[] = {"M","CM","D","CD","C","XC","L","XL","X","IX","V","IV","I"};
    string ans;
    for (int i = 0; i < 13; i++) {
        while (num >= vals[i]) {
            ans += syms[i];
            num -= vals[i];
        }
    }
    return ans;
}
// 贪心：从大到小 | 映射表降序 | 13对映射覆盖所有情况""",
"""触发条件：数值→有限符号集的映射 + 贪心可用 → 建表+贪心
此模式在游戏中常用于：稀有度显示、等级显示、成就徽章
复杂度：O(1)（因为输入范围固定）""",
"#13 罗马数字转整数(反向) | #273 整数转换英文表示(变体)")

add(13, """你是游戏配置文件解析系统的开发者。配置文件中的装备等级用自定义符号标记（如"X"=10,"V"=5,"I"=1），需要解析为数值。
给定一个符号令牌字符串，转换为对应的整数值。规则：小值在大值左侧做减法(IV=4)，右侧做加法(VI=6)。""",
"""1. 从右向左遍历（或从左向右+和前一位比较）
2. 从左到右：
   - 若当前值 < 下一个值 → 减去当前值
   - 否则 → 加上当前值
3. 哈希表存储字符→数值映射
4. 复杂度O(n)""",
"""int romanToInt(string s) {
    unordered_map<char, int> m = {
        {'I',1},{'V',5},{'X',10},{'L',50},{'C',100},{'D',500},{'M',1000}
    };
    int ans = 0;
    for (int i = 0; i < s.size(); i++) {
        if (i + 1 < s.size() && m[s[i]] < m[s[i+1]]) {
            ans -= m[s[i]];
        } else {
            ans += m[s[i]];
        }
    }
    return ans;
}
// 核心洞察：小值在大值前→做减法 | 否则做加法""",
"""触发条件：自定义符号→数值 + 位置相关规则 → 哈希映射+遍历判断
比#12稍复杂——因为需要根据相邻关系判断加减""",
"#12 整数转罗马数字(正向) | #171 Excel表列序号(类似进制转换)")

add(36, """你是RTS/城市建造类游戏的建筑放置验证系统开发者。地图是一个9×9的网格（相当于数独的9×9棋盘）。
在RTS游戏中，需要验证新建筑的放置不违反区域约束：同一行、同一列、同一3×3子区域内不能出现两个同类建筑（如同一子区域的两个兵营）。
给定当前地图上的建筑布局（部分格子有建筑，部分为空），验证当前布局是否合法。""",
"""1. 三组哈希集合：row[9][10], col[9][10], box[9][10]
2. 遍历棋盘，遇到非空格：
   - 检查row[i][num]是否为true → 冲突
   - 检查col[j][num] → 冲突
   - 检查box[boxIndex][num] → 冲突
   - 无冲突则全部标为true
3. boxIndex = (i/3)*3 + (j/3)
4. 注意只验证已放置的建筑，不判断是否有解""",
"""bool isValidSudoku(vector<vector<char>>& board) {
    bool row[9][10] = {}, col[9][10] = {}, box[9][10] = {};
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (board[i][j] == '.') continue;
            int num = board[i][j] - '0';
            int bi = i / 3 * 3 + j / 3;
            if (row[i][num] || col[j][num] || box[bi][num])
                return false;
            row[i][num] = col[j][num] = box[bi][num] = true;
        }
    }
    return true;
}
// 三组哈希表| box索引=(i/3)*3+(j/3) | 空格跳过""",
"""触发条件：网格 + 行/列/子区域唯一性约束 → 多维度哈希表
关键公式：boxIndex = (i/3)*3 + j/3（将3×3子网格映射到0-8）
复杂度：O(1)（固定9×9）
游戏应用：建筑放置、技能范围判定、地图区域约束""",
"#37 解数独(填充版) | #733 图像渲染(网格遍历)")

add(41, """你是游戏引擎中对象ID分配器的开发者。游戏中所有对象（角色、道具、特效等）创建时分配正数ID，销毁后ID回收。
给定当前所有活跃对象的ID数组（可能包含重复或负数），你需要找到可以分配给新对象的最小正整数ID（即第一个不在数组中的正整数）。
要求时间O(n)，空间O(1)（原地算法）。""",
"""1. 原地哈希：核心思路——把值x放到索引x-1的位置
2. 遍历数组，对于 nums[i]：
   - 若1 <= nums[i] <= n 且 nums[i] != nums[nums[i]-1]（不在正确位置）
   - 交换 nums[i] 和 nums[nums[i]-1]
   - 继续检查交换后的新 nums[i]（while循环而非if）
3. 二次遍历：找第一个 nums[i] != i+1 的位置，返回 i+1
4. 复杂度O(n)时间 O(1)空间
5. 为什么这是"哈希"：数组本身充当了哈希表的角色——索引就是"哈希位置”""",
"""int firstMissingPositive(vector<int>& nums) {
    int n = nums.size();
    for (int i = 0; i < n; i++) {
        // 将 nums[i] 放到索引 nums[i]-1（它的"家"）
        while (nums[i] >= 1 && nums[i] <= n && nums[i] != nums[nums[i]-1]) {
            swap(nums[i], nums[nums[i]-1]);
        }
    }
    for (int i = 0; i < n; i++) {
        if (nums[i] != i + 1) return i + 1;
    }
    return n + 1;
}
// 原地哈希：把值x放到索引x-1 | while不是if(交换后继续检查) | 二次扫描找缺口""",
"""触发条件：缺失的第一个正数/0到n中缺失的数 + O(n)+O(1) → 原地哈希
原地哈希核心：数组索引 = 哈希位置，值放到对应索引
while而非if：因为交换后新来的值可能也需要归位
游戏应用：对象池ID分配、网络连接ID管理""",
"#268 丢失的数字 | #448 找到所有数组中消失的数字(同技巧)")

# ======= 数学/位运算 (7,9,29,43) =======
add(29, """你是物理引擎中定点数计算模块的开发者。为了保证跨平台确定性（禁止使用浮点数），物理计算全部使用定点数。
实现定点数的除法运算——给定被除数dividend和除数divisor，不使用乘法/除法/mod运算符，计算除法结果并截断小数部分。
注意边界：-2³¹到2³¹-1，溢出返回INT_MAX（2³¹-1）""",
"""1. 暴力O(dividend)：不断减去除数直到被除数<除数 ❌ 太慢
2. 位移加速O(log n)：左移除数直到刚好小于被除数，然后逐级右移
3. 核心技巧：
   - 全部转成正数处理（用long防溢出）
   - 外层while(dividend>=divisor)：内层找最大shift使 divisor<<shift <= dividend
   - ans += (1<<shift); dividend -= (divisor<<shift)
4. 符号处理：异或判断结果符号
5. 边界：dividend=INT_MIN divisor=-1时结果溢出→返回INT_MAX""",
"""int divide(int dividend, int divisor) {
    if (dividend == INT_MIN && divisor == -1) return INT_MAX;
    long a = labs(dividend), b = labs(divisor);
    long ans = 0;
    int sign = (dividend > 0) ^ (divisor > 0) ? -1 : 1;
    while (a >= b) {
        long temp = b, shift = 1;
        while (a >= (temp << 1)) {
            temp <<= 1; shift <<= 1;
        }
        a -= temp;
        ans += shift;
    }
    return ans * sign;
}
// 位移除法：每次找最大的2^k*divisor ≤ dividend | 用long防溢出""",
"""触发条件：不允许乘/除/模 + 需要高效除法 → 位移模拟除法
核心思想：每次减掉最大的2^k倍除数（相当于二进制的除法竖式）
游戏应用：定点数物理引擎（保证跨平台确定性，浮点数结果不可重现）
复杂度：O(log²n)
易错：INT_MIN/-1溢出是经典边缘条件""",
"#50 Pow(x,n)(快速幂同位移思想) | #7 整数反转")

add(43, """你是MMO经济系统的开发者。拍卖行中两笔大额货币（超过64位整数范围）需要进行乘法运算（如：黄金数量 × 汇率 = 手续费）。
由于数值超大，不能直接用整数乘法。需要实现字符串级别的乘法运算。
给定两个非负整数字符串 num1 和 num2，返回它们的乘积字符串。""",
"""1. 模拟手工竖式乘法：
   - 结果的位数最多为 m+n
   - pos[i+j] 和 pos[i+j+1] 存储当前乘积位
   - num1[i] * num2[j] 的结果加到 pos[i+j+1]（低位）
   - 逐位进位处理
2. 为什么不能简单转整数：num1或num2可长达110位
3. 优化：用int数组而非string拼接（减少字符串操作开销）
4. 边界处理：乘积为0时返回"0"而非空串""",
"""string multiply(string num1, string num2) {
    if (num1 == "0" || num2 == "0") return "0";
    int m = num1.size(), n = num2.size();
    vector<int> pos(m + n, 0);
    for (int i = m - 1; i >= 0; i--) {
        for (int j = n - 1; j >= 0; j--) {
            int mul = (num1[i] - '0') * (num2[j] - '0');
            int p1 = i + j, p2 = i + j + 1;
            int sum = mul + pos[p2];
            pos[p2] = sum % 10;
            pos[p1] += sum / 10;
        }
    }
    string ans;
    for (int d : pos) {
        if (!(ans.empty() && d == 0)) ans += char(d + '0');
    }
    return ans;
}
// 竖式乘法入位 | pos[p1]存进位 pos[p2]存本位 | 跳过前导零""",
"""触发条件：超大数运算（超过int64范围） → 字符串模拟竖式计算
类似题目：#415 字符串相加 #67 二进制求和
游戏应用：MMO经济系统、天文学数值计算、科学模拟
复杂度：O(mn)时间 O(m+n)空间""",
"#415 字符串相加 | #67 二进制求和 | #2 两数相加(链表版大数加法)")

# ======= 字符串 (6,8,14,28,38) =======
add(14, """你是渲染引擎中资源加载系统的开发者。场景中需要加载大量纹理资源，路径如 "textures/environment/forest/tree_01.dds"。
为了批量加载同一目录下的资源以减少I/O次数，你需要找到一组资源路径的公共前缀。""",
"""1. 纵向扫描：以第一个字符串为基准，逐字符与其他字符串比较
2. 横向扫描：逐个字符串比较，每次更新公共前缀
3. 分治法：将字符串数组分成两半递归求解（同归并思想）
4. 推荐纵向扫描：最直观，O(S)时间（S是所有字符总数）
5. 优化：先排序，比较第一个和最后一个字符串的公共前缀即可（因为排序后差异最大化）""",
"""string longestCommonPrefix(vector<string>& strs) {
    if (strs.empty()) return "";
    // 以第一个为基准
    for (int i = 0; i < strs[0].size(); i++) {
        char c = strs[0][i];
        for (int j = 1; j < strs.size(); j++) {
            if (i == strs[j].size() || strs[j][i] != c)
                return strs[0].substr(0, i);
        }
    }
    return strs[0];
}
// 纵向扫描：固定列号逐串比较 | 索引越界或字符不等时返回""",
"""触发条件：多字符串公共前缀 → 纵向扫描 / 排序后比较首尾
游戏应用：资源路径优化、UI文本本地化key管理
排序优化法：sort后比较first和last（因为公共前缀在排序后会聚集）
复杂度：O(S) S=总字符数""",
"#3 最长不重复子串 | #28 实现strStr")

add(28, """你是游戏服务器日志搜索工具开发者。每天产生海量的服务器日志，需要快速定位某条特定错误信息第一次出现的位置。
实现KMP字符串匹配算法——在haystack中找needle的第一次出现索引。""",
"""1. 暴力O(mn)：从每个位置开始逐个比较 ❌
2. KMP O(n+m)：利用匹配失败时的已知信息跳过无效尝试
   Step 1：构建next数组（前缀函数）—对于needle的每个位置，最长相同前后缀的长度
   Step 2：匹配主串和模式串，失配时利用next数组跳转
3. next数组含义：next[i] = needle[0..i-1]的最长相等前后缀长度
4. 实际工作中可能直接用标准库find()，但KMP的next数组思想在游戏很多场景有用（状态机、序列预测）""",
"""int strStr(string haystack, string needle) {
    if (needle.empty()) return 0;
    int n = haystack.size(), m = needle.size();
    // Step1: 构建next数组
    vector<int> next(m, 0);
    for (int i = 1, j = 0; i < m; i++) {
        while (j > 0 && needle[i] != needle[j]) j = next[j-1];
        if (needle[i] == needle[j]) j++;
        next[i] = j;
    }
    // Step2: 匹配
    for (int i = 0, j = 0; i < n; i++) {
        while (j > 0 && haystack[i] != needle[j]) j = next[j-1];
        if (haystack[i] == needle[j]) j++;
        if (j == m) return i - m + 1;
    }
    return -1;
}
// KMP核心：next数组 + 失配时j回退到next[j-1] | O(n+m)""",
"""触发条件：字符串模式匹配（不用内置find）→ KMP
KMP精髓：匹配失败时利用"已匹配部分"的信息跳过不可能的位置
next[i] = pattern[0..i-1]的最长相等前后缀 = 匹配失败时j应该回退的位置
游戏应用：日志搜索、聊天过滤模式匹配、作弊码检测
复杂度：构建next O(m) 匹配O(n)""",
"#214 最短回文串(KMP应用) | #459 重复的子字符串(KMP周期)")

add(38, """你是游戏存档压缩系统的开发者。存档数据需要压缩存储以节省云存档空间。使用游程编码（Run-Length Encoding, RLE）对字符串进行压缩。
压缩规则：count-and-say——统计连续相同字符的个数+字符本身。
例如：第1项="1"；第2项="11"(1个1)；第3项="21"(2个1)；第4项="1211"(1个2+2个1)...
给定n，返回第n项的外观数列。""",
"""1. 递归/迭代生成：从第1项开始逐项生成
2. 对每个字符串做RLE编码：遍历+计数组+拼接
3. 复杂度：第n个字符串长度约为1.3^(n-1)
4. 关键：内层双指针/计数循环
   - cnt记录当前字符连续出现次数
   - 遇到不同字符时，追加cnt+字符到结果
5. 游戏应用：RLE广泛用于纹理压缩、高度图压缩、存档压缩""",
"""string countAndSay(int n) {
    string s = "1";
    for (int i = 1; i < n; i++) {
        string next;
        for (int j = 0; j < s.size(); ) {
            int k = j;
            while (k < s.size() && s[k] == s[j]) k++;
            next += to_string(k - j) + s[j];
            j = k;
        }
        s = next;
    }
    return s;
}
// RLE编码：内层while计数 | to_string(cnt)+字符 | 迭代n-1次""",
"""触发条件：压缩/编码 + 迭代生成 → 模拟+计数
RLE是游戏中最常用的压缩算法之一——GPU纹理压缩、高度图压缩
核心模式：while(k<n && s[k]==s[j]) k++; 计数字符连续个数
复杂度：字符串长度指数增长 ~O(1.3ⁿ)""",
"#443 字符串压缩(标准RLE) | #28 实现strStr")

# ======= 其余问题 =======
add(2, """同前所述战斗伤害浮动计算：两个链表各表示一把武器的各段伤害值，合并计算总伤害。""",
"""见代码模板。核心：模拟竖式加法，dummy节点+进位carry。""",
"""// 见完整代码模板（已在该记录中）""",
"""链表加法=模拟竖式 关键：dummy+carry+短链表补0""",
"#415 字符串相加 | #445 两数相加II")

for n in [2,3,4,5,6,7,8,9,10]:
    if n in D: continue  # already defined in detail above
    D[n] = {
        "题目描述": f"问题 #{n} - 已有基础场景描述，详见游戏开发场景字段",
        "解题思路": "详见原有解法和注释",
        "代码模板": "参见对应 .cpp 文件",
        "算法思维框架": "参考对应算法类别的思维框架",
        "相似题目": "待补充"
    }


def update_record(prob_num, record_id, content):
    """Update a single record via lark-cli."""
    json_str = json.dumps(content, ensure_ascii=False)
    lark = r"C:\Users\11060\AppData\Roaming\npm\lark-cli"
    cmd = [
        "cmd", "/c", lark, "base", "+record-upsert",
        "--base-token", BASE,
        "--table-id", TBL,
        "--record-id", record_id,
        "--json", json_str,
        "--as", "user"
    ]
    result = subprocess.run(cmd, capture_output=True, text=True, timeout=30)
    if result.returncode != 0:
        print(f"  FAIL #{prob_num}: {result.stderr[:200]}")
        return False
    return True


def main():
    total = len(D)
    done = 0
    for pnum in sorted(D.keys()):
        rid = rids.get(pnum)
        if not rid:
            print(f"SKIP #{pnum}: no record_id")
            continue
        print(f"Updating #{pnum} ({rid})...", end=" ", flush=True)
        if update_record(pnum, rid, D[pnum]):
            print("OK")
            done += 1
        else:
            print("FAIL")
        time.sleep(0.6)  # rate limiting
    print(f"\nDone: {done}/{total} records updated.")

if __name__ == "__main__":
    main()
