# DirectedGraphLinearisation

This repo attempts to create a reasonably performing 'linearisation of a directed graph' such that it minimises the cost that is defined as $c = \sum_{i} \mathrm{dist}(e_i),$ where $\mathrm{dist}(e_i)$ is the distance (i.e. number of hops) between the two vertices of the edge $e_i$.

Additionally:
- Analyse time and space complexity, i.e. big-O of the number of vertices and edges.

Jargon used:
- If two vertices have bi-directional connection (i.e. an edge in each direction), I'll call them doubly-connected or double, else I'll call them singly-connected or single.

Observations:
- Representing graph as adjacency list has better space complexity.
- The cost function factors into independent parts for disconnected graphs.
- I will tailor the linearisation to graphs that have regions of doubly-connected vertices that are further interconnected by single connections (single edges within doubly-connected regions are ignored).

Steps:
1. Find connected components of the given graph.
    1. Transform the graph into undirected graph; complexity:
        - time: $O(n_{e})$,
        - space: $O(n_{v} + n_{e})$.
    2. Use breath-first search to find connected components (CC); complexity
        - time: $O(n_{v} + n_{e})$,
        - space: $O(n_{v})$.
2. Process each CC separately.
    1. Split the CC into doubly-connected components (DCCs). (Complexity as above for time and $O(n_{v}^2)$ for space.)
    2. Order the DCCs. (Complexity $O(n_{v}^2)$ for both time and space.)
        1. Choose the DCC with most connections to the other DCCs and place it into a list.
        2. Repeat until all DCCs are in the list
            1. For the front and back elements of list, pick an unplaced DCC (uDCC) that has the most connections to it.
            2. Compare the number of connections of uDCC.
            3. Place the uDCC (to the front/back) with more connections.
            4. If front or back element does not have an uDCC, use the other.
            5. If neither has connections, check the element next to it until a uDCC is found.
    3. Order the vertices inside each DCCs. (Complexity $O(n_{v} + n_{e})$ for time and $O(n_{v})$ for space.)
        1. Choose the vertex with most connections within DCC as a seed for breath-first search.
        2. Initiate a branch for each connection of the seed and store visited vertices in the branches until all visited.
        3. For each branch create a 'side number', i.e. add +1 for each outside connection to the right and -1 for connections to the left.
        4. Balance and split the branches based on their 'side number' and number of elements.
        5. Zip insert the branches into the final list.
    4. Concatenate the partial lists from step 2.3. according to the order from the step 2.2. (Complexity $O(n_{v})$ for both time and space.)
3. Arbitrarily concatenates the ordered lists from the step 2. (Complexity $O(n_{v})$ for both time and space.)

# Building

To build the repo one can use the provided `Makefile`. It offers the following targets:
- `build` (builds the project) with `COMPILER` and `BUILD_TYPE` options;
- `test` (runs tests, one must first build) with `COMPILER` and `BUILD_TYPE` options;
- `clean` (removes the build, except external downloads) with `COMPILER` and `BUILD_TYPE` options;
- `cclean` (removes the build, with external downloads) with `COMPILER` and `BUILD_TYPE` options;
- `coverage` (builds coverage) with `COMPILER` option, placed in `./build/COMPILER/Coverage/coverage/html`;
- `docs` (builds documentation), placed in `./build/docs/html`;
- `tidy` (runs clang tidy);
- `format` (runs clang format);
- `all` (runs all combinations);
- `cleanall` (cleans `./build` folder).

The variables, `COMPILER` and `BUILD_TYPE`, can be set to 'gcc' or 'clang' and 'Debug' or 'Release', respectively.
All binaries are places in `./build/COMPILER/BUILD_TYPE/bin`.
The `solution` is in the example folder.

Makefile is adapted for Intel MacBook with HomeBrew; for a different system please change [this](https://github.com/JurajX/DirectedGraphLinearisation/blob/master/Makefile#L44) part of the makefile.
