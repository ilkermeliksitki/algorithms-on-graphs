#include <stdio.h>
#include <stdlib.h>

#define HEAP_SIZE_MAX 100000
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
void decrease_key(struct min_heap *Q, int v, long long int d);
long long int dist(struct min_heap *Q, int j);
void sift_up(struct min_heap *h, int i);
void sift_down(struct min_heap *h, int i);
struct min_heap *init_min_heap(int n);
void build_min_heap(struct min_heap *h);
struct min_heap *initialize_single_sources(int n, int source);
void swap(struct vertex *a, struct vertex *b);

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

/**
 * dijkstra - Find the shortest path length using Dijkstra's algorithm.
 *
 * @adj:   adjacency list representing the graph.
 * @cost:  cost matrix representing the weights of edges in the graph.
 * @edges: array representing the number of edges for each vertex.
 * @s:     source vertex index.
 * @t:     target vertex index.
 * @n:     number of vertices in the graph.
 *
 * Finds the shortest path length from the source vertex @s to the
 * target vertex @t in a weighted graph represented by an adjacency list
 * and a cost matrix. The function returns the length of the shortest path.
 * If there is no path from the source to the target, the function returns -1.
 *
 * Returns:
 * The length of the shortest path from @s to @t. If no path exists, returns -1.
 */
long long int dijkstra(int **adj, int **cost, int *edges, int s, int t, int n)
{
    // allocate an array to store vertex distances
    struct vertex *distances = malloc(n * sizeof(struct vertex));

    // initialize a min heap with single sources to use as priority queue
    struct min_heap *Q = initialize_single_sources(n, s);

    while (Q->heap_size != 0) {
        struct vertex u = extract_min(Q);
        distances[u.index] = u;

        // process adjacent vertices of the vertex u
        for (int i = 0; i < edges[u.index]; ++i) {
            int adj_vertex_idx = adj[u.index][i];
            long long int prev_dist = dist(Q, adj_vertex_idx);

            // vertex is not in the priotiry queue if prev_dist == -1
            // i.e. it is already discovered
            if (prev_dist == -1) {
                continue;
            }

            // relaxation step
            long long int curr_dist = u.dist + cost[u.index][i];
            if (prev_dist > curr_dist) {
                decrease_key(Q, adj_vertex_idx, curr_dist);
            }
        }
    }

    // fine-tune the output
    long long int shortest_path = distances[t].dist;
    if (shortest_path == INFINITY)
        shortest_path = -1;

    // free the allocated memory
    free(distances);
    free(Q);

    return shortest_path;
}

/**
 * extract_min - Extracts the minimum element from a min heap.
 *
 * @h: Pointer to the min heap data structure.
 *
 * Extracts the vertex having minimum dist key from the given
 * min heap and maintains the min heap property.
 *
 * Return: The minimum vertex extracted from the min heap.
 */
struct vertex extract_min(struct min_heap *h)
{
    struct vertex min = h->H[0];
    h->H[0] = h->H[h->heap_size - 1];
    h->heap_size--;
    sift_down(h, 0);
    return min;
}

/**
 * decrease_key - Decreases the key (distance) of a specific element in the min_heap.
 *
 * @Q: min_heap structure.
 * @v: the index of the element whose distance needs to be decreased.
 * @d: the new distance for the element at index 'v'.
 *
 * Decrease the distance (key) of a specific element with index 'v'
 * in the min_heap 'Q' to the new distance 'd'. After updating the
 * distance, the function ensures that the min_heap property is
 * maintained by performing a "sift_up" operation. Note that
 * decrease_key name is a bit misleading. It is used because of the
 * convention. The function actually update the dist value of a
 * specific vertex to a lower value.
 */
void decrease_key(struct min_heap *Q, int v, long long int d)
{
    for (int i = 0; i < Q->heap_size; ++i) {
        if (Q->H[i].index == v) {
            Q->H[i].dist = d;
            sift_up(Q, i);
        }
    }
}

/**
 * dist - finds the distance associated with a given index in the min_heap.
 *
 * @Q: Pointer to the min_heap.
 * @j: The index of the vertex
 *
 * Linearly searches the distance value of a specified vertex with
 * the given index.
 *
 * Return: the distance associated with the given index,
 *         or -1 if the index is not found.
 */
long long int dist(struct min_heap *Q, int j)
{
    for (int i = 0; i < Q->heap_size; ++i) {
        if (Q->H[i].index == j) {
            return Q->H[i].dist;
        }
    }
    return -1;
}

/**
 * initialize_single_sources - initializes a single-sources
 *
 * @n:      the total number of vertices in the graph
 * @source: the index of the source vertex
 *
 * Initializes a priotiry queue and fill the distances of the
 * vertices with INFINITY except the source vertex. Its distance
 * is initialized with 0.
 *
 * Return: pointer to the initialized min_heap structure
 */
struct min_heap *initialize_single_sources(int n, int source)
{
    struct min_heap *Q = init_min_heap(n);
    for (int i = 0; i < n; ++i) {
        struct vertex a;
        a.index = i;
        a.dist = INFINITY;
        Q->H[i] = a;
    }
    Q->H[source].dist = 0;
    build_min_heap(Q);
    return Q;
}

void sift_up(struct min_heap *h, int i)
{
    int parent = (i - 1) / 2;
    while (i > 0 && h->H[i].dist < h->H[parent].dist) {
        swap(&h->H[i], &h->H[parent]);
        i = parent;
        parent = (i - 1) / 2;
    }
}

void sift_down(struct min_heap *h, int i)
{
    // left child of ith element of the heap tree
    int l = 2 * i + 1;
    // right child of ith element of the heap tree
    int r = 2 * i + 2;

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
        sift_down(h, smallest);
    }
}

struct min_heap *init_min_heap(int n)
{
    struct min_heap *h = malloc(sizeof(struct min_heap));
    h->heap_size = n;
    return h;
}

void build_min_heap(struct min_heap *h)
{
    for (int i = h->heap_size / 2; i >= 0; --i) {
        sift_down(h, i);
    }
}

void swap(struct vertex *a, struct vertex *b)
{
    struct vertex temp = *a;
    *a = *b;
    *b = temp;
}

