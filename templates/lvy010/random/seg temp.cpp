// template source: https://leetcode.cn/circle/discuss/mOr1u6/
template<typename T, typename F>
class LazySegmentTree {
    // note: you can also remove template<typename T, typename F> and define T and F here instead
    // using T = pair<int, int>;
    // using F = pair<int, int>;

    // initial value of the lazy tag
    const F TODO_INIT = 0; // **modify per problem**

    struct Node {
        T val;
        F todo;
    };

    int n;
    vector<Node> tree;

    // merge two vals
    T merge_val(const T& a, const T& b) const {
        return a + b; // **modify per problem**
    }

    // merge two lazy tags
    F merge_todo(const F& a, const F& b) const {
        return a + b; // **modify per problem**
    }

    // apply the lazy tag to node's subtree (range add in this example)
    void apply(int node, int l, int r, F todo) {
        Node& cur = tree[node];
        // compute the overall change of tree[node]'s interval
        cur.val += todo * (r - l + 1); // **modify per problem**
        cur.todo = merge_todo(todo, cur.todo);
    }

    // push the current node's lazy tag down to its left and right children
    void spread(int node, int l, int r) {
        Node& cur = tree[node];
        F todo = cur.todo;
        if (todo == TODO_INIT) { // nothing to push down
            return;
        }
        int m = (l + r) / 2;
        apply(node * 2, l, m, todo);
        apply(node * 2 + 1, m + 1, r, todo);
        cur.todo = TODO_INIT; // push-down done
    }

    // merge the left and right children's val into the current node's val
    void maintain(int node) {
        tree[node].val = merge_val(tree[node * 2].val, tree[node * 2 + 1].val);
    }

    // initialize the segment tree from a
    // time complexity O(n)
    void build(const vector<T>& a, int node, int l, int r) {
        Node& cur = tree[node];
        cur.todo = TODO_INIT;
        if (l == r) { // leaf
            cur.val = a[l]; // initialize the leaf value
            return;
        }
        int m = (l + r) / 2;
        build(a, node * 2, l, m); // initialize the left subtree
        build(a, node * 2 + 1, m + 1, r); // initialize the right subtree
        maintain(node);
    }

    void update(int node, int l, int r, int ql, int qr, F f) {
        if (ql <= l && r <= qr) { // current subtree is entirely within [ql, qr]
            apply(node, l, r, f);
            return;
        }
        spread(node, l, r);
        int m = (l + r) / 2;
        if (ql <= m) { // update the left subtree
            update(node * 2, l, m, ql, qr, f);
        }
        if (qr > m) { // update the right subtree
            update(node * 2 + 1, m + 1, r, ql, qr, f);
        }
        maintain(node);
    }

    T query(int node, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) { // current subtree is entirely within [ql, qr]
            return tree[node].val;
        }
        spread(node, l, r);
        int m = (l + r) / 2;
        if (qr <= m) { // [ql, qr] is in the left subtree
            return query(node * 2, l, m, ql, qr);
        }
        if (ql > m) { // [ql, qr] is in the right subtree
            return query(node * 2 + 1, m + 1, r, ql, qr);
        }
        T l_res = query(node * 2, l, m, ql, qr);
        T r_res = query(node * 2 + 1, m + 1, r, ql, qr);
        return merge_val(l_res, r_res);
    }

public:
    // the segment tree maintains an array of length n (indices 0 to n-1) with initial value init_val
    LazySegmentTree(int n, T init_val = 0) : LazySegmentTree(vector<T>(n, init_val)) {}

    // the segment tree maintains array a
    LazySegmentTree(const vector<T>& a) : n(a.size()), tree(2 << bit_width(a.size() - 1)) {
        build(a, 1, 0, n - 1);
    }

    // update every a[i] in [ql, qr] with f
    // 0 <= ql <= qr <= n-1
    // time complexity O(log n)
    void update(int ql, int qr, F f) {
        update(1, 0, n - 1, ql, qr, f);
    }

    // return the result of merging all a[i] with merge_val, for i in the closed interval [ql, qr]
    // 0 <= ql <= qr <= n-1
    // time complexity O(log n)
    T query(int ql, int qr) {
        return query(1, 0, n - 1, ql, qr);
    }
};

int main() {
    LazySegmentTree<long long, long long> t(8); // default value is 0
    t.update(3, 5, 100);
    t.update(4, 6, 10);
    cout << t.query(0, 7) << endl;

    vector<long long> nums = {3, 1, 4, 1, 5, 9, 2, 6};
    LazySegmentTree<long long, long long> t2(nums);
    t2.update(3, 5, 1);
    t2.update(4, 6, 1);
    cout << t2.query(0, 7) << endl;
    return 0;
}
