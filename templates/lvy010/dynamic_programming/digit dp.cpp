/*
Fast exponent modulo plus digit DP counting weight of binary 1 bits
compute modular product via total exponent difference in range.
*/

class Solution {
    int pow(long long x, long long n, long long mod) {
        long long res = 1 % mod; // note: mod may equal 1
        for (; n; n /= 2) {
            if (n % 2) {
                res = res * x % mod;
            }
            x = x * x % mod;
        }
        return res;
    }

    long long sum_e(long long k) {
        long long res = 0, n = 0, cnt1 = 0, sum_i = 0;
        for (long long i = __lg(k + 1); i >= 0; i--) {
            long long c = (cnt1 << i) + (i << i >> 1); // number of newly added powers
            if (c <= k) {
                k -= c;
                res += (sum_i << i) + ((i * (i - 1) / 2) << i >> 1);
                sum_i += i; // sum of the exponents of the 1s filled so far
                cnt1++; // number of 1s filled so far
                n |= 1LL << i; // fill in a 1
            }
        }
        // the remaining k powers are supplied by the lowest k 1-bits of n
        while (k--) {
            res += __builtin_ctzll(n);
            n &= n - 1; // remove the lowest 1-bit (set it to 0)
        }
        return res;
    }

public:
    vector<int> findProductsOfElements(vector<vector<long long>>& queries) {
        vector<int> ans;
        for (auto& q : queries) {
            auto er = sum_e(q[1] + 1);
            auto el = sum_e(q[0]);
            ans.push_back(pow(2, er - el, q[2]));
        }
        return ans;
    }
};
