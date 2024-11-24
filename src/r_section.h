#ifndef SECTION_H
#define SECTION_H

#include "sg_queue.h"

struct section {
    char header;
    struct sg_queue *trains;
    int dispatched_trains;
};

struct section *new_section(char header);
void delete_section(struct section **s);
void add_train(struct section *s);
void dispatch_train(struct section *s, char dest);

#endif