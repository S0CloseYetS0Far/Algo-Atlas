//Discretization + difference/segment tree for sweep line traversal implementation
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

typedef long long ll;
const int N = 200010;

// sweep line event: x coordinate, y interval [l,r], weight val (+1/-1)
struct Event {
    ll x, l, r, val;
    bool operator<(const Event& e) const { return x < e.x; }
} ev[N];

// compressed coordinate array + segment tree nodes (maintaining covered length and cover count)
ll ys[N];
struct Node {
    int l, r, cnt;
    ll len;
} tr[N << 2];

// segment tree pull-up
void pushup(int u) {
    if (tr[u].cnt) tr[u].len = ys[tr[u].r + 1] - ys[tr[u].l];
    else if (tr[u].l == tr[u].r) tr[u].len = 0;
    else tr[u].len = tr[u << 1].len + tr[u << 1 | 1].len;
}

// segment tree build
void build(int u, int l, int r) {
    tr[u] = {l, r, 0, 0};
    if (l == r) return;
    int mid = l + r >> 1;
    build(u << 1, l, mid), build(u << 1 | 1, mid + 1, r);
}

// segment tree range update
void update(int u, int l, int r, int val) {
    if (tr[u].l >= l && tr[u].r <= r) {
        tr[u].cnt += val;
        pushup(u);
        return;
    }
    int mid = l + r >> 1;
    if (l <= mid) update(u << 1, l, r, val);
    if (r > mid) update(u << 1 | 1, l, r, val);
    pushup(u);
}

int main() {
    ios::sync_with_stdio(false), cin.tie(0);
    int n, idx = 0, yidx = 0;
    cin >> n;
    for (int i = 0; i < n; i++) {
        ll x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        ev[idx++] = {x1, y1, y2, 1};  // entering edge
        ev[idx++] = {x2, y1, y2, -1}; // leaving edge
        ys[yidx++] = y1, ys[yidx++] = y2;
    }
    // compress y coordinates
    sort(ys, ys + yidx);
    yidx = unique(ys, ys + yidx) - ys;
    // sort events
    sort(ev, ev + idx);
    // build the tree (on compressed indices)
    build(1, 0, yidx - 2);
    ll res = 0;
    for (int i = 0; i < idx; i++) {
        if (i) res += tr[1].len * (ev[i].x - ev[i-1].x);
        // find the compressed index of y
        int l = lower_bound(ys, ys + yidx, ev[i].l) - ys;
        int r = lower_bound(ys, ys + yidx, ev[i].r) - ys - 1;
        update(1, l, r, ev[i].val);
    }
    cout << res << endl;
    return 0;
}
