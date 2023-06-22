#include <stdio.h>
#include <stdlib.h>

// function prototypes
void topological_sort(int **adj_list, int *out_going_edge_count, int n);
void dfs(int v, int **adj_list, int *out_going_edge_count, int *visited, int *rec_stack, int *top);
void push(int *rec_stack, int *top, int v);
int pop(int *rec_stack, int *top);

int main() {
    // get #vertices and #edges
    int n, m;
    scanf("%d%d", &n, &m);
    
    // initialize adjacency list and out_going_edge_count array
    int **adj_list = calloc(n, sizeof(int *));
    int *out_going_edge_count = calloc(n, sizeof(int));

    /** 
     * fill the adjacency list
     * example:
     *     vertices: 1, 2, 3, 4, 5
     *     edges: (1, 2), (1, 3), (2, 4), (3, 5)
     *
     *     adj_list[0] = {2, 3}  vertex 1 --> vertex 2, and vertex 3
     *     adj_list[1] = {4}     vertex 2 --> vertex 4
     *     adj_list[2] = {5}     vertex 3 --> vertex 5
     *     adj_list[3] = {}      no outgoing edge for vertex 4
     *     adj_list[4] = {}      no outgoing edge for vertex 5
     * note that indices are 0-based.
     *
     * out_going_edge_count array is used to keep track of the number of
     *     outgoing edges for a particular vertex and is used to dynami-
     *     cally reallocate memory of the adjacency list.
     *     out_going_edge_count = {2, 1, 1, 0} for the previous example
     *     case.
     */

    int a, b;
    for (int i = 0; i < m; ++i) {
        scanf("%d %d", &a, &b);
        // adjust for 0-based index
        a--;
        b--;
        adj_list[a] = realloc(adj_list[a], (out_going_edge_count[a] + 1) * sizeof(int));
        adj_list[a][out_going_edge_count[a]] = b;
        out_going_edge_count[a]++;
    }

    topological_sort(adj_list, out_going_edge_count, n);
    
    // free the allocated memory
    for (int i = 0; i < n; i++) {
        free(adj_list[i]);
    }
    free(adj_list);
    free(out_going_edge_count);

    return 0;
}

/**
 * dfs - performs depth-first search from a given vertex @v
 * @v:                    the starting vertex of dfs
 * @adj_list:             the adjacency list of the DAG
 * @out_going_edge_count: the array that counts the number of outgoing edges
 * @visited:              the array keeping track of visited vertices
 * @rec_stack:            the stack used for recording topological ordering of vertices
 * @top:                  the integer pointer pointing the top of the @rec_stack
 *
 * The function performs a depth-search traversal starting from @v and recursively traverse
 * all the vertices and fills the @rec_stack array with the vertices by topological order
 *
 * Return: None
 */
void dfs(int v, int **adj_list, int *out_going_edge_count, int *visited, int *rec_stack, int *top)
{
    visited[v] = 1;
    for (int i = 0; i < out_going_edge_count[v]; ++i) {
        if (visited[adj_list[v][i]] == 0) {
            dfs(adj_list[v][i], adj_list, out_going_edge_count, visited, rec_stack, top);
        }
    }
    push(rec_stack, top, v);
}

/**
 * topological_sort - performs topological sorting of a DAG
 * @adj_list:             the adjacency list which represents DAG
 * @out_going_edge_count: the array that counts the number of outgoing edges
 * @n:                    the number of vertices of DAG
 *
 * It performs topological sort to the DAG with the help of dfs() function
 * and print out the result by poping out of the rec_stack
 *
 * Returns: None
 */
void topological_sort(int **adj_list, int *out_going_edge_count, int n)
{
    int top = -1;
    int *visited = calloc(n, sizeof(int));
    int *rec_stack = calloc(n, sizeof(int));

    for (int i = 0; i < n; ++i) {
        if (visited[i] == 0) {
            dfs(i, adj_list, out_going_edge_count, visited, rec_stack, &top);
        }
    }
    
    // print the result
    while(top >= 1) {
        // adjust for 1-based index
        printf("%d ", pop(rec_stack, &top) + 1);
    }
    printf("%d", pop(rec_stack, &top) + 1);

    // free memory
    free(visited);
    free(rec_stack);
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
