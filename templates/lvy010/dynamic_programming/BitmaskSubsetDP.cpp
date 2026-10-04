class Solution {
public:
    long long minMergeCost(vector<vector<int>>& lists) {
        int n = lists.size();
        int g[1 << n], h[1 << n];
        for (int i = 1; i < (1 << n); i++) {
            g[i] = 0;
            // precompute the merged lengths
            for (int j = 0; j < n; j++) if (i >> j & 1) g[i] += lists[j].size();

            // binary search for the median of multiple sorted lists
            int head = -1e9, tail = 1e9;
            while (head < tail) {
                int mid = (head + tail) >> 1;
                int cnt = 0;
                // inside, binary search again to count how many numbers <= mid are in each list
                for (int j = 0; j < n; j++) if (i >> j & 1) cnt += upper_bound(lists[j].begin(), lists[j].end(), mid) - lists[j].begin();
                if (cnt >= (g[i] + 1) / 2) tail = mid;
                else head = mid + 1;
            }
            h[i] = head;
        }

        const long long INF = 1e18;
        long long f[1 << n];
        // enumerate the binary mask
        for (int i = 1; i < (1 << n); i++) {
            if (__builtin_popcount(i) == 1) { f[i] = 0; continue; }
            f[i] = INF;
            // enumerate subsets
            for (int j = i; j; j = (j - 1) & i) if (j != i) {
                int k = i ^ j;
                long long c = f[j] + f[k] + g[j] + g[k] + abs(h[j] - h[k]);
                f[i] = min(f[i], c);
            }
        }
        return f[(1 << n) - 1];
    }
};
