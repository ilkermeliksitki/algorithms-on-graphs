#include <stdio.h>
#include <stdlib.h>

int reach(int **adj, int n, int x, int y);

int main() {
    // read input values
    int n, m;
    scanf("%d %d", &n, &m);
    
    // read edge data
    int *data = malloc((2 * m + 2) * sizeof(int));
    for (int i = 0; i < 2 * m + 2; i++) {
        scanf("%d", &data[i]);
    }
    
    // read start and end vertices
    int x = data[2 * m];
    int y = data[2 * m + 1];

    // adjust start and end vertices to 0-based indexing
    x -= 1;
    y -= 1;
    
    // create adjacency matrix
    int **adj = malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        adj[i] = malloc(n * sizeof(int));
        for (int j = 0; j < n; j++) {
            adj[i][j] = 0;
        }
    }
    
    // fill adjacency matrix with edge data
    for (int i = 0; i < 2 * m; i += 2) {
        int a = data[i] - 1;
        int b = data[i + 1] - 1;
        adj[a][b] = 1; adj[b][a] = 1;
    }
    
    // call reach function and print result
    printf("%d\n", reach(adj, n, x, y));
    
    // free dynamically allocated memory
    for (int i = 0; i < n; i++) {
        free(adj[i]);
    }
    free(adj);
    free(data);
    
    return 0;
}

int reach(int** adj, int n, int x, int y) {
    // Allocate an array to keep track of visited vertices
    int *visited = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
         // Mark all vertices as unvisited initially
        visited[i] = 0;
    }
    // Mark the starting vertex as visited
    visited[x] = 1;

    /* Allocate a queue to store the vertices to be processed
     * a queue is used because of 'breadth-first' visits of 
     * vertices in breadth-first search (BFS) algorightm.
     */
    int *queue = malloc(n * sizeof(int));
    int front = 0;        // Index of the front of the queue
    int rear = 0;         // Index of the rear of the queue
    queue[rear++] = x;    // Add the starting vertex to the queue

    // Perform BFS until the queue is empty or the target vertex is found
    while (front < rear) {
        int u = queue[front++];      // Remove the next vertex from the queue
        for (int v = 0; v < n; v++) {
            // Check if there is an edge from u to v, and if v has not been visited yet
            if (adj[u][v] == 1 && visited[v] == 0) {
                visited[v] = 1;      // Mark v as visited
                queue[rear++] = v;   // Add v to the queue
            }
        }
    }

    // Determine if the target vertex was reached during BFS
    int result = visited[y];

    // Free the dynamically allocated arrays
    free(visited);
    free(queue);

    // Return 1 if the target vertex was reached, 0 otherwise
    return result;
}

