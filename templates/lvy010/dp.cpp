class Solution {
public:
    int minMaxWaitingTime(vector<int>& demand, vector<int>& fuel) {
        unordered_map<int, pair<int, int>> memo;

        // fuel pump 0 becomes free after wait0 seconds, with fuel0 fuel remaining
        // fuel pump 1 becomes free after wait1 seconds, with fuel1 fuel remaining
        auto dfs = [&](this auto&& dfs, int i, int wait0, int wait1, int fuel0, int fuel1) -> pair<int, int> {
            if (i == demand.size()) {
                return {};
            }

            int key = i << 24 | wait0 << 18 | wait1 << 12 | fuel0 << 6 | fuel1;
            if (memo.contains(key)) {
                return memo[key];
            }

            int max_num = 0;
            int best_wait_time = 0;
            int d = demand[i];

            // choose pump 0: wait wait0 seconds to start refueling; pump 1's wait time decreases by wait0 seconds
            if (d <= fuel0) {
                auto [num, time] = dfs(i + 1, d, max(wait1 - wait0, 0), fuel0 - d, fuel1);
                max_num = num + 1;
                best_wait_time = max(time, wait0);
            }

            // choose pump 1: wait wait1 seconds to start refueling; pump 0's wait time decreases by wait1 seconds
            if (d <= fuel1) {
                auto [num, time] = dfs(i + 1, max(wait0 - wait1, 0), d, fuel0, fuel1 - d);
                num++;
                time = max(time, wait1);
                if (num > max_num || num == max_num && time < best_wait_time) {
                    max_num = num;
                    best_wait_time = time;
                }
            }

            return memo[key] = {max_num, best_wait_time};
        };

        auto [max_num, best_wait_time] = dfs(0, 0, 0, fuel[0], fuel[1]);
        if (max_num == 0) {
            return -1;
        }
        return best_wait_time;
    }
};
