# DirectedGraphLinearisation

This repo attempts to create a reasonably preforming 'linearisation of a directed graph' such that it minimises the cost that is defined as $c = \sum_{i} \mathrm{dist}(e_i)\,,$ where $\mathrm{dist}(e_i)$ is the distance (i.e. number of hops) between the two vertices of the edge $e_i$.

Jargon:
- If two vertices have bi-directional connection (i.e. an edge in each direction), I'll call them doubly-connected, else I'll call them singly-connected.

Additionals:
- Minimise time and space requirements of the solution; complexity:
    - average: $O(n_{v} + n_{e})$,
    - worst: $O(n_{v}^2)$.
- Analyse time and space complexity, i.e. big-O of the number of vertices and edges.

Observations:
- Representing graph as adjacency list has better space complexity.
- The cost function factors into independent parts for disconnected graphs.
- I will tailor the linearisation for connected graphs that have groups of doubly-connected vertices connected components

Steps:
1. Find connected components of the given graph.
    1. Transform the graph into undirected graph; complexity:
        - time: $O(n_{e})$,
        - space: $O(n_{v} + n_{e})$.
    2. Use breath-first search to find connected components (CC); complexity
        - time: $O(n_{v} + n_{e})$,
        - space: $O(n_{v})$.
2. Process each CC separately.
    1. Split the CC into doubly-connected components (DCCs).
    2. Order the DCCs.
        1. Choose the DCC with most connections to the other DCCs and place it into a list.
        2. Repeat untill all DCCs are in the list
            1. For the front and back elements of list, pick an unplaced DCC (uDCC) that has the most connections to it.
            2. Compare the number of connections of uDCC.
            3. Place the uDCC (to the front/back) with more connections.
            4. If front or back element does not have an uDCC, use the other.
            5. If neither has connections, check the element next to it untill a uDCC is found.
    3. Order the vertices inside each DCCs.
        1. Choose the vertex with most connections within DCC as a seed for breath-first search.
        2. Initiate a branch for each connection of the seed and store visited vertices in the branches until all visited.
        3. For each branch create a 'side number', i.e. add +1 for each outside connection to the right and -1 for connections to the left.
        4. Balance and split the branches based on their 'side number' and number of elements.
        5. Zip insert the branches into the final list.
    4. Concatinate the partial lists from step 2.3. according to the order from the step 2.2.
3. Arbitrarily concantinates the ordered lists from the step 2.
