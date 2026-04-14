import matplotlib.pyplot as plt
import numpy as np

schemes = ['方案0\n(限制4人)', '方案1\n(拿不到释放)', '方案2\n(奇偶不对称)']

# 平均等待时间
avg_wait = [41925464.00, 8198019.80, 13422663.40]

throughput = [50, 50, 50]

# 每个哲学家等待时间
p0_wait = [42445367, 9271657, 11250098]
p1_wait = [42244783, 9110211, 12202532]
p2_wait = [42293366, 10086268, 14151779]
p3_wait = [38206190, 6409853, 18299677]
p4_wait = [44437614, 6112110, 11209231]

total_wait_time = [
    209627320,   # 方案0
    40990099,    # 方案1
    67113317     # 方案2
]

plt.rcParams['font.sans-serif'] = ['SimHei', 'WenQuanYi Zen Hei']
plt.rcParams['axes.unicode_minus'] = False

# --------------------------------------------------------
# 图1：平均等待时间对比
# --------------------------------------------------------
plt.figure(figsize=(10, 6))
plt.bar(schemes, avg_wait, color=['#3498db', '#e74c3c', '#2ecc71'], width=0.6)
plt.title('不同方案平均等待时间对比', fontsize=16)
plt.ylabel('等待时间（微秒）', fontsize=12)
plt.grid(axis='y', linestyle='--', alpha=0.3)
plt.tight_layout()
plt.savefig('图1_平均等待时间.png', dpi=300)
plt.close()

# --------------------------------------------------------
# 图2：总运行效率对比（总等待时间越少越快）
# --------------------------------------------------------
plt.figure(figsize=(10, 6))
plt.bar(schemes, total_wait_time, color=['#9b59b6', '#f39c12', '#1abc9c'], width=0.6)
plt.title('不同方案总等待时间（系统效率对比）', fontsize=16)
plt.ylabel('总等待时间（越小越快）', fontsize=12)
plt.grid(axis='y', linestyle='--', alpha=0.3)
plt.tight_layout()
plt.savefig('图2_系统总效率.png', dpi=300)
plt.close()

# --------------------------------------------------------
# 图3：各哲学家等待时间分布
# --------------------------------------------------------
x = np.arange(len(schemes))
width = 0.15

plt.figure(figsize=(14, 7))
plt.bar(x - 2*width, p0_wait, width, label='哲学家0')
plt.bar(x - width,   p1_wait, width, label='哲学家1')
plt.bar(x,           p2_wait, width, label='哲学家2')
plt.bar(x + width,   p3_wait, width, label='哲学家3')
plt.bar(x + 2*width, p4_wait, width, label='哲学家4')

plt.title('各哲学家在不同方案下的等待时间', fontsize=16)
plt.ylabel('等待时间（微秒）', fontsize=12)
plt.xticks(x, schemes)
plt.legend()
plt.grid(axis='y', linestyle='--', alpha=0.3)
plt.tight_layout()
plt.savefig('图3_各哲学家等待时间.png', dpi=300)
plt.close()

# 图4：性能综合雷达图（等待时间、效率、公平性、复杂度）
categories = ['总耗时', '效率', '公平性', '实现难度', '资源占用']
angles = np.linspace(0, 2 * np.pi, len(categories), endpoint=False).tolist()
angles += angles[:1]

score0 = [30, 40, 95, 70, 80]   # 方案0
score1 = [95, 95, 80, 95, 70]   # 方案1
score2 = [80, 80, 90, 80, 95]   # 方案2

score0 += score0[:1]
score1 += score1[:1]
score2 += score2[:1]

plt.figure(figsize=(8, 8))
plt.polar(angles, score0, 'o-', label='方案0', linewidth=2)
plt.polar(angles, score1, 'o-', label='方案1', linewidth=2)
plt.polar(angles, score2, 'o-', label='方案2', linewidth=2)
plt.fill(angles, score0, alpha=0.2)
plt.fill(angles, score1, alpha=0.2)
plt.fill(angles, score2, alpha=0.2)

plt.xticks(angles[:-1], categories)
plt.legend(loc='upper right')
plt.title('方案性能综合对比', fontsize=16)
plt.tight_layout()
plt.savefig('图4_性能雷达图.png', dpi=300)
plt.close()

print("✅ 4 张正确图表已生成！")