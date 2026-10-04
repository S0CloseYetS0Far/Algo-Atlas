template <typename T>
class UnionFind {
public:
    vector<int> fa;
    vector<T> dis;  // distance from x to the representative of its set

    UnionFind(int n) : fa(n), dis(n) {
        for (int i = 0; i <= n - 1; i++) fa[i] = i;
    }

    int get(int x) {
        if (fa[x] != x) {
            int root = get(fa[x]);
            dis[x] += dis[fa[x]];  // recursively update the distance from x to its representative
            fa[x] = root;
        }
        return fa[x];
    }

    bool same(int x, int y) { return get(x) == get(y); }

    // relative distance from `from` to `to`; they must be in the same set
    T get_relative_distance(int from, int to) {
        get(from), get(to);
        return dis[from] - dis[to];
    }

    // merge `from` and `to`, adding the constraint to - from = value
    // if `to` and `from` are in different sets, return true; otherwise return whether the constraint is consistent with existing information
    bool merge(int from, int to, T value) {
        int x = get(from), y = get(to);
        if (x == y) return dis[from] - dis[to] == value;
        dis[x] = value + dis[to] - dis[from]; // update the distance between representatives
        fa[x] = y;
        return true;
    }
};