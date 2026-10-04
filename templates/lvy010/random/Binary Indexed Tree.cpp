// Fenwick tree (binary indexed tree) template
class BIT {
    vector<int> tree;
public:
    BIT(int n) : tree(n) {}

    // add one to the number at index i
    void inc(int i) {
        while (i < tree.size()) {
            ++tree[i];
            i += i & -i;
        }
    }

    // return the sum of the closed interval [1, i]
    int sum(int x) {
        int res = 0;
        while (x > 0) {
            res += tree[x];
            x &= x - 1;
        }
        return res;
    }

    // return the sum of the closed interval [left, right]
    int query(int left, int right) {
        return sum(right) - sum(left - 1);
    }
};
