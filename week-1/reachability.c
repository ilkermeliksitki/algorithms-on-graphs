#include <stdio.h>
#include <stdlib.h>

struct node {
    int vertex;
    struct node *next;
};

struct node **create_adj_list(int n, int m);

int main(void)
{
    int n, m;
    scanf("%d %d", &n, &m);
    struct node **adj_list = create_adj_list(n , m);
    return 0;
}

struct node **create_adj_list(int n, int m)
{
    struct node **adj_list = malloc(sizeof(struct node *) * (n + 1));
    for (int i = 0; i < n + 1; ++i) {
        adj_list[i] = NULL;
    }
    int a, b;
    for (int i = 0; i < m; ++i) {
        scanf("%d %d", &a, &b);
        struct node *new_node = malloc(sizeof(struct node));
        new_node->vertex = a;
        new_node->next = NULL;
        if (adj_list[a] == NULL) {
            adj_list[a] = new_node;
        } else {
            struct node *second_node = adj_list[a]->next;
            new_node->next = second_node;
            adj_list[a]->next = new_node;
        }
    }
    return adj_list;
}
