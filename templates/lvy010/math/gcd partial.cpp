//3312


class Solution {
public:
    vector<int> gcdValues(vector<int>& nums, vector<long long>& queries) {
        int mx = ranges::max(nums);
        vector<int> cnt_x(mx + 1);
        for (int x : nums) {
            cnt_x[x]++;
        }

        vector<long long> cnt_gcd(mx + 1);
        for (int i = mx; i > 0; i--) {
            int c = 0;
            for (int j = i; j <= mx; j += i) {
                c += cnt_x[j];
                cnt_gcd[i] -= cnt_gcd[j]; // pairs whose gcd is 2i, 3i, 4i, ... must not be counted
            }
            cnt_gcd[i] += (long long) c * (c - 1) / 2; // choose 2 of the c numbers, giving c*(c-1)/2 pairs
        }

        // compute prefix sums in place
        partial_sum(cnt_gcd.begin(), cnt_gcd.end(), cnt_gcd.begin());

        vector<int> ans(queries.size());
        for (int i = 0; i < queries.size(); i++) {
            ans[i] = ranges::upper_bound(cnt_gcd, queries[i]) - cnt_gcd.begin();
        }
        return ans;
    }
};
