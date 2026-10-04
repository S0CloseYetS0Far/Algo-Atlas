#include <bits/stdc++.h>
using namespace std;

using ll = long long;

pair<ll, int> treeSlidingWindow(
    int n,
    const vector<vector<pair<int, int>>>& g,  // adjacency list: {to, weight}
    const vector<int>& color                  // "color"/value of each node
) {
    pair<ll, int> best = {-1, 0};  // {max_length, start_depth}
    vector<ll> dis = {0};           // path prefix sums
    unordered_map<int, int> last;   // color -> depth of its last occurrence + 1

    function<void(int, int, int)> dfs = [&](int u, int fa, int top_depth) {
        int c = color[u];
        int old = last[c];
        top_depth = max(top_depth, old);  // update the window's left boundary

        // update the best answer: length = current prefix sum - prefix sum at window start
        ll len = dis.back() - dis[top_depth];
        if (len > best.first || (len == best.first && top_depth < best.second)) {
            best = {len, top_depth};
        }

        last[c] = dis.size();  // record the current color's position

        for (auto& [v, w] : g[u]) {
            if (v != fa) {
                dis.push_back(dis.back() + w);
                dfs(v, u, top_depth);
                dis.pop_back();
            }
        }

        last[c] = old;  // restore on backtrack
    };

    dfs(0, -1, 0);
    return best;
}

// example main function
int main() {
    int n;
    cin >> n;
    vector<int> color(n);
    for (int i = 0; i < n; ++i) cin >> color[i];

    vector<vector<pair<int, int>>> g(n);
    for (int i = 0; i < n - 1; ++i) {
        int x, y, w;
        cin >> x >> y >> w;
        g[x].emplace_back(y, w);
        g[y].emplace_back(x, w);
    }

    auto [max_len, start_depth] = treeSlidingWindow(n, g, color);
    cout << max_len << " " << start_depth << endl;
    return 0;
}
