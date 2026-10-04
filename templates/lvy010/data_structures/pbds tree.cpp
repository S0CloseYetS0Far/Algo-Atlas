//lc3737

#include <ext/pb_ds/assoc_container.hpp>

using namespace __gnu_pbds;
// use pair<key, index> to support duplicate keys
using ordered_set = tree<pair<int, int>, null_type, less<>, rb_tree_tag, tree_order_statistics_node_update>;

class Solution {
public:
    long long countMajoritySubarrays(vector<int>& nums, int target) {
        int idx = 0; // incremented on insert to keep the elements in st distinct
        ordered_set* st = new ordered_set();
        st->insert({0, ++idx});
        long long ans = 0;
        int s = 0;
        for (int x : nums) {
            s += x == target ? 1 : -1;
            // order_of_key(key) counts the elements in st strictly less than key
            ans += st->order_of_key({s, 0});
            st->insert({s, ++idx});
        }
        delete st;
        return ans;
    }
};
