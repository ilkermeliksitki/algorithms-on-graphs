#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define QUEUE_CAPACITY 100000

// define queue structure
struct queue {
    int *nodes;
    int front;
    int rear;
};

// function prototypes
struct queue *initialize_queue(); 
void enqueue(struct queue *q, int node);
int dequeue(struct queue *q);
bool is_empty(struct queue *q);
int bipartite(int **adj_list, int *edge_count, int n);

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

    // print the result
    int result = bipartite(adj_list, edge_count, n);
    printf("%d\n", result);

    // free the memory
    for (int i = 0; i < n; ++i) {
        free(adj_list[i]);
    }
    free(adj_list);
    free(edge_count);
    return 0;
}

/**
 * bipartite - check if a graph bipartite
 * @adj_list:   adjacenty list representing the graph
 * @edge_count: array counting the number of each vertex
 * @n:          number of vertices
 *
 * this function checks if the graph being represented with
 * adjacenty list bipartite by utilizing the breadth first
 * search and assigning color to the vertices while exploring
 * them.
 *
 * Return:      1, if the graph bipartite, 0 otherwise
 *
 */
int bipartite(int **adj_list, int *edge_count, int n)
{
    struct queue *q = initialize_queue();
    int *visited = malloc(n * sizeof(int));
    bool *colored = malloc(n * sizeof(bool));
    for (int i = 0; i < n; ++i) {
        visited[i] = 0;
        colored[i] = false;
    }
    // pick the first vertex having non-zero number of edge
    int source;
    for (int i = 0; i < n; ++i) {
        if (edge_count[i] != 0) {
            source = i;
            break;
        }
    }
    bool found_color_conflict = false;
    visited[source] = 1;
    colored[source] = true;
    enqueue(q, source);
    while (!is_empty(q) && !found_color_conflict) {
        int current = dequeue(q);
        for (int i = 0; i < edge_count[current]; ++i) {
            int neighbor = adj_list[current][i];
            if (!visited[neighbor]) {
                enqueue(q, neighbor);
                visited[neighbor] = 1;
                colored[neighbor] = !colored[current];
            }
            if (colored[current] == colored[neighbor]) {
                found_color_conflict = true;
                break;
            }
        }
    }
    free(visited);
    free(colored);
    free(q->nodes);
    free(q);
    return found_color_conflict ? 0 : 1;
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
