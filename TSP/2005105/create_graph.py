import os
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

os.makedirs("graphs/individuals", exist_ok=True)
data = pd.read_csv("tour_costs.csv")
sns.set(style="whitegrid")

data["Filename"] = data["Filename"].astype(str)
data["Filename"] = data["Filename"].str.replace("data/", "", regex=False)


plt.figure(figsize=(16, 8))
x = data["Filename"]
nearest = data["NearestNeighborHeuristic"]
greedy = data["GreedyHeuristic"]
cheapest = data["CheapestInsertionHeuristic"]

bar_width = 0.2
index = range(len(x))

plt.bar(index, nearest, width=bar_width, label="Nearest Neighbor", color="blue")
plt.bar([i + bar_width for i in index], greedy, width=bar_width, label="Greedy", color="green")
plt.bar([i + 2 * bar_width for i in index], cheapest, width=bar_width, label="Cheapest", color="orange")

plt.xticks([i + bar_width for i in index], x, rotation=90, fontsize=10)
plt.yscale("log")

plt.title("Comparison of Nearest Neighbor, Greedy, and Cheapest Insertion")
plt.ylabel("Costs (log scale)")
plt.xlabel("Filename")
plt.legend()
plt.tight_layout()

plt.savefig("graphs/compare_constructives.png")
plt.close()


# Individual graphs
for _, row in data.iterrows():
    file_name = row["Filename"]

    constructive_values = [row["NearestNeighborHeuristic"], row["GreedyHeuristic"], row["CheapestInsertionHeuristic"]]
    two_opt_values = [row["2opt-1"], row["2opt-2"], row["2opt-3"]]
    node_shift_values = [row["NodeShift-1"], row["NodeShift-2"], row["NodeShift-3"]]
    node_swap_values = [row["NodeSwap-1"], row["NodeSwap-2"], row["NodeSwap-3"]]

    constructive_labels = ["Nearest Neighbor", "Greedy", "Cheapest"]
    two_opt_labels = ["2opt-1", "2opt-2", "2opt-3"]
    node_shift_labels = ["NodeShift-1", "NodeShift-2", "NodeShift-3"]
    node_swap_labels = ["NodeSwap-1", "NodeSwap-2", "NodeSwap-3"]

    plt.figure(figsize=(15, 12))

    bar_width = 0.15
    index = np.arange(len(constructive_values))

    plt.bar(index, constructive_values, width=bar_width, label="Constructive Heuristics", color="red")
    plt.bar(index + bar_width, two_opt_values, width=bar_width, label="2opt Heuristic", color="green")
    plt.bar(index + 2 * bar_width, node_shift_values, width=bar_width, label="NodeShift Heuristic", color="orange")
    plt.bar(index + 3 * bar_width, node_swap_values, width=bar_width, label="NodeSwap Heuristic", color="blue")

    plt.xticks(index + 1.5 * bar_width, constructive_labels, rotation=45, fontsize=10)

    plt.title(f"Comparison of Algorithms for File: {file_name}")
    plt.ylabel("Costs")
    plt.xlabel("Heuristics")

    plt.legend()
    plt.tight_layout()
    plt.savefig(f"graphs/individuals/{file_name}.png")
    plt.close()