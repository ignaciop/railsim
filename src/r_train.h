#ifndef TRAIN_H
#define TRAIN_H

#include "r_time.h"

struct train {
    int id;
    int length;
    struct r_time *arrival_time;
    struct r_time *departure_time;
    char origin;
    char destination;
};

struct train *new_train(void);
void delete_train(struct train **t);
void print_status(struct train *t);

#endif