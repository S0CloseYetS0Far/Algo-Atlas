/*
3962
discretized Fenwick Tree storing element count and sum
iterates all subarrays to maintain inner
outer elements dynamically
swaps up to k extreme values between inside and outside
computes the maximum subarray sum after swaps.
*/
class FenwickTree {
    const int high_bit;
    const vector<int>& sorted;
    vector<int> cnt;
    vector<long long> sum;

public:
    FenwickTree(const vector<int>& sorted) : 
        cnt(sorted.size() + 1), 
        sum(sorted.size() + 1), 
        sorted(sorted), 
        high_bit(1 << (bit_width(sorted.size()) - 1)) {}

    // add num copies of val, where val's compressed value is i (i starts from 1)
    // if num < 0, remove -num copies of val
    void update(int i, int num, int val) {
        for (; i < cnt.size(); i += i & -i) {
            cnt[i] += num;
            sum[i] += val;
        }
    }

    // return the k-th smallest number (k starts from 1)
    int kth(int k) const {
        int i = 0;
        for (int b = high_bit; b > 0; b >>= 1) {
            int nxt = i | b;
            if (nxt < cnt.size() && cnt[nxt] < k) {
                k -= cnt[nxt];
                i = nxt;
            }
        }
        return sorted[i];
    }

    // return the sum of the k smallest numbers (k starts from 1)
    long long pre_sum(int k) const {
        long long s = 0;
        int i = 0;
        for (int b = high_bit; b > 0; b >>= 1) {
            int nxt = i | b;
            if (nxt < cnt.size() && cnt[nxt] < k) {
                k -= cnt[nxt];
                s += sum[nxt];
                i = nxt;
            }
        }
        // add the numbers equal to the k-th smallest
        return s + 1LL * sorted[i] * k;;
    }
};

class Solution {
public:
    long long maxSum(vector<int>& nums, int k) {
        // coordinate compression
        int n = nums.size();
        vector<int> sorted = nums;
        ranges::sort(sorted);
        sorted.erase(ranges::unique(sorted).begin(), sorted.end());
        vector<int> rank(n); // rank[i] is the compressed value of nums[i] (starting from 1)
        FenwickTree all_tree(sorted); // Fenwick tree containing all elements
        long long total = 0;
        for (int i = 0; i < n; i++) {
            int x = nums[i];
            rank[i] = ranges::lower_bound(sorted, x) - sorted.begin() + 1;
            all_tree.update(rank[i], 1, x);
            total += x;
        }

        long long ans = LLONG_MIN;

        // enumerate the left endpoint of the subarray
        for (int left = 0; left < n; left++) {
            FenwickTree in_tree(sorted);
            FenwickTree out_tree = all_tree;
            int need_swap = 0;
            long long sub_sum = 0;

            // enumerate the right endpoint of the subarray
            for (int right = left; right < n; right++) {
                // x moves from outside the subarray to inside it
                int x = nums[right];
                int rk = rank[right];
                sub_sum += x;
                in_tree.update(rk, 1, x);
                out_tree.update(rk, -1, -x);

                bool inc = false;
                int sz = right - left + 1;
                if (need_swap < k && need_swap < sz && need_swap < n - sz) {
                    // can we do one more swap
                    if (in_tree.kth(need_swap + 1) < out_tree.kth(n - sz - need_swap)) {
                        inc = true;
                        need_swap++;
                    }
                }

                if (!inc && need_swap > 0) {
                    // whether we need to reduce the number of swaps
                    if (in_tree.kth(need_swap) >= out_tree.kth(n - sz - need_swap + 1)) {
                        need_swap--;
                    }
                }

                // compute the increase in the element sum gained through swaps
                long long delta = 0;
                if (need_swap > 0) {
                    long long in_sum = in_tree.pre_sum(need_swap);
                    long long out_sum = total - sub_sum - out_tree.pre_sum(n - sz - need_swap);
                    delta = out_sum - in_sum;
                }

                ans = max(ans, sub_sum + delta);
            }
        }

        return ans;
    }
};
