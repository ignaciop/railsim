#ifndef SECTION_H
#define SECTION_H

#include <pthread.h>
#include <stdbool.h>

#include "sg_queue.h"

struct section {
    char header;
    double prob_arrive;
    struct sg_queue *trains;
    int dispatched_trains;
    pthread_mutex_t *mtx;
    pthread_cond_t *cv;
};

struct section *new_section(char header, double prob_arrive);
void delete_section(struct section **s);

/*void dispatch_train(struct section *s, char dest);*/

#endif