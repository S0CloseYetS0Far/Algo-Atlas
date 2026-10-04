class Solution {
public:
    double minTime(int n, int k, int m, vector<int>& time, vector<double>& mul) {
        int u = 1 << n;
        // compute the maximum of each subset of time
        vector<int> max_time(u);
        for (int i = 0; i < n; i++) {
            int t = time[i];
            int high_bit = 1 << i;
            for (int mask = 0; mask < high_bit; mask++) {
                max_time[high_bit | mask] = max({max_time[high_bit | mask], max_time[mask], t});
            }
        }
        // set entries of max_time for subsets of size greater than k to inf
        for (uint32_t i = 0; i < u; i++) {
            if (popcount(i) > k) {
                max_time[i] = INT_MAX;
            }
        }

        vector dis(m, vector<double>(u, DBL_MAX));
        using T = tuple<double, int, int>;
        priority_queue<T, vector<T>, greater<>> pq;

        auto push = [&](double d, int stage, int mask) {
            if (d < dis[stage][mask]) {
                dis[stage][mask] = d;
                pq.emplace(d, stage, mask);
            }
        };

        push(0, 0, u - 1); // starting point

        while (!pq.empty()) {
            auto [d, stage, left] = pq.top();
            pq.pop();
            if (left == 0) { // everyone has crossed the river
                return d;
            }
            if (d > dis[stage][left]) {
                continue;
            }
            // enumerate the group `sub` taking one boat
            for (int sub = left; sub > 0; sub = (sub - 1) & left) {
                if (max_time[sub] == INT_MAX) {
                    continue;
                }
                // sub crosses the river
                double cost = max_time[sub] * mul[stage];
                int cur_stage = (stage + int(cost)) % m; // stage after crossing
                // everyone has crossed the river
                if (sub == left) {
                    push(d + cost, cur_stage, 0);
                    continue;
                }
                // enumerate who comes back (can be someone who crossed earlier)
                for (int s = (u - 1) ^ left ^ sub, lb; s > 0; s ^= lb) {
                    lb = s & -s;
                    double return_time = max_time[lb] * mul[cur_stage];
                    push(d + cost + return_time, (cur_stage + int(return_time)) % m, left ^ sub ^ lb);
                }
            }
        }
        return -1;
    }
};
