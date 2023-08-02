#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define HEAP_SIZE_MAX 10000
#define INFINITY 1000000000

struct vertex {
    int index;
    long long int dist;
};

struct min_heap {
    int heap_size;
    struct vertex H[HEAP_SIZE_MAX];
};

/* function prototypes */
long long int dijkstra(int **adj, int **cost, int *edges, int s, int t, int n);
struct vertex extract_min(struct min_heap *h);
struct min_heap *init_min_heap(int n);
void build_min_heap(struct min_heap *h);

int main(void)
{
    int n, m;
    scanf("%d %d", &n, &m);

    int **adj_list = calloc(n, sizeof(int *));
    int **flight_cost = calloc(n, sizeof(int *));
    int *edge_count = calloc(n , sizeof(int));

    int a, b, w;
    for (int i = 0; i < m; ++i) {
        scanf("%d %d %d", &a, &b, &w);
        a--; b--;
        adj_list[a] = realloc(adj_list[a], (edge_count[a] + 1) * sizeof(int));
        adj_list[a][edge_count[a]] = b;

        flight_cost[a] = realloc(flight_cost[a], (edge_count[a] + 1) * sizeof(int));
        flight_cost[a][edge_count[a]] = w;

        edge_count[a]++;
    }

    int s, t;
    scanf("%d %d", &s, &t);
    s--; t--;
    printf("%lld\n", dijkstra(adj_list, flight_cost, edge_count, s, t, n));

    // free the memory
    for (int i = 0; i < n; ++i) {
        free(adj_list[i]);
        free(flight_cost[i]);
    }
    free(adj_list);
    free(flight_cost);
    free(edge_count);
    return 0;
}

long long int dist(struct min_heap *Q, int j)
{
    for (int i = 0; i < Q->heap_size; ++i) {
        if (Q->H[i].index == j) {
            return Q->H[i].dist;
        }
    }
    return -1;
}

void decrease_key(struct min_heap *Q, int v, long long int d)
{
    for (int i = 0; i < Q->heap_size; ++i) {
        if (Q->H[i].index == v) {
            Q->H[i].dist = d;
            // build-min-heap is in efficient O(V), instead implement sift 
            build_min_heap(Q);
            return;
        }
    }
}

long long int dijkstra(int **adj, int **cost, int *edges, int s, int t, int n)
{
    struct vertex *distances = malloc(n * sizeof(struct vertex));
    struct min_heap *Q = init_min_heap(n);
    for (int i = 0; i < n; ++i) {
        struct vertex a;
        a.index = i;
        a.dist = INFINITY;
        Q->H[i] = a;
    }
    Q->H[s].dist = 0;
    build_min_heap(Q);

    while (Q->heap_size != 0) {
        struct vertex u = extract_min(Q);
        distances[u.index] = u;
        for (int i = 0; i < edges[u.index]; ++i) {
            //printf("vertex, adj, and cost: %d->%d (%d)\n", u.index, adj[u.index][i], cost[u.index][i]);
            int v_idx = adj[u.index][i];
            long long int current_dist = u.dist + cost[u.index][i];
            long long int prev_dist = dist(Q, v_idx);
            //printf("prev and curr: %lld %lld\n", prev_dist, current_dist);
            if (prev_dist == -1) {
                prev_dist = distances[v_idx].dist;
            }
            if (prev_dist > current_dist) {
                decrease_key(Q, v_idx, current_dist);
            }
        }
        //printf("\n");
    }
    long long int r = distances[t].dist;
    if (r == INFINITY)
        r = -1;

    free(distances);
    free(Q);
    return r;
}

void swap(struct vertex *a, struct vertex *b)
{
    struct vertex temp = *a;
    *a = *b;
    *b = temp;
}

int left(int i)
{
    return 2 * i + 1;
}

int right(int i)
{
    return 2 * i + 2;
}

int parent(int i)
{
    return (i - 1) / 2;
}

void min_heapify(struct min_heap *h, int i)
{
    // so called sift-down function
    // left child of ith element of the heap tree
    int l = left(i);
    // right child of ith element of the heap tree
    int r = right(i);

    /*l is in the limits of the heap and child is smaller than its parent*/
    int smallest;
    if (l < h->heap_size && h->H[l].dist < h->H[i].dist)
        smallest = l;
    else
        smallest = i;

    /*r is in the limits of the heap and child is smaller than its parent*/
    if (r < h->heap_size && h->H[r].dist < h->H[smallest].dist)
        smallest = r;

    if (i != smallest) {
        swap(&h->H[i], &h->H[smallest]);
        min_heapify(h, smallest);
    }
}

void build_min_heap(struct min_heap *h)
{
    for (int i = h->heap_size / 2; i >= 0; --i) {
        min_heapify(h, i);
    }
}

struct min_heap *init_min_heap(int n)
{
    struct min_heap *h = malloc(sizeof(struct min_heap));
    h->heap_size = n;
    return h;
}

struct vertex extract_min(struct min_heap *h)
{
    struct vertex min = h->H[0];
    h->H[0] = h->H[h->heap_size - 1];
    h->heap_size--;
    min_heapify(h, 0);
    return min;
}

