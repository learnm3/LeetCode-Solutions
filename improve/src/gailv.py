import numpy as np
import scipy.stats as stats
import matplotlib.pyplot as plt
import pandas as pd

# 设置随机种子，保证结果可复现
np.random.seed(42)

# 总体参数
mu_true = 100
sigma_true = 15

# ------------------------------
# 任务1：单次区间估计（理解原理）
# ------------------------------
print("="*50)
print("任务1：单次区间估计（n=30）")
print("="*50)

# 抽取一个样本
n1 = 30
sample = np.random.normal(mu_true, sigma_true, n1)
x_bar = np.mean(sample)
s = np.std(sample, ddof=1)

# 已知方差时的区间（z区间）
z_alpha = stats.norm.ppf(0.975)   # 95% 双侧分位数
margin_z = z_alpha * (sigma_true / np.sqrt(n1))
ci_z = (x_bar - margin_z, x_bar + margin_z)

# 未知方差时的区间（t区间）
t_alpha = stats.t.ppf(0.975, df=n1-1)
margin_t = t_alpha * (s / np.sqrt(n1))
ci_t = (x_bar - margin_t, x_bar + margin_t)

print(f"样本均值 = {x_bar:.2f}, 样本标准差 = {s:.2f}")
print(f"已知方差（z）95% CI: [{ci_z[0]:.2f}, {ci_z[1]:.2f}] 包含真值? {ci_z[0] <= mu_true <= ci_z[1]}")
print(f"未知方差（t）95% CI: [{ci_t[0]:.2f}, {ci_t[1]:.2f}] 包含真值? {ci_t[0] <= mu_true <= ci_t[1]}\n")

# ------------------------------
# 任务2：多次模拟，评估不同置信水平下覆盖率（n=30，t区间）
# ------------------------------
print("="*50)
print("任务2：多次模拟（N=1000，n=30）—— t区间")
print("="*50)

N_sim = 1000
n2 = 30
conf_levels = [0.90, 0.95, 0.99]
coverages = {}

for conf in conf_levels:
    alpha = 1 - conf
    t_crit = stats.t.ppf(1 - alpha/2, df=n2-1)
    count_cover = 0
    for _ in range(N_sim):
        samp = np.random.normal(mu_true, sigma_true, n2)
        xb = np.mean(samp)
        sd = np.std(samp, ddof=1)
        margin = t_crit * (sd / np.sqrt(n2))
        lower, upper = xb - margin, xb + margin
        if lower <= mu_true <= upper:
            count_cover += 1
    coverages[conf] = count_cover / N_sim
    print(f"名义置信水平 {conf*100:.0f}% -> 实际覆盖率 = {coverages[conf]*100:.2f}%")

# 绘制条形图
plt.figure(figsize=(6,4))
names = [f"{int(c*100)}%" for c in conf_levels]
actuals = [coverages[c]*100 for c in conf_levels]
plt.bar(names, actuals, color='skyblue')
plt.axhline(y=90, color='r', linestyle='--', label='名义水平 90%')
plt.axhline(y=95, color='g', linestyle='--', label='名义水平 95%')
plt.axhline(y=99, color='orange', linestyle='--', label='名义水平 99%')
plt.ylabel("实际覆盖率 (%)")
plt.title("不同置信水平下 t 区间的实际覆盖率 (n=30)")
plt.legend()
plt.ylim(85, 100)
plt.show()

# ------------------------------
# 任务3：样本量影响分析（固定置信水平95%，t区间）
# ------------------------------
print("\n"+"="*50)
print("任务3：样本量对覆盖率的影响（95%置信水平，t区间）")
print("="*50)

sample_sizes = [5, 10, 30, 50, 100]
coverage_vs_n = []
for n in sample_sizes:
    t_crit = stats.t.ppf(0.975, df=n-1)
    cover_cnt = 0
    for _ in range(N_sim):
        samp = np.random.normal(mu_true, sigma_true, n)
        xb = np.mean(samp)
        sd = np.std(samp, ddof=1)
        margin = t_crit * (sd / np.sqrt(n))
        if xb - margin <= mu_true <= xb + margin:
            cover_cnt += 1
    cov = cover_cnt / N_sim
    coverage_vs_n.append(cov)
    print(f"n = {n:3d} -> 实际覆盖率 = {cov*100:.2f}%")

# 绘制折线图
plt.figure(figsize=(8,5))
plt.plot(sample_sizes, [c*100 for c in coverage_vs_n], 'bo-', linewidth=2, markersize=8)
plt.axhline(y=95, color='r', linestyle='--', label='名义水平 95%')
plt.xlabel("样本量 n")
plt.ylabel("实际覆盖率 (%)")
plt.title("样本量对 t 区间覆盖率的影响 (置信水平 95%)")
plt.legend()
plt.grid(True, alpha=0.3)
plt.show()

# ------------------------------
# 任务4：非正态总体下的稳健性（固定n=30，置信水平95%，t区间）
# ------------------------------
print("\n"+"="*50)
print("任务4：非正态总体下的稳健性（n=30，95% t区间）")
print("="*50)

# 4.1 指数分布（均值为100，则 rate=0.01）
# 指数分布均值 = 1/λ, 方差 = 1/λ^2 = 10000
lambda_exp = 0.01
mu_exp = 100

cover_exp = 0
t_crit = stats.t.ppf(0.975, df=n2-1)
for _ in range(N_sim):
    samp = np.random.exponential(1/lambda_exp, n2)  # scale = 1/λ = 100
    xb = np.mean(samp)
    sd = np.std(samp, ddof=1)
    margin = t_crit * (sd / np.sqrt(n2))
    if xb - margin <= mu_exp <= xb + margin:
        cover_exp += 1
cov_exp = cover_exp / N_sim
print(f"指数分布（均值100）: 实际覆盖率 = {cov_exp*100:.2f}%")

# 4.2 均匀分布（范围选择使均值为100，例如 U(50,150)）
# 均匀分布均值 = (a+b)/2 = 100, 方差 = (b-a)^2/12
a, b = 50, 150
mu_unif = 100
cover_unif = 0
for _ in range(N_sim):
    samp = np.random.uniform(a, b, n2)
    xb = np.mean(samp)
    sd = np.std(samp, ddof=1)
    margin = t_crit * (sd / np.sqrt(n2))
    if xb - margin <= mu_unif <= xb + margin:
        cover_unif += 1
cov_unif = cover_unif / N_sim
print(f"均匀分布 U(50,150): 实际覆盖率 = {cov_unif*100:.2f}%")

# 输出汇总表格
print("\n=== 汇总表格（n=30，t区间，95%名义水平）===")
print(pd.DataFrame({
    "总体分布": ["正态分布", "指数分布", "均匀分布"],
    "实际覆盖率(%)": [coverages[0.95]*100, cov_exp*100, cov_unif*100]
}))

# 可选：绘制三种分布的覆盖率对比
plt.figure(figsize=(6,4))
plt.bar(["正态", "指数", "均匀"], [coverages[0.95]*100, cov_exp*100, cov_unif*100], color=['royalblue', 'salmon', 'lightgreen'])
plt.axhline(y=95, color='r', linestyle='--', label='名义水平 95%')
plt.ylabel("实际覆盖率 (%)")
plt.title("不同总体分布下 t 区间的覆盖率 (n=30)")
plt.legend()
plt.ylim(85, 100)
plt.show()