/*
Minimum Degree of a Connected Trio in a Graph

lower degree → higher degree
if degrees are equal, smaller id → larger id
Classic optimization for enumerating triangles (trios)
*/
class Solution {
public:
    int minTrioDegree(int n, vector<vector<int>>& edges) {
        vector<unordered_set<int>> d(n);
        for (auto &e : edges) {
            int u = e[0] - 1, v = e[1] - 1;
            d[u].insert(v), d[v].insert(u);
        }
        
        vector<vector<int>> adj(n);
        for (auto &e : edges) {
            int u = e[0] - 1, v = e[1] - 1;
            if (d[u].size() < d[v].size() || (d[u].size() == d[v].size() && u < v))
                adj[u].emplace_back(v);
            else
                adj[v].emplace_back(u);
        }

        int ans = INT_MAX;
        for (int u = 0; u < n; ++u)
            for (int v : adj[u])
                for (int w : adj[v])
                    if (d[u].count(w))
                        ans = min(ans, (int)(d[u].size() + d[v].size() + d[w].size() - 6));
        
        return ans == INT_MAX ? -1 : ans;
    }
};
