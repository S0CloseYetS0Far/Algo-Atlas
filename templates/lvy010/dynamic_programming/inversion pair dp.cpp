/*
The difficulty of this problem lies in the state design. The core idea is to count the inversions between the current element and the [numbers not yet placed],
so subproblems don't need to consider the numbers already placed on the right. Thinking this way simplifies the inversion count,
so that each subproblem only needs to consider inversions between the current element and the unplaced numbers to the left,
rather than inversions between the current element and the numbers already placed on the right
*/
class Solution {
    const int MOD = 1'000'000'007;
public:
    int numberOfPermutations(int n, vector<vector<int>>& requirements) {
        vector<int> req(n, -1);
        req[0] = 0;
        for (auto& p : requirements) {
            req[p[0]] = p[1];
        }
        if (req[0]) {
            return 0;
        }

        int m = ranges::max(req);
        vector<vector<int>> memo(n, vector<int>(m + 1, -1)); // -1 means not computed yet
        auto dfs = [&](auto&& dfs, int i, int j) -> int {
            if (i == 0) {
                return 1;
            }
            int& res = memo[i][j]; // note: this is a reference
            if (res != -1) { // already computed
                return res;
            }
            res = 0;
            if (int r = req[i - 1]; r >= 0) {
                if (j >= r && j - i <= r) {
                    res = dfs(dfs, i - 1, r);
                }
            } else {
                for (int k = 0; k <= min(i, j); k++) {
                    res = (res + dfs(dfs, i - 1, j - k)) % MOD;
                }
            }
            return res;
        };
        return dfs(dfs, n - 1, req[n - 1]);
    }
};
