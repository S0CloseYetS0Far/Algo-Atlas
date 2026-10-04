/*
1. Reusable: just replace the cost() function to adapt it to different partition-cost problems.
​
2. Time complexity: O(k·n log n), much more efficient than the naive DP's O(k·n²).
​
3. When to use: when the DP transition satisfies decision monotonicity (i.e. the optimal split point is non-decreasing as the interval moves right), you can use this template
*/
#include <vector>
#include <algorithm>
using namespace std;

const long long INF = 1e18;

// 1. define the cost function
// example: compute the cost of the interval [l, r)
long long cost(int l, int r, vector<long long>& presum) {
    long long s = presum[r] - presum[l];
    return s * (s + 1) / 2;
}

// 2. core divide-and-conquer optimization function
void solve(int l, int r, int optL, int optR, vector<vector<long long>>& dp, vector<long long>& presum, int k) {
    if (l > r) return;
    int mid = (l + r) / 2;
    long long best = INF;
    int bestOpt = optL;
    // search for the optimal split point in [optL, min(optR, mid-1)]
    for (int i = optL; i <= min(optR, mid - 1); ++i) {
        long long current = dp[k-1][i] + cost(i, mid, presum);
        if (current < best) {
            best = current;
            bestOpt = i;
        }
    }
    dp[k][mid] = best;
    // recurse on the left and right intervals
    solve(l, mid - 1, optL, bestOpt, dp, presum, k);
    solve(mid + 1, r, bestOpt, optR, dp, presum, k);
}

// 3. main DP function
vector<vector<long long>> divideAndConquerDP(int K, int n, vector<long long>& presum) {
    vector<vector<long long>> dp(K+1, vector<long long>(n+1, INF));
    // initialization: cost when partitioning into 1 segment
    for (int i = 1; i <= n; ++i) {
        dp[1][i] = cost(0, i, presum);
    }
    // solve with divide-and-conquer optimization
    for (int k = 2; k <= K; ++k) {
        solve(1, n, 1, n, dp, presum, k);
    }
    return dp;
}

// 4. problem entry function
long long solveProblem(vector<int>& nums, int k) {
    int n = nums.size();
    vector<long long> presum(n + 1, 0);
    for (int i = 0; i < n; ++i) {
        presum[i+1] = presum[i] + nums[i];
    }
    auto dp = divideAndConquerDP(k, n, presum);
    return dp[k][n];
}
