// Returns a 2D list where entry (i,j) is the shortest path length from i to j
// If j is unreachable from i, the shortest path length is LLONG_MAX / 2
// Negative edge weights are allowed
// If, after computation, some i has a shortest path from i to i less than 0, the graph has a negative cycle
// Nodes are numbered from 0 to n-1
// Time complexity O(n^3 + m), where m is the length of edges
vector<vector<long long>> shortestPathFloyd(int n, vector<vector<int>>& edges) {
    const long long INF = LLONG_MAX / 2; // prevent overflow on addition
    vector f(n, vector<long long>(n, INF));
    for (int i = 0; i < n; i++) {
        f[i][i] = 0;
    }

    for (auto& e : edges) {
        int x = e[0], y = e[1];
        long long wt = e[2];
        f[x][y] = min(f[x][y], wt); // with parallel edges, take the minimum weight
        f[y][x] = min(f[y][x], wt); // undirected graph
    }

    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            if (f[i][k] == INF) { // optimization for sparse graphs
                continue;
            }
            for (int j = 0; j < n; j++) {
                f[i][j] = min(f[i][j], f[i][k] + f[k][j]);
            }
        }
    }
    return f;
}
