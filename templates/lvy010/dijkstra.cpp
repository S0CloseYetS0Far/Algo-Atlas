//3977
class Solution {
public:
    vector<long long> minTimeMaxPower(int n, vector<vector<int>>& edges, int power, vector<int>& cost, int source, int target) {
        vector<vector<pair<int, int>>> g(n);
        for (auto& e : edges) {
            g[e[0]].emplace_back(e[1], e[2]);
        }

        vector dis(n, vector<long long>(power + 1, LLONG_MAX));
        dis[source][power] = 0;
        // tuple{shortest path length, -remaining battery, node id}
        priority_queue<tuple<long long, int, int>, vector<tuple<long long, int, int>>, greater<>> pq;
        pq.emplace(0, -power, source);

        while (!pq.empty()) {
            auto [d, rem, x] = pq.top();
            pq.pop();
            rem = -rem;
            if (x == target) {
                return {d, rem};
            }
            if (d > dis[x][rem] || rem < cost[x]) {
                continue;
            }
            auto new_rem = rem - cost[x];
            for (auto& [y, w] : g[x]) {
                auto new_dis = d + w;
                if (new_dis < dis[y][new_rem]) {
                    dis[y][new_rem] = new_dis;
                    pq.emplace(new_dis, -new_rem, y);
                }
            }
        }

        return {-1, -1};
    }
};
