
//sparse table

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
using namespace std;

const int MAXN = 1e5 + 5;
const int LOG = 20;

int st[LOG][MAXN];
int a[MAXN];
int n;

void build() {
    for (int i = 1; i <= n; i++) st[0][i] = a[i];
    for (int k = 1; k < LOG; k++)
        for (int i = 1; i + (1 << k) - 1 <= n; i++)
            st[k][i] = max(st[k-1][i], st[k-1][i + (1 << (k-1))]);
}

int query(int l, int r) {
    int len = r - l + 1;
    int k = log2(len);
    return max(st[k][l], st[k][r - (1 << k) + 1]);
}

int main() {
    int m;
    cin >> n >> m;
    for (int i = 1; i <= n; i++) cin >> a[i];
    build();
    while (m--) {
        int l, r;
        cin >> l >> r;
        cout << query(l, r) << endl;
    }
    return 0;
}
