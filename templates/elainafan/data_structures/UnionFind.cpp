class UnionFind {
    vector<int> fa;
    vector<int> sz;  // set sizes
public:
    int cc;  // number of connected components
    UnionFind(int n) : fa(n), sz(n, 1), cc(n) { ranges::iota(fa, 0); }
    int get(int x) {
        if (fa[x] != x) fa[x] = get(fa[x]);
        return fa[x];
    }
    bool is_same(int x, int y) { return get(x) == get(y); }
    bool merge(int from, int to) {
        int x = get(from), y = get(to);
        if (x == y) return false;
        fa[x] = y;
        sz[y] += sz[x];
        cc--;
        return true;
    }
    int get_size(int x) {  // size of the set containing x
        return sz[get(x)];
    }
};