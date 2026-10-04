/*
Rerooting DP
First, DFS to compute the total number of edges to reverse with 0 as the root
Then reroot to quickly derive the reversal count with every node as the root
Signs (+/-) mark the forward/reverse direction of edges
*/
class Solution {
    vector<vector<pair<int, int>>> g;
    vector<int> ans;

    void dfs(int x, int fa) {
        for (auto &[y, dir] : g[x]) {
            if (y != fa) {
                ans[0] += dir < 0;
                dfs(y, x);
            }
        }
    }

    void reroot(int x, int fa) {
        for (auto &[y, dir] : g[x]) {
            if (y != fa) {
                ans[y] = ans[x] + dir; // dir is the "delta" when moving the root from x to y
                reroot(y, x);
            }
        }
    }

public:
    vector<int> minEdgeReversals(int n, vector<vector<int>> &edges) {
        g.resize(n);
        for (auto &e : edges) {
            int x = e[0], y = e[1];
            g[x].emplace_back(y, 1);
            g[y].emplace_back(x, -1); // going from y to x requires reversing
        }

        ans.resize(n);
        dfs(0, -1);
        reroot(0, -1);
        return ans;
    }
};
