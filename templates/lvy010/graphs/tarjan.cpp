#include <vector>
#include <algorithm>
using namespace std;
//Strongly connected components + condensation
//Tarjan finds SCCs → each SCC is contracted into a single node → build a new directed acyclic graph (DAG)
//You can then directly do topological sort, count in/out degrees, etc.
const int MAXN = 2e5 + 10; // adjust per problem

vector<int> g[MAXN];
int dfn[MAXN], low[MAXN], idx;
int stk[MAXN], in_stk[MAXN], top;
int scc[MAXN], scc_cnt; // scc[i] = id of the SCC that node i belongs to

void tarjan(int u) {
    dfn[u] = low[u] = ++idx;
    stk[++top] = u;
    in_stk[u] = 1;

    for (int v : g[u]) {
        if (!dfn[v]) {
            tarjan(v);
            low[u] = min(low[u], low[v]);
        } else if (in_stk[v]) {
            low[u] = min(low[u], dfn[v]);
        }
    }

    if (dfn[u] == low[u]) {
        scc_cnt++;
        int v;
        do {
            v = stk[top--];
            in_stk[v] = 0;
            scc[v] = scc_cnt;
        } while (v != u);
    }
}

// condensation: build the new graph
vector<int> ng[MAXN]; // new graph after condensation
int in_deg[MAXN], out_deg[MAXN];

void build_new_graph(int n) {
    for (int u = 1; u <= n; u++) {
        for (int v : g[u]) {
            if (scc[u] != scc[v]) {
                ng[scc[u]].push_back(scc[v]);
            }
        }
    }

    // deduplicate (optional, avoids duplicate edges)
    for (int i = 1; i <= scc_cnt; i++) {
        sort(ng[i].begin(), ng[i].end());
        ng[i].erase(unique(ng[i].begin(), ng[i].end()), ng[i].end());
        out_deg[i] = ng[i].size();
        for (int v : ng[i]) in_deg[v]++;
    }
}

// usage
void init(int n) {
    idx = top = scc_cnt = 0;
    for (int i = 1; i <= n; i++) {
        g[i].clear();
        ng[i].clear();
        dfn[i] = low[i] = scc[i] = in_stk[i] = in_deg[i] = out_deg[i] = 0;
    }
}

int main() {
    int n, m;
    // read input and build the graph...
    init(n);
    for (int i = 1; i <= n; i++) {
        if (!dfn[i]) tarjan(i);
    }
    build_new_graph(n);
    return 0;
}
