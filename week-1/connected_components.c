#include <stdio.h>
#include <stdlib.h>

int number_of_components(int **adj, int n);
void dfs(int u, int **adj, int n, int *visited);

int main(void)
{
    // read vertice number and number of edges
    int n, m;
    scanf("%d %d", &n, &m);

    // initialize adjacent matrix
    int **adj = malloc(n * sizeof(int *));
    for (int i = 0; i < n; ++i) {
        adj[i] = malloc(n * sizeof(int));
        for (int j = 0; j < n; ++j) {
            adj[i][j] = 0;
        }
    }

    // fill adjacent matrix
    for (int i = 0; i < m; ++i) {
        int u, v;
        scanf("%d %d", &u, &v);
        adj[u - 1][v - 1] = 1;
        adj[v - 1][u - 1] = 1;
    }

    // print the result
    printf("%d\n", number_of_components(adj, n));

    // free the allocated memory
    for (int i = 0; i < n; ++i) {
        free(adj[i]);
    }
    free(adj);
    return 0;
}

int number_of_components(int **adj, int n){
    int result = 0;

    // allocate an array for keeping track of visited nodes
    // by initializing with 0
    int *visited = malloc(n * sizeof(int));
    for(int i = 0; i < n; ++i){
        visited[i] = 0;
    }

    // loop over all the nodes 
    for(int i = 0; i < n; ++i){
        if(!visited[i]){             // if the node hasn't been visited yet
            result++;                // incement result by one.
            dfs(i, adj, n, visited); // visit the connected 
        }
    }
    free(visited);
    return result;
}

/**
 * Recursively visit the connected nodes by using Depth First Search (DFS).
 * Starting from the given node, the function visits all reachable nodes
 * by following the edges of the graph. During the traversal, the function
 * marks the visited visited nodes to avoid repetition.
 *
 * @param u: the current node.
 * @param adj: the adjacency matrix of the graph.
 * @param n: the number of nodes.
 * @param visited: array showing visited nodes.
 */
void dfs(int u, int **adj, int n, int *visited){
    visited[u] = 1;
    for(int v = 0; v < n; ++v){
        if(adj[u][v] && !visited[v]){
            dfs(v, adj, n, visited);
        }
    }
}

