#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// function prototypes
int pop(int *rec_stack, int *top);
void push(int *rec_stack, int *top, int v);
int find_sccs(int **adj_list, int *out_going_edge_count, int n);
int **reverse_adj_list(int **adj_list, int *out_going_edge_count, int *incoming_edge_count, int n);

int main(void)
{
    // get # of vertices and # of edges
    int n, m;
    scanf("%d %d", &n, &m);

    // initialize adjacency list and out_going_edge_count array
    int **adj_list = calloc(n, sizeof(int *));
    int *out_going_edge_count = calloc(n, sizeof(int));

    int a, b;
    for (int i = 0; i < m; ++i) {
        scanf("%d %d", &a, &b);
        // adjust for 0-based index
        a--;
        b--;
        adj_list[a] = realloc(adj_list[a], (out_going_edge_count[a] + 1) * sizeof(int));
        adj_list[a][out_going_edge_count[a]++] = b;
    }

    // strongly CC
    int scc_number = find_sccs(adj_list, out_going_edge_count, n);

    // print the result
    printf("%d\n", scc_number);
    
    // free the allocated memory
    for (int i = 0; i < n; ++i) {
        free(adj_list[i]);
    }
    free(adj_list);
    free(out_going_edge_count);

    return 0;
}

/**
 * dfs - depth-first search function
 * @v:                    current vertex
 * @adj_list:             adjacency list representing directed graph
 * @out_going_edge_count: array storing the outgoing edge number per vertex
 * @visited:              array storing the visited vertices
 * @rec_stack:            stack array storing the recursively called vertices in order
 * @top:                  pointer indicating the top location of @rec_stack
 *
 * Return:                None
 */
void dfs(int v, int **adj_list, int *out_going_edge_count, int *visited, int *rec_stack, int *top)
{
    visited[v] = 1;
    for (int i = 0; i < out_going_edge_count[v]; ++i) {
        int destination_vertex_idx = adj_list[v][i];
        if (visited[destination_vertex_idx] == 0) {
            dfs(destination_vertex_idx, adj_list, out_going_edge_count, visited, rec_stack, top);
        }
    }
    push(rec_stack, top, v);
}

/**
 * reverse_adj_list - reverse the directions of the adjacenty list
 * @adj_list:             adjacency list representing directed graph
 * @out_going_edge_count: array storing the outgoing edge number per vertex
 * @incoming_edge_count:  array storing the incoming edge number per vertex
 * @n:                    number of vertices
 *
 * Return:                2D reversed adjacency list
 *
 */
int **reverse_adj_list(int **adj_list, int *out_going_edge_count, int *incoming_edge_count, int n)
{
    int **reversed_adj_list = calloc(n, sizeof(int *));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < out_going_edge_count[i]; ++j) {
            int dest_vertex_idx = adj_list[i][j];
            reversed_adj_list[dest_vertex_idx] = realloc(reversed_adj_list[dest_vertex_idx], (incoming_edge_count[dest_vertex_idx] + 1) * sizeof(int));
            reversed_adj_list[dest_vertex_idx][incoming_edge_count[dest_vertex_idx]] = i;
            incoming_edge_count[dest_vertex_idx]++;
        }
    }

    return reversed_adj_list;
}

/**
 * find_sccs - finds strongly connected components of a graphs
 * @adj_list:             adjacency list representing directed graph
 * @out_going_edge_count: array storing the outgoing edge number per vertex
 * @n:                    number of vertices
 *
 * The function performs DFS first on G^R and then fill the the @rec_stack
 * in the order of visit. Then, again, perfoms DFS on G to count the number
 * of SCCs. Every performed DFS for a particular vertex means one SCC.
 *
 * Return:                the number of SCCs
 */
int find_sccs(int **adj_list, int *out_going_edge_count, int n)
{
    int top = -1;
    int *visited = calloc(n, sizeof(int));
    int *rec_stack = calloc(2 * n, sizeof(int));
    int *incoming_edge_count = calloc(n, sizeof(int));

    // create the reversed adj_list (graph)
    int **reversed_adj_list = reverse_adj_list(adj_list, out_going_edge_count, incoming_edge_count, n);

    // dfs on G^R
    for (int i = 0; i < n; ++i) {
        if (visited[i] == 0) {
            dfs(i, reversed_adj_list, incoming_edge_count, visited, rec_stack, &top);
        }
    }

    // reset visited array to zeros
    memset(visited, 0, n * sizeof(int));

    // find the number of strongly connected components (SCCs)
    int scc_count = 0;
    while (top != -1) {
        int v = pop(rec_stack, &top);
        if (visited[v] == 0) {
            dfs(v, adj_list, out_going_edge_count, visited, rec_stack, &top);
            scc_count++;
        }
    }

    // free the allocated memory
    for (int i = 0; i < n; ++i) {
        free(reversed_adj_list[i]);
    }
    free(reversed_adj_list);
    free(visited);
    free(rec_stack);
    free(incoming_edge_count);

    return scc_count;
}

/**
 * push - pushes the vertext @v to the @rec_stack
 * @rec_stack: the stack used for recording topological ordering of vertices
 * @top:       the integer pointer pointing the top of the @rec_stack
 * @v:         the vertex
 *
 * Returns: None
 */
void push(int *rec_stack, int *top, int v)
{
    rec_stack[++(*top)] = v;
}

/**
 * pop - pop a value from the @rec_stack
 *
 * @rec_stack: the stack used for recording topological ordering of vertices
 * @top:       the integer pointer pointing the top of the @rec_stack
 *
 * Returns:    the value of the popped value.
 */
int pop(int *rec_stack, int *top)
{
    return rec_stack[(*top)--];
}

