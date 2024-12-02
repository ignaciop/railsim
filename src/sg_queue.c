#include <stdio.h>
#include <stdlib.h>

#include "sg_queue.h"

struct sg_queue *sg_queue_new(void) {
    struct sg_queue *q = (struct sg_queue *)malloc(sizeof(struct sg_queue));
    
    if (q == NULL) {
        perror("Error allocating memory for new queue");
        
        exit(EXIT_FAILURE);    
    }
    
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    
    return q;
}

void sg_queue_enqueue(struct sg_queue *q, void *data) {
    if (q == NULL) {
        return;
    }
    
    struct sg_queue_node *new_node = (struct sg_queue_node *)malloc(sizeof(struct sg_queue_node));
    
    if (new_node == NULL) {
        perror("Error allocating memory for new node");
        
        exit(EXIT_FAILURE);    
    }
    
    new_node->data = data;
    new_node->next = NULL;
    
    if (sg_queue_size(q) == 0) {
        q->head = q->tail = new_node;
    } else {
        q->tail->next = new_node;
        q->tail = new_node;
    }
    
    q->size++;
}

void *sg_queue_dequeue(struct sg_queue *q) {
    if (q == NULL || sg_queue_size(q) == 0) {
        return NULL;
    }
    
    struct sg_queue_node *del_node = q->head;
    void *data = del_node->data;
    
    q->head = q->head->next;
    
    /* 
     * Queue user has to explicitly delete node data with data's own dealloc function
     *
     * free(del_node->data);
     * del_node->data = NULL;
     *
     */
    
    free(del_node);
    del_node = NULL;
    
    q->size--;
    
    return data;
}

size_t sg_queue_size(const struct sg_queue *q) {
    size_t qs = (q == NULL) ? 0 : q->size;
    
    return qs;
}

void sg_queue_delete(struct sg_queue **q) {
    free(*q);
    *q = NULL;
}