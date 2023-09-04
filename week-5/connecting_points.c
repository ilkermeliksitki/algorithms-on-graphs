#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct point {
    int x;
    int y;
};

struct edge {
    int src;
    int dest;
    double weight;
};

// calculate distance between two points
double distance(struct point p1, struct point p2) {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

// compare two edges based on their weights for sorting
int compare_edges(const void *a, const void *b) {
    return ((struct edge*)a)->weight > ((struct edge*)b)->weight ? 1 : -1;
}

// find the parent of a vertex (used in disjoint set union)
int find_parent(int parent[], int vertex) {
    if (parent[vertex] == -1)
        return vertex;
    return find_parent(parent, parent[vertex]);
}

// "union" sets basically by changing their parent
void union_sets(int parent[], int x, int y) {
    int root_x = find_parent(parent, x);
    int root_y = find_parent(parent, y);
    parent[root_x] = root_y;
}

// generate all possible edges between points
void generate_edges(struct point points[], struct edge edges[], int n)
{
    int edge_index = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            edges[edge_index].src = i;
            edges[edge_index].dest = j;
            edges[edge_index].weight = distance(points[i], points[j]);
            edge_index++;
        }
    }
}

// Kruskal's algorithm to find the minimum spanning tree
double kruskal_mst(struct point points[], int n) {
    int num_edges = n * (n - 1) / 2;

    struct edge edges[num_edges];
    generate_edges(points, edges, n);

    // sort the edges in non-decreasing order of weight
    qsort(edges, num_edges, sizeof(struct edge), compare_edges);
    
    // initialize parent array to keep track of parent of each vertex (disjoint set), -1 means root
    int parent[n];
    for (int i = 0; i < n; i++) {
        parent[i] = -1;
    }
    
    double total_length = 0.0;
    
    // iterate through the sorted edges and add them to the MST
    for (int i = 0; i < num_edges; i++) {
        int src = edges[i].src;
        int dest = edges[i].dest;
        double weight = edges[i].weight;
        
        // check if including this edge forms a cycle in MST
        if (find_parent(parent, src) != find_parent(parent, dest)) {
            // add the edge in MST
            total_length += weight;
            union_sets(parent, src, dest);
        }
    }
    
    return total_length;
}

int main() {
    int n;
    scanf("%d", &n);
    
    struct point points[n];
    
    for (int i = 0; i < n; i++) {
        scanf("%d %d", &points[i].x, &points[i].y);
    }
    
    double total_length = kruskal_mst(points, n);
    printf("%.9f\n", total_length);
    
    return 0;
}

