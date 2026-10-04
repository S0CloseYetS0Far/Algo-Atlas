![image-20251216181717422](image-20251216181717422.png)

This is a **tree knapsack DP** problem that combines =="dependency relationships" and a "discount mechanism"==. Let's understand it directly through Example 3.

![image-20251216181840826](image-20251216181840826.png)

------

### **1. Modeling the Problem (State Definition)**

- **State meaning**: `f[j][k]` is the maximum profit in the subtree of `x` using budget `j`, given that the parent's state is `k`.

- `k=0`: the parent was **not bought** → the current node `x` gets **no discount** (full price `present[x]`)
- `k=1`: the parent **was bought** → the current node `x` gets **50% off** (price `present[x]/2`)

💡 Why do we need `k`? Because the problem states: **a child node gets the discount only if its parent was bought**.

------

### **2. State Transitions**

Two steps:

#### **(1) Merge the subtree results (knapsack merge)**

- ==For each child `y`, merge all of its possible budget allocations (`fy[jy][k]`) into the current node `x`'s `sub_f[j][k]`==.
- Similar to a "group knapsack": each subtree is a group of items, and exactly one budget allocation must be chosen.

#### **(2) Decide for the current node (buy or not)**

- **Don't buy** `x`: profit = max subtree profit (children see "parent not bought" → `sub_f[j][0]`)
- **Buy** `x`: profit = subtree profit (children see "parent bought" → `sub_f[j-cost][1]`) + `(future[x] - cost)`
  (`cost` is either full price or half price, depending on the parent's state `k`)

------

### **3. Initialization**

- `sub_f` starts as all zeros (with no children, the profit is 0)
- The recursion computes from the leaves upward (post-order traversal)

------

### **4. Traversal Order**

- **DFS post-order traversal**: process all children first, ==then the current node (it depends on the subproblem results==)
- **Reverse-order knapsack loop**: `j` goes from `budget` down to `0`, ==to avoid choosing the same subtree more than once==

------

### **5. Final Answer**

- `dfs(0)[budget][0]`: starting from the root (node 0) with total budget `budget`, where the root **has no parent** (treated as `k=0`, no discount)

------

### 🌰 An Example

Suppose:

- Node 0 is the root, price 10, future price 15
- Node 1 is a child, price 8, future price 12
- Budget = 12

**Decision process**:

1. If we **don't buy the root**: we can only spend 12 on the child (no discount, price 8), profit = 12-8=4
2. If we **buy the root**: spend 10, leaving 2 → the child gets 50% off (price 4), but there isn't enough money → profit = 15-10=5
   **The final choice is option 2 (profit 5)**

This is ==DP automatically comparing all possibilities==

```cpp
class Solution {
public:
    int maxProfit(int n, vector<int>& present, vector<int>& future, vector<vector<int>>& hierarchy, int budget) 
    {
        vector<vector<int>> g(n);
        for (auto& e : hierarchy) {
            g[e[0] - 1].push_back(e[1] - 1);
        }//build the tree - parent points to child

        auto dfs = [&](this auto&& dfs, int x) -> vector<array<int, 2>> 
        {
            // compute the max total profit obtainable from all of x's child subtrees y (x not bought, x bought)
            vector<array<int, 2>> sub_f(budget + 1);
            for (int y : g[x]) {
                auto fy = dfs(y);
                for (int j = budget; j >= 0; j--) {
                    // enumerate jy, the budget given to subtree y
                    // treat it as an item with volume jy and value fy[jy][k]
                    for (int jy = 0; jy <= j; jy++) {
                        for (int k = 0; k < 2; k++) { 
                            // k=0 means x is not bought, k=1 means x is bought
                            sub_f[j][k] = max(sub_f[j][k], sub_f[j - jy][k] + fy[jy][k]);
                        }
                    }
                }
            }

            // compute the max total profit obtainable from subtree x (x's parent not bought, x's parent bought)
            vector<array<int, 2>> f(budget + 1);
            for (int j = 0; j <= budget; j++) {
                for (int k = 0; k < 2; k++) { 
                    // k=0 means x's parent is not bought, k=1 means x's parent is bought
                    int cost = present[x] / (k + 1);
                    if (j >= cost) 
                    {
                        // don't buy x: transition from sub_f[j][0]
                        // buy x: transition from sub_f[j-cost][1], since for the subtree the parent is definitely bought
                        f[j][k] = max(sub_f[j][0], sub_f[j - cost][1] + future[x] - cost);
                    } 
                    else 
                    { 
                        // can only skip buying x
                        f[j][k] = sub_f[j][0];
                    }
                }
            }
            return f;
        };
        return dfs(0)[budget][0];
    }
};
```
