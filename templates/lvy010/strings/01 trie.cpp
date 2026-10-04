//2935 +sliding win
class Node {
public:
    array<Node*, 2> children{};
    int cnt = 0; // subtree size
};

class Trie {
    static const int HIGH_BIT = 19;
public:
    Node *root = new Node();

    // add val
    void insert(int val) {
        Node *cur = root;
        for (int i = HIGH_BIT; i >= 0; i--) {
            int bit = (val >> i) & 1;
            if (cur->children[bit] == nullptr) {
                cur->children[bit] = new Node();
            }
            cur = cur->children[bit];
            cur->cnt++; // maintain the subtree size
        }
    }

    // delete val, but don't delete nodes
    // val must be in the trie
    void remove(int val) {
        Node *cur = root;
        for (int i = HIGH_BIT; i >= 0; i--) {
            cur = cur->children[(val >> i) & 1];
            cur->cnt--; // maintain the subtree size
        }
    }

    // return the maximum XOR of val with an element in the trie
    // the trie must not be empty
    int max_xor(int val) {
        Node *cur = root;
        int ans = 0;
        for (int i = HIGH_BIT; i >= 0; i--) {
            int bit = (val >> i) & 1;
            // if cur.children[bit^1].cnt == 0, treat it as an empty node
            if (cur->children[bit ^ 1] && cur->children[bit ^ 1]->cnt) {
                ans |= 1 << i;
                bit ^= 1;
            }
            cur = cur->children[bit];
        }
        return ans;
    }
};

class Solution {
public:
    int maximumStrongPairXor(vector<int> &nums) {
        sort(nums.begin(), nums.end());
        Trie t{};
        int ans = 0, left = 0;
        for (int y: nums) {
            t.insert(y);
            while (nums[left] * 2 < y) {
                t.remove(nums[left++]);
            }
            ans = max(ans, t.max_xor(y));
        }
        return ans;
    }
};
