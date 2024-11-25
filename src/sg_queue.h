#ifndef SG_QUEUE_H
#define SG_QUEUE_H

struct sg_queue_node {
    struct sg_queue_node *next;
    void *data;
};

struct sg_queue {
    struct sg_queue_node *head;
    struct sg_queue_node *tail;
    size_t size;
};

struct sg_queue *sg_queue_new(void);
void sg_queue_enqueue(struct sg_queue *q, void *data);
void *sg_queue_dequeue(struct sg_queue *q);
size_t sg_queue_size(struct sg_queue *q);
void sg_queue_delete(struct sg_queue **q);

#endif