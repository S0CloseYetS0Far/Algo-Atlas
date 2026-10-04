#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

const int MAXN = 1e5 + 5;

int a[MAXN];          // original array
long long sum[MAXN];  // sum of each block
int add[MAXN];        // lazy tag of each block (increment)
int block_size;       // block size
int n;

// initialize the blocks
void init() {
    block_size = sqrt(n);
    for (int i = 1; i <= n; i++) {
        int bid = i / block_size;
        sum[bid] += a[i];
    }
}

// range add: [l, r] += val
void update(int l, int r, int val) {
    int bl = l / block_size;
    int br = r / block_size;
    if (bl == br) {
        // same block: brute-force update
        for (int i = l; i <= r; i++) {
            a[i] += val;
            sum[bl] += val;
        }
    } else {
        // partial block on the left
        for (int i = l; i < (bl + 1) * block_size; i++) {
            a[i] += val;
            sum[bl] += val;
        }
        // full blocks in the middle
        for (int i = bl + 1; i < br; i++) {
            add[i] += val;
        }
        // partial block on the right
        for (int i = br * block_size; i <= r; i++) {
            a[i] += val;
            sum[br] += val;
        }
    }
}

// range query: sum of [l, r]
long long query(int l, int r) {
    long long res = 0;
    int bl = l / block_size;
    int br = r / block_size;
    if (bl == br) {
        for (int i = l; i <= r; i++) {
            res += a[i] + add[bl];
        }
    } else {
        for (int i = l; i < (bl + 1) * block_size; i++) {
            res += a[i] + add[bl];
        }
        for (int i = bl + 1; i < br; i++) {
            res += sum[i] + (long long)add[i] * block_size;
        }
        for (int i = br * block_size; i <= r; i++) {
            res += a[i] + add[br];
        }
    }
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    // read n and array a
    // init();
    // handle update / query
    return 0;
}
