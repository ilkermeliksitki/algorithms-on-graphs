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
int bfs(int u, int v, int **adj_list, int n);

int main(void)
{
    // get # of vertices and # of edges
    int n, m;
    scanf("%d %d", &n, &m);

    // create adjacency matrix
    int **adj_list;
    adj_list = malloc(n * sizeof(int *));
    for (int i = 0; i < n; ++i) {
        adj_list[i] = calloc(n, sizeof(int));
    }

    // fill adjacency matrix based on the edges
    int a, b;
    for (int i = 0; i < m; ++i) {
        scanf("%d %d", &a, &b);
        adj_list[a - 1][b - 1] = 1;
        adj_list[b - 1][a - 1] = 1;
    }

    // get the vertices that is being used to calculate min distance
    int u, v;
    scanf("%d %d", &u, &v);

    int min_number_of_flight_segment = bfs(u - 1, v - 1, adj_list, n);
    printf("%d\n", min_number_of_flight_segment);

    // free allocated memory
    for (int i = 0; i < n; ++i) {
        free(adj_list[i]);
    }
    free(adj_list);
    return 0;
}

struct queue *initialize_queue()
{
    struct queue *q = malloc(sizeof(struct queue));
    q->nodes = malloc(QUEUE_CAPACITY * sizeof(int));
    q->front = 0;
    q->rear = 0;
    return q;
}

int bfs(int source, int target, int **adj_list, int n)
{
    struct queue *q = initialize_queue();
    int *visited = calloc(n, sizeof(int));
    int *dist = malloc(n * sizeof(int));
    for (int i = 0; i < n; ++i) {
        dist[i] = -1;
    }
    
    dist[source] = 0;
    enqueue(q, source);
    while (!is_empty(q)) {
       int current = dequeue(q);
       for (int i = 0; i < n; ++i) {
           if (adj_list[current][i] == 1 && !visited[i]) {
               enqueue(q, i);
               visited[i] = 1;
               dist[i] = dist[current] + 1;
           }
       }
    }

    int source_target_min_distance = dist[target];

    // free memory
    free(q->nodes);
    free(q);
    free(visited);
    free(dist);
    return source_target_min_distance;
}

void enqueue(struct queue *q, int node)
{
    q->nodes[q->rear] = node;
    q->rear = (q->rear + 1) % QUEUE_CAPACITY;
}

int dequeue(struct queue *q) {
    int node = q->nodes[q->front];
    q->front = (q->front + 1) % QUEUE_CAPACITY;
    return node;
}

bool is_empty(struct queue *q)
{
    if (q->front == q->rear)
        return true;
    return false;
}

