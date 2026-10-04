class Solution {
public:
    long long subtreeInversionSum(vector<vector<int>>& edges, vector<int>& nums, int k) {
        int n = nums.size();
        vector<vector<int>> g(n);
        for (auto& e : edges) {
            int x = e[0], y = e[1];
            g[x].push_back(y);
            g[y].push_back(x);
        }

        vector memo(n, vector<array<long long, 2>>(k, {LLONG_MIN, LLONG_MIN}));
        auto dfs = [&](this auto&& dfs, int x, int fa, int cd, bool parity) -> long long {
            auto& res = memo[x][cd][parity]; // note: this is a reference
            if (res != LLONG_MIN) {
                return res;
            }

            // don't flip
            res = parity ? -nums[x] : nums[x];
            for (int y : g[x]) {
                if (y != fa) {
                    res += dfs(y, x, max(cd - 1, 0), parity);
                }
            }

            // flip
            if (cd == 0) {
                long long s = parity ? nums[x] : -nums[x];
                for (int y : g[x]) {
                    if (y != fa) {
                        s += dfs(y, x, k - 1, !parity); // reset CD
                    }
                }
                res = max(res, s);
            }

            return res;
        };

        return dfs(0, -1, 0, 0);
    }
};
