#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define INFINITY 1000000000000000000

struct vertex {
    int dist;
};

struct graph {
    int **adj;
    int **cost;
    int *edge_count;
    int n;
    int source;
    long long int *distance;
    int *shortest;
    int *reachable;
};

/* function definitions */
void free_memory(struct graph *G);

struct graph *initialize_graph(int **adj, int **cost, int *edge_count, int n, int s)
{
    struct graph *G = malloc(sizeof(struct graph));
    G->adj = adj;
    G->cost = cost;
    G->edge_count = edge_count;
    G->n = n;
    G->source = s;
    G->distance = malloc(n * sizeof(long long int));
    G->shortest = malloc(n * sizeof(int));
    G->reachable = calloc(n, sizeof(int));
    for (int i = 0; i < n; ++i) {
        G->distance[i] = INFINITY;
        G->shortest[i] = 1;
    }
    G->distance[s] = 0;
    G->reachable[s] = 1;
    return G;
}

bool relax(struct graph *G, int u_idx, int v_idx, int weight)
{
    if (G->distance[v_idx] > G->distance[u_idx] + weight) {
        G->distance[v_idx] = G->distance[u_idx] + weight;
        return true;
    }
    return false;
}

void shortest_path(struct graph *G)
{
    // run |G.V| - 1  times
    for (int k = 0; k < G->n - 1; ++k) {
        // for each edge G.E (requires two inner for loop)
        for (int i = 0; i < G->n; ++i) {
            for (int j = 0; j < G->edge_count[i]; ++j) {
                //printf("%d->%d: %d\n", i, G->adj[i][j], G->cost[i][j]);
                relax(G, i, G->adj[i][j], G->cost[i][j]);
                if (G->reachable[i]) {
                    G->reachable[G->adj[i][j]] = 1;
                }
            }
        }
        //printf("round %d is completed\n", k + 1);
    }

    // detecting vertices inside the negative loop
    for (int k = 0; k < G->n; ++k) {
        for (int i = 0; i < G->n; ++i) {
            for (int j = 0; j < G->edge_count[i]; ++j) {
                //printf("%d->%d: %d\n", i, G->adj[i][j], G->cost[i][j]);
                if (relax(G, i, G->adj[i][j], G->cost[i][j])){
                    int v_idx = G->adj[i][j];
                    G->shortest[v_idx] = 0;
                }
            }
        }
    }
}

void print_result(struct graph *G)
{
    for (int i = 0; i < G->n; ++i) {
        if (G->reachable[i] == 0)
            printf("%c\n", '*');
        else if (G->shortest[i] == 0)
            printf("%c\n", '-');
        else
            printf("%lld\n", G->distance[i]);
    }
}

int main(void)
{
    int n, m;
    scanf("%d %d", &n, &m);

    // fill adjacency list and "cost list"
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

    int source;
    scanf("%d", &source);
    source--;

    struct graph *G = initialize_graph(adj_list, cost, edge_count, n, source);
    shortest_path(G);
    print_result(G);

    free_memory(G);
    return 0;
}

void free_memory(struct graph *G)
{
    for (int i = 0; i < G->n; ++i) {
        free(G->cost[i]);
        free(G->adj[i]);
    }
    free(G->adj);
    free(G->cost);
    free(G->edge_count);
    free(G->distance);
    free(G->shortest);
    free(G->reachable);
    free(G);
}
