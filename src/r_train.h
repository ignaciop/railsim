#ifndef TRAIN_H
#define TRAIN_H

#include "r_time.h"

#define PROB_LENGTH 0.7

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

#endif