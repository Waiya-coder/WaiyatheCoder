# Author: Brenda Waiya
# Date:March 6th, 2023
# Purpose: Algorithms



from collections import deque

def input_taker(start,goal):
    Frontier = deque()
    Backpointer = {}
    Frontier.append(start)
    Backpointer[start] = None

    while (goal not in Backpointer) and (len(Frontier) != 0):
        curr_v = Frontier.popleft()
        for adj_v in curr_v.adjacency_list:
            if adj_v not in Backpointer:
                Frontier.append(adj_v)
                Backpointer[adj_v] = curr_v

    v = goal
    path = []
    while v != None:
        path.append(v)
        bp = Backpointer[v]
        v = bp

    return path
