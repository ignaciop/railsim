#ifndef SECTION_H
#define SECTION_H

#include <pthread.h>

#include "sg_queue.h"

struct section {
    double prob_arrive;
    struct sg_queue *trains;
    pthread_mutex_t mtx;
    pthread_cond_t cv;
    char header;
};

struct section *new_section(const char header, const double prob_arrive);
void delete_section(struct section **s);

#endif