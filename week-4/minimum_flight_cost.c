#include <stdio.h>
#include <stdlib.h>

struct edge {
    int dest;
    int weight;
};

int main(void)
{

    // get # of vertices and # of edges
    int n, m;
    scanf("%d %d", &n, &m);

    struct edge **adj_list = calloc(n, sizeof(struct edge *));
    int *edge_count = calloc(n, sizeof(int));

    // fill adj_list
    int a, b, w;
    for (int i = 0; i < m; ++i) {
        scanf("%d %d %d", &a, &b, &w);
        a--; b--;
        adj_list[a] = realloc(adj_list[a], (edge_count[a] + 1) * sizeof(struct edge));
        adj_list[a][edge_count[a]].dest = b;
        adj_list[a][edge_count[a]].weight = w;
        edge_count[a]++;
    }

    int u, v;
    scanf("%d %d", &u, &v);
    u--; v--;
    int min_total_weight_between_u_and_v = distance();
    printf("%d\n", min_total_weight_between_u_and_v);

    // Don't forget to free memory when you're done using the adjacency list
    for (int i = 0; i < n; ++i) {
        free(adj_list[i]);
    }
    free(adj_list);
    free(edge_count);
    return 0;
}
