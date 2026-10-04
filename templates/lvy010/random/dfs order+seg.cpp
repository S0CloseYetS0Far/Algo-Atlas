//Classic problem on DFS order of a tree: 3515
class Solution {
public:
    vector<int> treeQueries(int n, vector<vector<int>>& edges, vector<vector<int>>& queries) {

        ////////////////segment tree begins
        //for this problem only range update and point query are implemented
        //since it's range update, lazy tags are required
        //since it's point query, there's no need to store range sums etc. (note that range sums would exceed the int limit)
        //tree[i] is the lazy tag of the interval represented by i, i.e. the value added to the interval
        vector<int> tree(n * 4);

        //push down tags
        auto pushdown = [&](int p){
            if (!tree[p]) return;
            tree[p * 2] += tree[p];
            tree[p * 2 + 1] += tree[p];
            tree[p] = 0;
        };
        //point query implementation
        auto query = [&](auto&& self, int p, int s, int t, int i)->int{
            if(s == t) return tree[p];
            int mid = (s + t) / 2;
            pushdown(p);
            if(i <= mid) return self(self, p * 2, s, mid, i);
            else return self(self, p * 2 + 1, mid + 1, t, i);
        };

        //range update implementation
        auto update = [&](auto&& self, int p, int s, int t, int l, int r, int v)->void{
            if(s >= l && t <= r){
                tree[p] += v;
                return;
            }
            int mid = (s + t) / 2;
            pushdown(p);
            if(l <= mid) self(self, p * 2, s, mid, l, r, v);
            if(r > mid) self(self, p * 2 + 1, mid + 1, t, l, r, v);
        };
        //public API: point query
        auto Query = [&](int i)->int{
            return query(query, 1, 0, n - 1, i);
        };
        //public API: range add
        auto Update = [&](int l, int r, int v)->void{
            update(update, 1, 0, n - 1, l, r, v);
        };
        ////////////////segment tree ends

        //build the graph as a weighted adjacency list; using map is more convenient
        //the constant factor is close to unordered_map's, with no risk of anti-hash tests
        //the root in this problem is 1; convert to 0-based
        vector<map<int, int>> g(n);
        for(auto& e : edges){
            e[0]--; e[1]--;
            g[e[0]][e[1]] = e[2];
            g[e[1]][e[0]] = e[2];
        }

        //standard DFS order
        //node x and its subtree occupy the closed interval [in[x], out[x] - 1]
        vector<int> in(n); 
        //time when entering node x
        vector<int> out(n); 
        //time when leaving node x
        int time = 0;
        auto dfs = [&](auto&& self, int u, int p, int d)->void{
            in[u] = time++;
            Update(in[u], in[u], d);
            for(auto [v, w] : g[u]){
                if(v == p) continue;
                self(self, v, u, d + w);
            }
            out[u] = time;
        };
        dfs(dfs, 0, -1, 0);

        vector<int> ans;
        for(auto& q : queries){
            if(q[0] == 1){
                int u = q[1] - 1, v = q[2] - 1;
                if(in[u] > in[v]) swap(u, v);
                int old = Query(in[v]) - Query(in[u]); //dynamic weight of edge u->v
                //you can also query and maintain it as follows
                /*
                int old = g[u][v];
                g[u][v] = g[v][u] = q[3];
                */
                Update(in[v], out[v] - 1, q[3] - old);
            }
            else{
                ans.push_back(Query(in[q[1] - 1]));
            }
        }
        return ans;
    }
};
