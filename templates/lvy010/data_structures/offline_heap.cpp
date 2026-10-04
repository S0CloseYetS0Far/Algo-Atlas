class Solution {
/*
lc 2503 Maximum Score From Grid Queries with

Online processing: answer each query as soon as it arrives
Offline processing: collect all queries, then process and answer them together.
*/
    const int dirs[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
public:
    vector<int> maxPoints(vector<vector<int>> &grid, vector<int> &queries) {
        // sort query indices by query value in ascending order, for offline processing
        int k = queries.size(), id[k];
        iota(id, id + k, 0);
        sort(id, id + k, [&](int i, int j) { return queries[i] < queries[j]; });

        vector<int> ans(k);
        priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, greater<>> pq; // min-heap
        pq.emplace(grid[0][0], 0, 0);
        grid[0][0] = 0; // serves as the vis (visited) array
        int m = grid.size(), n = grid[0].size(), cnt = 0;
        for (int qi : id) {
            int q = queries[qi];
            while (!pq.empty() && get<0>(pq.top()) < q) {
                ++cnt;
                auto[_, i, j] = pq.top();
                pq.pop();
                for (auto &d : dirs) { // enumerate the four neighboring cells
                    int x = i + d[0], y = j + d[1];
                    if (0 <= x && x < m && 0 <= y && y < n && grid[x][y]) {
                        pq.emplace(grid[x][y], x, y);
                        grid[x][y] = 0; // serves as the vis (visited) array
                    }
                }
            }
            ans[qi] = cnt;
        }
        return ans;
    }
};
