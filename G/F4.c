#include <stdio.h>
#include <stdlib.h>

typedef struct node{
    int value;
    struct node *next;
    struct node *prev;
} node_t;

void delete_node(node_t* node) {
    node->prev->next = node->next;
    node->next->prev = node->prev;
}

void add_node(node_t* list_end, node_t* new_node) {
    new_node->prev = list_end;
    new_node->next = list_end->next;
    list_end->next = new_node;
    new_node->next->prev = new_node;
}

void rearrange_list(node_t* head) {
    node_t *last_node = head->prev;
    node_t *first_big = NULL;
    node_t *current = head->next;
    node_t *tmp;

    while ((current != head) && (current != first_big)) {
        //printf("-> %d %d %d \n", current->value, last_node->value, first_big != NULL ? first_big->value : -1);
        if (current->value % 2 != 0 && current->value < 100) {
            delete_node(current);
        } else if (current->value > 100 && current != last_node) {
            if (first_big == NULL) { first_big = current; }
            tmp = current->next;
            delete_node(current);
            add_node(last_node, current);
            last_node = last_node->next;
            current = tmp;
            continue;
        } 
        current = current->next;
    }
}

void print_list_reverse(node_t* tail) {
    node_t* current = tail;

    while (current->prev != tail) {
        printf("%d ", current->value);
        current = current->prev;
    }
    printf("\n");
}

int main(void) {
    node_t *head;
    head = malloc(sizeof(node_t));
    head->value = 0; 
    head->next = head;
    head->prev = head;
    node_t *tail = head; int x;

    while (scanf("%d", &x) == 1){
        node_t *new_node;
        new_node = malloc(sizeof(node_t));
        new_node->value = x; 
        new_node->next = NULL;
        new_node->prev = NULL;
        add_node(tail, new_node);
        tail = new_node;
    }

    // Call the target function
    rearrange_list(head);
    // Print the result in reverse order
    print_list_reverse(head->prev);

    return 0;
}