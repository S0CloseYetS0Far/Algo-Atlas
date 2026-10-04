
//Binary lifting on trees
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

const int MAXN = 1e5 + 5;
const int LOG = 20; // 2^20 is enough to cover 1e5 nodes

vector<int> adj[MAXN];
int up[LOG][MAXN]; // up[k][u] is the ancestor of u 2^k steps up
int depth[MAXN];   // node depths

// DFS to precompute depths and 2^0-level ancestors (parents)
void dfs(int u, int fa) {
    up[0][u] = fa;
    depth[u] = depth[fa] + 1;
    for (int v : adj[u]) {
        if (v != fa) dfs(v, u);
    }
}

// precompute the binary lifting table
void init(int root, int n) {
    dfs(root, 0);
    for (int k = 1; k < LOG; k++) {
        for (int u = 1; u <= n; u++) {
            up[k][u] = up[k-1][up[k-1][u]];
        }
    }
}

// move u up by k steps
int jump(int u, int k) {
    for (int i = 0; i < LOG; i++) {
        if (k & (1 << i)) u = up[i][u];
    }
    return u;
}

// compute the LCA
int lca(int u, int v) {
    if (depth[u] < depth[v]) swap(u, v);
    // 1. lift u to the same depth as v
    u = jump(u, depth[u] - depth[v]);
    if (u == v) return u;
    // 2. lift both nodes together until their parents are the same
    for (int k = LOG-1; k >= 0; k--) {
        if (up[k][u] != up[k][v]) {
            u = up[k][u];
            v = up[k][v];
        }
    }
    return up[0][u];
}

int main() {
    int n, m;
    cin >> n >> m;
    // build the graph
    for (int i = 0; i < n-1; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    // initialize (root is node 1)
    init(1, n);
    // queries
    while (m--) {
        int u, v;
        cin >> u >> v;
        cout << lca(u, v) << endl;
    }
    return 0;
}
