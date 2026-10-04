#lc2059 minimum operations to convert number
class Solution:
#When I first got this problem I couldn't find any pattern. It asks for the minimum number of operations, and the data range is tiny: unweighted shortest path... brute force... hmm~ BFS?!

#I used to love using DFS for problems, because recursive code is usually shorter.
# But the biggest difference between DFS and BFS in path finding is that DFS follows one path all the way to the end, so it suits "does a path exist?"
# whereas BFS is like shadow clones: each clone advances one step from where it was, so it suits "what is the shortest path?".

#Once you understand the difference above, you can solve it smoothly.

    def minimumOperations(self, nums: List[int], start: int, goal: int) -> int:
        if start < 0 or start > 1000 or start == goal:
            return start == goal
        q = deque([start])
        vis = set([start])
        step = 0

        while q:
            size = len(q)
            while size:
                cur = q.popleft()
                for x in nums:
                    compute = (cur + x, cur - x, cur ^ x)
                    for y in compute:
                        if y == goal:
                            return step + 1
                        if 0 <= y <= 1000 and y not in vis:
                            q.append(y)
                            vis.add(y)
                size -= 1
            step += 1
        
        return -1