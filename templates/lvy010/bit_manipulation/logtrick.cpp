#include <bits/stdc++.h>
using namespace std;

// count the number of distinct GCDs over all subarrays (for OR/AND just replace gcd)
long long logTrick(vector<int>& a) {
    int n = a.size();
    long long res = 0;
    // store {current gcd value, info about subarrays ending here with this value}
    map<int, int> mp; 

    for (int x : a) {
        map<int, int> tmp;
        // 1. the current number alone as a subarray
        tmp[x]++;
        // 2. extend the previous subarrays
        for (auto& [g, cnt] : mp) {
            int new_g = gcd(g, x); 
            // OR: new_g = g | x;
            // AND: new_g = g & x;
            tmp[new_g] += cnt;
        }
        // 3. count distinct GCDs
        res += tmp.size();
        mp.swap(tmp);
    }
    return res;
}
