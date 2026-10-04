//Find the endpoints of a tree's diameter, a slight modification of the diameter algorithm
//Two BFS/DFS passes: first find the farthest node u from any node, then the farthest node v from u; u-v is the diameter
class Solution {
    int n;
    vector<vector<int>> graph;
    string res;
    vector<int> dis;
    void Dfs(int x, int fa) {
        for (const auto& y : graph[x]) {
            if (y != fa) {
                dis[y] = dis[x] + 1;
                Dfs(y, x);
            }
        }
    }
    int Round(int x) {
        dis[x] = 0;
        Dfs(x, -1);
        int ret = max_element(dis.begin(), dis.end()) - dis.begin();
        int maxi = dis[ret];
        for (int i = 0; i < n; i++) {
            if (dis[i] == maxi) {
                res[i] = '1';
            }
        }
        return ret;
    }

public:
    string findSpecialNodes(int n, vector<vector<int>>& edges) {
        Solution::n = n;
        graph.assign(n, {});
        res.assign(n, '0');
        dis.resize(n);
        for (const auto& e : edges) {
            int a = e[0];
            int b = e[1];
            graph[a].push_back(b);
            graph[b].push_back(a);
        }
        Round(Round(0));
        return res;
    }
};
