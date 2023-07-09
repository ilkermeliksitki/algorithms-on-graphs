#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define QUEUE_CAPACITY 1000000

struct queue {
    int *nodes;
    int front;
    int rear;
};

// function prototypes
int dequeue(struct queue *q);
void enqueue(struct queue *q, int node);
bool is_empty(struct queue *q);
int bfs(int u, int v, int **adj_list, int n, int *edge_count);
struct queue *initialize_queue();

int main(void)
{
    // get # of vertices and # of edges
    int n, m;
    scanf("%d %d", &n, &m);

    // create adjacency matrix
    int **adj_list = calloc(n, sizeof(int *));
    int *edge_count = calloc(n, sizeof(int));

    // fill adjacency matrix dynamically based on the edges
    int a, b;
    for (int i = 0; i < m; ++i) {
        scanf("%d %d", &a, &b);
        a--; b--;
        adj_list[a] = realloc(adj_list[a], (edge_count[a] + 1) * sizeof(int));
        adj_list[a][edge_count[a]++] = b;

        adj_list[b] = realloc(adj_list[b], (edge_count[b] + 1) * sizeof(int));
        adj_list[b][edge_count[b]++] = a;
    }

    // get the vertices that is being used to calculate min distance
    int u, v;
    scanf("%d %d", &u, &v);

    // print the result
    int min_number_of_flight_segment = bfs(u - 1, v - 1, adj_list, n, edge_count);
    printf("%d\n", min_number_of_flight_segment);

    // free allocated memory
    for (int i = 0; i < n; ++i) {
        free(adj_list[i]);
    }
    free(adj_list);
    free(edge_count);
    return 0;
}

/**
 * bfs - breadth first search function
 * @source:     source vertex index
 * @target:     target vertex index
 * @adj_list:   adjacency list representing undirected graph
 * @n:          number of vertices
 * @edge_count: array keeping track of number of edges for a specific edge
 *
 * This function performs breadth first search and while doing that it fills
 * the minimum distance array @dist. Couple of notes about the function:
 *     - it keeps track of the visited vertices if dist == -1
 *     - queue data structure is used to process the same level vertices
 *         sequentially.
 *     - is_target_found flag is used to make the algorith efficient. When
 *         the target is found, there is no need to continue bfs.
 *
 * Return:      minimum distance between target and source vertices
 */
int bfs(int source, int target, int **adj_list, int n, int *edge_count)
{
    struct queue *q = initialize_queue();
    int *dist = malloc(n * sizeof(int));
    for (int i = 0; i < n; ++i) {
        dist[i] = -1;
    }

    bool is_target_found = false;
    dist[source] = 0;
    enqueue(q, source);
    while (!is_empty(q) && !is_target_found) {
       int current = dequeue(q);
       for (int i = 0; i < edge_count[current]; ++i) {
           int neighbor = adj_list[current][i];
           if (dist[neighbor] == -1) {
              enqueue(q, neighbor);
              dist[neighbor] = dist[current] + 1;
           }
           // check if the target node is found, if so, no need to go furhter
           if (neighbor == target) {
               is_target_found = true;
               break;
           }
       }
    }

    int source_target_min_distance = dist[target];

    // free memory
    free(q->nodes);
    free(q);
    free(dist);

    return source_target_min_distance;
}

/**
 * enqueue - add a node to the queue
 * @q:    pointer to the queue data structure
 * @node: node to be added to the queue
 */
void enqueue(struct queue *q, int node)
{
    q->nodes[q->rear] = node;
    q->rear = (q->rear + 1) % QUEUE_CAPACITY;
}

/**
 * dequeue - remove and return the front node from the queue
 * @q:      pointer to the queue data structure
 *
 * Return : dequeued node from the queue
 */
int dequeue(struct queue *q) {
    int node = q->nodes[q->front];
    q->front = (q->front + 1) % QUEUE_CAPACITY;
    return node;
}

/**
 * is_empty - check whether the queue is empty or not
 * @q:     pointer to the queue data structure
 *
 * Return: true if @q is empty, false if not
 *
 */
bool is_empty(struct queue *q)
{
    if (q->front == q->rear)
        return true;
    return false;
}

/**
 * initialize_queue - initialize the queue
 * Return: the pointer for the initialized queue
 */
struct queue *initialize_queue()
{
    struct queue *q = malloc(sizeof(struct queue));
    q->nodes = malloc(QUEUE_CAPACITY * sizeof(int));
    q->front = 0;
    q->rear = 0;
    return q;
}

