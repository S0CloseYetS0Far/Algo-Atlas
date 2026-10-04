/*
lc2876
builds reversed graph & uses topological sort to isolate cycle nodes
DFS backward to calculate total reachable nodes for every point in functional graph with exactly one outgoing edge per node.
*/

class Solution {
public:
    vector<int> countVisitedNodes(vector<int>& g) {
        int n = g.size();
        vector<vector<int>> rg(n); // reverse graph
        vector<int> deg(n);
        for (int x = 0; x < n; x++) {
            int y = g[x];
            rg[y].push_back(x);
            deg[y]++;
        }

        // topological sort to prune all tree branches off g
        // after topological sort, nodes with deg 1 are on the cycle, and nodes with deg 0 are on tree branches
        queue<int> q;
        for (int i = 0; i < n; i++) {
            if (deg[i] == 0) {
                q.push(i);
            }
        }
        while (!q.empty()) {
            int x = q.front(); q.pop();
            int y = g[x];
            deg[y]--;
            if (deg[y] == 0) {
                q.push(y);
            }
        }

        vector<int> ans(n);

        // traverse tree branches on the reverse graph
        auto rdfs = [&](this auto&& rdfs, int x, int depth) -> void {
            ans[x] = depth;
            for (int y : rg[x]) {
                if (deg[y] == 0) { // after topological sort, nodes on tree branches all have in-degree 0
                    rdfs(y, depth + 1);
                }
            }
        };

        for (int i = 0; i < n; i++) {
            if (deg[i] <= 0) {
                continue;
            }
            vector<int> ring;
            for (int x = i; ; x = g[x]) {
                deg[x] = -1; // mark the in-degree of cycle nodes as -1 to avoid revisiting
                ring.push_back(x); // collect the nodes on the cycle
                if (g[x] == i) {
                    break;
                }
            }
            for (int x : ring) {
                rdfs(x, ring.size()); // for convenience, use ring.size() as the initial depth
            }
        }

        return ans;
    }
};
