#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// function prototypes
bool acyclic(int **adj_list, int n);
bool dfs(int v, int **adj_list, int *visited, int *recursion_stack,  int n);

int main() {
    // get #vertices and #edges
    int n, m;
    scanf("%d%d", &n, &m);
    
    // initialize adjacency list with zeros
    int **adj_list = malloc(n * sizeof(int *));
    for (int i = 0; i < n; ++i) {
        adj_list[i] = calloc(n, sizeof(int));
    }
    
    // fill the adjacency list 
    int a, b;
    for (int i = 0; i < m; ++i) {
        scanf("%d %d", &a, &b);
        adj_list[a - 1][b - 1] = 1;
    }
    
    // print the result
    printf("%d\n", acyclic(adj_list, n));

    // free the allocated memory
    for (int i = 0; i < n; i++) {
        free(adj_list[i]);
    }
    free(adj_list);

    return 0;
}

/**
 * acyclic() - Check whether the directed graph is acyclic.
 * @adj_list:  The adjacency list containing information about the edges
 *             and the direction of the edges. Each row and column index
 *             represent the vertices. If the the value is equal to 1, it
 *             means there is an directed edge. For example:
 *             adj_list[2][3] => (2) -> (3).
 * @n:         Number of vertices in the directed graph.
 *
 * The function checks if a directed graph has a cycle in it with the help
 * of @dfs() function. It iterates through the vertices to check if there
 * is a cycle or not. If so, it return true, it not it returns false.
 *
 * Return: Return true if the directed graph acyclic i.e. DAG, otherwise
 *         it returns false
 */

bool acyclic(int **adj_list, int n)
{
    int *visited = calloc(n, sizeof(int));
    int *recursion_stack = calloc(n, sizeof(int));

    for (int i = 0; i < n; ++i) {
        if (visited[i] == 0 && dfs(i, adj_list, visited, recursion_stack, n)) {
            free(visited);
            free(recursion_stack);
            return true;
        }
    }

    free(visited);
    free(recursion_stack);

    return false;
}

/**
 * dfs() - Performs depth-first traverse to detect cycles in a directed graph.
 *
 * @v:         The current visited vertex
 * @visited:   Visited vertex array
 * @recursion_stack: Recursion stack array showing vertices in stack
 * @adj_list:  Adjacency list of the directed graph
 * @n:         The number of vertices
 *
 * This function recursively traverse the directed graph. The starting point of
 * traversal is the given vertex @v. It marks the vertex @v as visited and add
 * @v to the recursion stack. Then, with the help of the adjacency 2D array, the
 * edges between vertices detected. If the vertex is not visited, the vertices
 * recursively discovered.
 * Two returning condition appears:
 *     1-) if the vertex is not visited and there is a cycle return true.
 *     2-) if the vertex is in stack, return true.
 *
 * If there was not return in the for loop, meaning there is no cycle, then
 * remove the vertex @v from stack and return false.
 *
 * Return      Returns true if a cycle is detected, otherwise false
 */

bool dfs(int v, int **adj_list, int *visited, int *recursion_stack,  int n)
{
    // mark the current vertex as visited
    visited[v] = 1;
    // add the current vertex to stack
    recursion_stack[v] = 1;

    for (int i = 0; i < n; ++i) {
        // if there is an edge between v and i, and i is not visited, then
        // continue to traverse
        if (adj_list[v][i] == 1) {
            // if vertex is not visited and there is cyclye return true
            if (visited[i] == 0 && dfs(i, adj_list, visited, recursion_stack, n))
                return true;
            // if vertex is aldready in stack, there is a cycle. return true
            else if (recursion_stack[i] == 1)
                return true;
        }
    }

    // remove the current vertex from stack
    recursion_stack[v] = 0;
    return false;
}

