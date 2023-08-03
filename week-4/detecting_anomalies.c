#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define INFINITY 1000000

struct vertex {
    int dist;
};


/* function prototypes */
struct vertex *initialize_single_source (int n, int s);
bool negative_cycle(int **adj, int **cost, int *edge_count, int n);
void relax(struct vertex *vertices, int u_idx, int v_idx, int weight);

int main(void)
{
    // get # of edge and # of vertex
    int n, m;
    scanf("%d %d", &n, &m);

    // allocate memory for adjacency list and cost function.
    // edge_count is used the keep track of the number of
    // adjacency vertices of a particular vertex.
    int **adj_list = calloc(n, sizeof(int *));
    int **cost = calloc(n, sizeof(int *));
    int *edge_count = calloc(n , sizeof(int));

    int a, b, w;
    for (int i = 0; i < m; ++i) {
        scanf("%d %d %d", &a, &b, &w);
        a--; b--;
        adj_list[a] = realloc(adj_list[a], (edge_count[a] + 1) * sizeof(int));
        adj_list[a][edge_count[a]] = b;

        cost[a] = realloc(cost[a], (edge_count[a] + 1) * sizeof(int));
        cost[a][edge_count[a]] = w;

        edge_count[a]++;
    }
    
    // print the result.
    printf("%d\n", negative_cycle(adj_list, cost, edge_count, n));

    // free allocated memory
    for (int i = 0; i < n; ++i) {
        free(adj_list[i]);
        free(cost[i]);
    }
    free(adj_list);
    free(cost);
    free(edge_count);
    return 0;
}

/**
 * negative_cycle - detects the negative weighted cycle
 * 
 * @adj: adjacency list
 * @cost: cost list 
 * @edge_count: array counting the number of edges of adj, and cost
 * @n: the number of vertices
 *
 * uses bellman-ford algorithm to detect the shortest path by
 * choosing the source vertex as the 0th vertex and running
 * |G.V|-1 times while doing relaxation for each vertex. it
 * also run one more time additionally for detecting the
 * negative weighted cycle and return true if there is, return
 * false if there is no negative weighted cycle.
 *
 * Return: true, if graph contains a negative cycle, false otherwise
 */
bool negative_cycle(int **adj, int **cost, int *edge_count, int n)
{
    struct vertex *vertices = initialize_single_source(n, 0);
    // run |G.V| - 1 times
    for (int k = 0; k < n - 1; ++k) {
        // for each edge G.E (requires two inner for loop)
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < edge_count[i]; ++j) {
                relax(vertices, i, adj[i][j], cost[i][j]);
            }
        }
    }

    // run |V|th time for detecting the negative weight cycle
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < edge_count[i]; ++j) {
            struct vertex u = vertices[i]; 
            struct vertex v = vertices[adj[i][j]];
            if (v.dist > u.dist + cost[i][j]) {
                free(vertices);
                return true;
            }
        }
    }
    free(vertices);
    return false;
}

/**
 * relax - do relaxation between given vertices
 * 
 * @vertices: array containing the vertices
 * @u_idx: the index of the source vertex
 * @v_idx: the index of the target vertex
 * @weight: the weight between source and target vertex
 */
void relax(struct vertex *vertices, int u_idx, int v_idx, int weight)
{
    if (vertices[v_idx].dist > vertices[u_idx].dist + weight) {
        vertices[v_idx].dist = vertices[u_idx].dist + weight;
    }
}

/**
 * initialize_single_source - initialize the single source
 * 
 * @n: the number of vertices
 * @s: the source vertex
 *
 * initializes the vertices' dist value with INFINITY
 * except the source vertex, it is initialized with 0
 * and return the array pointer containing the initialized vertices
 *
 * Return: the pointer of array containing vertices
 */
struct vertex *initialize_single_source (int n, int s)
{
    struct vertex *vertices = malloc(n * sizeof(struct vertex));
    for (int i = 0; i < n; ++i) {
        struct vertex v;
        v.dist = INFINITY;
        vertices[i] = v;
    }
    vertices[s].dist = 0;
    
    return vertices;
}

