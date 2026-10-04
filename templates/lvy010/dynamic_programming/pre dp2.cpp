//3251
class Solution {
public:
    int countOfPairs(vector<int>& nums) {
        const int MOD = 1'000'000'007;
        int n = nums.size();
        int m = ranges::max(nums);
        vector f(n, vector<long long>(m + 1));
        vector<long long> s(m + 1);

        fill(f[0].begin(), f[0].begin() + nums[0] + 1, 1);
        for (int i = 1; i < n; i++) {
            partial_sum(f[i - 1].begin(), f[i - 1].end(), s.begin()); // prefix sums of f[i-1]
            for (int j = 0; j <= nums[i]; j++) {
                int max_k = j + min(nums[i - 1] - nums[i], 0);
                f[i][j] = max_k >= 0 ? s[max_k] % MOD : 0;
            }
        }

        return reduce(f[n - 1].begin(), f[n - 1].begin() + nums[n - 1] + 1, 0LL) % MOD;
    }
};



//3797
class Solution {
public:
    int numberOfRoutes(vector<string>& grid, int d) {
        constexpr int MOD = 1'000'000'007;
        int m = grid[0].size();
        vector<long long> sum_f(m + 1);
        vector<long long> sum(m + 1);

        for (int i = 0; i < grid.size(); i++) {
            auto& row = grid[i];
            // number of ways to move from row i-1 to row i
            vector<long long> f(m);
            for (int j = 0; j < m; j++) {
                if (row[j] == '#') {
                    continue;
                }
                if (i == 0) { // first row (starting point)
                    f[j] = 1; // DP initial value
                } else {
                    f[j] = sum[min(j + d, m)] - sum[max(j - d + 1, 0)];
                }
            }

            // prefix sums of f
            for (int j = 0; j < m; j++) {
                sum_f[j + 1] = (sum_f[j] + f[j]) % MOD;
            }

            // number of ways to move from row i to row i
            vector<long long> g(m);
            for (int j = 0; j < m; j++) {
                if (row[j] == '#') {
                    continue;
                }
                // can't stay in place, subtract f[j]
                g[j] = sum_f[min(j + d + 1, m)] - sum_f[max(j - d, 0)] - f[j];
            }

            // prefix sums of f[j] + g[j]
            for (int j = 0; j < m; j++) {
                sum[j + 1] = (sum[j] + f[j] + g[j]) % MOD;
            }
        }

        return (sum[m] + MOD) % MOD; // +MOD keeps the result non-negative
    }
};

//car wash
class Solution {
public:
    int numberOfRoutes(vector<string>& g, int d) {
        const int MOD = 1e9 + 7;
        int n = g.size(),m = g[0].size();
        int k = sqrt(d * d - 1);
        using ll = long long;
        vector f(n,vector(m,vector<ll>(2,0)));
        for(int j = 0;j < m;j++)if(g[n - 1][j] == '.'){
            f[n - 1][j][0] = 1;
        }
        for(int i = n - 1;i >= 0;i--){
            ll sum = 0;
            if(i < n - 1){
                for(int j = 0;j <= k && j < m;j++) sum = (sum + f[i + 1][j][0] + f[i + 1][j][1]) % MOD;
                for(int j = 0;j < m;j++){
                    if(j - k - 1 >= 0) sum = (sum - f[i + 1][j - k - 1][0] - f[i + 1][j - k - 1][1] + MOD + MOD) % MOD;
                    if(g[i][j] == '.') f[i][j][0] = sum;
                    if(j + k + 1 < m) sum = (sum + f[i + 1][j + k + 1][0] + f[i + 1][j + k + 1][1]) % MOD;
                }
            }
            sum = 0;
            for(int j = 1;j <= d && j < m;j++) sum = (sum + f[i][j][0]) % MOD;
            for(int j = 0;j < m;j++){
                if(j > 0) sum = (sum + f[i][j - 1][0]) % MOD;
                if(j - d - 1 >= 0) sum = (sum - f[i][j - d - 1][0] + MOD) % MOD;
                if(g[i][j] == '.') f[i][j][1] = sum;
                if(j + 1 < m) sum = (sum - f[i][j + 1][0] + MOD) % MOD;
                if(j + d + 1 < m) sum = (sum + f[i][j + d + 1][0]) % MOD;
            }
        }
        ll ans = 0;
        for(int j = 0;j < m;j++){
            for(int k = 0;k < 2;k++){
                ans = (ans + f[0][j][k]) % MOD;
            }
        }
        return ans;
    }
};
