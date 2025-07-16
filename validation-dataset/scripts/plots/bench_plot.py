# Importing necessary libraries again due to reset environment
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.lines import Line2D

# Categorized benchmark data
benchmarks = [
    {"name": "openmp_1025", "category": "openmp (OMP)", "original": 6148.4, "human": 140.8, "llm": 6098.2},
    {"name": "openmp_1038", "category": "openmp (OMP)", "original": 11880.6, "human": 11546.4, "llm": 11840.6},
    {"name": "openmp_1144", "category": "openmp (OMP)", "original": 799.4, "human": 678.8, "llm": 812.6},
    {"name": "openmp_122", "category": "openmp (OMP)", "original": 8637.6, "human": 803.8, "llm": 8549.6},
    {"name": "openmp_1277", "category": "openmp (OMP)", "original": 7465.2, "human": 2099.2, "llm": None},
    {"name": "openmp_1402", "category": "openmp (OMP)", "original": 5055.4, "human": 360.8, "llm": None},
    {"name": "openmp_1423", "category": "openmp (OMP)", "original": 9874.6, "human": 3887.4, "llm": None},
    {"name": "openmp_1520", "category": "openmp (OMP)", "original": 6942.6, "human": 3055.6, "llm": 1770.8},
    {"name": "openmp_1583", "category": "openmp (OMP)", "original": 5324.2, "human": 407.2, "llm": 5321.2},
    {"name": "openmp_1693", "category": "openmp (OMP)", "original": 6238.0, "human": 2504.0, "llm": None},
    {"name": "openmp_442", "category": "openmp (OMP)", "original": 9395.6, "human": 5299.6, "llm": 9293.8},
    {"name": "openmp_674", "category": "openmp (OMP)", "original": 4480.0, "human": 538.8, "llm": 609.0},
    {"name": "openmp_744", "category": "openmp (OMP)", "original": 8681.2, "human": 6925.4, "llm": 7907.8},
    {"name": "openmp_767", "category": "openmp (OMP)", "original": 7053.8, "human": 359.0, "llm": None},
    {"name": "openmp_890", "category": "openmp (OMP)", "original": 3878.4, "human": 3397.4, "llm": 3311.4},
    {"name": "UE_006", "category": "UE", "original": 36479.0, "human": 31247.2, "llm": 7379.2},
    {"name": "RF_007", "category": "RF", "original": 3684.6, "human": 2432.6, "llm": 2794.4},
    {"name": "RF_009", "category": "RF", "original": 9082.2, "human": 7832.4, "llm": None},
    {"name": "UE_049", "category": "UE", "original": 6319.2, "human": 5187.0, "llm": None},
    {"name": "CE_074", "category": "CE", "original": 477.6, "human": 356.2, "llm": 526.0},
    {"name": "UE_125", "category": "UE", "original": 4317.2, "human": 964.2, "llm": 4341.2},
    {"name": "UE_162", "category": "UE", "original": 4872.2, "human": 151.2, "llm": 4871.8},
    {"name": "UE_166", "category": "UE", "original": 8745.2, "human": 8614.8, "llm": None},
    {"name": "UE_176", "category": "UE", "original": 3869.2, "human": 202.4, "llm": 165.4},
    {"name": "LI_215", "category": "LI", "original": 9399.8, "human": 1282.4, "llm": 1876.6},
    {"name": "CE_236", "category": "CE", "original": 3012.2, "human": 293.6, "llm": 3282.2},
    {"name": "LI_331", "category": "LI", "original": 28765.6, "human": 9.6, "llm": None},
    {"name": "LI_333", "category": "LI", "original": 8348.2, "human": 1386.4, "llm": 8321.0},
    {"name": "UE_384", "category": "UE", "original": 5343.0, "human": 15.0, "llm": 19.2},
    {"name": "RF_395", "category": "RF", "original": 7410.4, "human": 483.4, "llm": 196.0},
    {"name": "UE_402", "category": "UE", "original": 11046.2, "human": 1.0, "llm": None},
    {"name": "UE_403", "category": "UE", "original": 8012.6, "human": 7.0, "llm": None},
    {"name": "LI_553", "category": "LI", "original": 7887.4, "human": 7758.8, "llm": 4637.6},
    {"name": "LI_659", "category": "LI", "original": 12971.4, "human": 10146.6, "llm": 14592.8},
    {"name": "LI_879", "category": "LI", "original": 6758.8, "human": 167.2, "llm": 201.4},
]

names = [bm["name"] for bm in benchmarks]
original = [bm["original"] for bm in benchmarks]
human = [bm["human"] for bm in benchmarks]
llm = [bm["llm"] if bm["llm"] else 0 for bm in benchmarks]
llm_failed = [bm["llm"] is None for bm in benchmarks]
categories = [bm["category"] for bm in benchmarks]

# Plot
fig, ax = plt.subplots(figsize=(20, 8))
x = np.arange(len(names))
width = 0.25

ax.bar(x - width, original, width, label='Original', color='skyblue')
ax.bar(x, human, width, label='Human Optimized', color='limegreen')
ax.bar(x + width, llm, width, label='Best LLM (GPT-4o-mini with CoT)', color='orange')

# Mark LLM failures
for i, failed in enumerate(llm_failed):
    if failed:
        ax.text(x[i] + width, 100, '✗', ha='center', color='red', fontsize=14, fontweight='bold')

ax.set_ylabel('Mean Time (ms)')
ax.set_title('Performance Comparison by Categories')
ax.set_xticks(x)
ax.set_xticklabels(names, rotation=90)

legend_elements = [
    Line2D([0], [0], color='skyblue', lw=4),
    Line2D([0], [0], color='limegreen', lw=4),
    Line2D([0], [0], color='orange', lw=4),
    Line2D([0], [0], linestyle='', marker='x', color='red', markersize=14, markeredgewidth=3)
]

ax.legend(legend_elements, [
    'Original', 'Human Optimized', 'Best LLM (GPT-4o-mini with CoT)', 'LLM Failed'
])

plt.tight_layout()
plt.savefig("bench_plot.pdf")
plt.show()
