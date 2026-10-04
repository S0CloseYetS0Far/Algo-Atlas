class Solution { //lc3650
public:
    int minCost(int n, vector<vector<int>>& edges) {
        vector<vector<pair<int, int>>> g(n); // adjacency list
        for (auto& e : edges) {
            int x = e[0], y = e[1], wt = e[2];
            g[x].emplace_back(y, wt);
            g[y].emplace_back(x, wt * 2);
        }

        vector<int> dis(n, INT_MAX);
        // min heap(shortest path length from the start to node x, node x)
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;       
        dis[0] = 0; // the distance from the start to itself is 0
        pq.emplace(0, 0);

        while (!pq.empty()) {
            auto [dis_x, x] = pq.top();
            pq.pop();
            if (dis_x > dis[x]) 
            // x has already been popped from the heap
                continue;            
            if (x == n - 1)  // reached the destination
                return dis_x;
            
            for (auto& [y, wt] : g[x]) {
               auto new_dis_y = dis_x + wt;
                if (new_dis_y < dis[y]) {
                    dis[y] = new_dis_y; 
                    // update the shortest paths of x's neighbors
                    // lazy heap update: only insert, never update entries already in the heap
                    // the same node may have several different new_dis_y; all but the smallest new_dis_y will trigger the continue above
                  pq.emplace(new_dis_y, y);
                }
            }
        }
        return -1;
    }
};
