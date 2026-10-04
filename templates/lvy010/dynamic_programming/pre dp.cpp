/*
3077
Optimize 2D DP with prefix sum
to select k weighted subarrays for maximum total strength
maintain pre-optimal value to cut one loop to O(nk).
*/
class Solution {
public:
    long long maximumStrength(vector<int> &nums, int k) {
        int n = nums.size();
        vector<long long> s(n + 1);
        for (int i = 0; i < n; i++) {
            s[i + 1] = s[i] + nums[i];
        }
        vector<vector<long long>> f(k + 1, vector<long long>(n + 1));
        for (int i = 1; i <= k; i++) {
            f[i][i - 1] = LLONG_MIN;
            long long mx = LLONG_MIN;
            int w = (k - i + 1) * (i % 2 ? 1 : -1);
            // j can't be too small or too large: leave i-1 numbers before it and k-i numbers after it
            for (int j = i; j <= n - k + i; j++) {
                mx = max(mx, f[i - 1][j - 1] - s[j - 1] * w);
                f[i][j] = max(f[i][j - 1], s[j] * w + mx);
            }
        }
        return f[k][n];
    }
};
