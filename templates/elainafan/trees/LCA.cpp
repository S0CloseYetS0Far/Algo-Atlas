class TreeAncestor {
    vector<int> depth;       // depth
    vector<vector<int>> pa;  // 2^i-th ancestor

public:
    TreeAncestor(vector<vector<int>>& edges) {  // pass the edge list directly
        int n = edges.size() + 1;
        int m = bit_width(unsigned(n));
        vector<vector<int>> ma(n);  // still 0-based here
        for (auto& p : edges) {
            int x = p[0], y = p[1];
            ma[x].push_back(y);
            ma[y].push_back(x);
        }
        depth.resize(n);
        pa.resize(n, vector<int>(m, -1));
        auto dfs = [&](this auto&& dfs, int x, int fa) -> void {
            pa[x][0] = fa;
            for (int y : ma[x]) {
                if (y == fa) continue;
                depth[y] = depth[x] + 1;
                dfs(y, x);
            }
            return;
        };  // precompute depths
        dfs(0, -1);
        for (int i = 0; i < m - 1; i++) {
            for (int x = 0; x < n; x++) {
                if (pa[x][i] == -1) continue;
                pa[x][i + 1] = pa[pa[x][i]][i];
            }
        }  // precompute 2^i-th ancestors
    }

    int get_depth(int x) { return depth[x]; }  // get the depth of a node

    int get_kth_ancestor(int node, int k) {
        for (int x = k; node != -1 && x > 0; x -= lowbit(x)) {
            node = pa[node][countr_zero((unsigned)x)];
        }
        return node;
    }  // get the k-th ancestor, binary lifting similar to a Fenwick tree

    int get_lca(int x, int y) {
        if (depth[x] > depth[y]) swap(x, y);
        y = get_kth_ancestor(y, depth[y] - depth[x]);  // first jump to the same depth
        if (x == y) return x;
        for (int i = pa[x].size() - 1; i >= 0; i--) {
            int px = pa[x][i], py = pa[y][i];
            if (px != py) {
                x = px, y = py;
            }  // binary lifting jump
        }
        return pa[x][0];
    }  // get the lowest common ancestor
};