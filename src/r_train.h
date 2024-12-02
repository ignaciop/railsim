#ifndef TRAIN_H
#define TRAIN_H

#include "r_time.h"

#define TRAIN_MAX_IDS 1000
#define TRAIN_LENGTH_1 100
#define TRAIN_LENGTH_2 200
#define TRAIN_PROB_LENGTH 0.7

struct train {
    int id;
    int length;
    char origin;
    char destination;
    struct r_time *arrival_time;
    struct r_time *departure_time;
};

struct train *new_train(void);
void delete_train(struct train **t);

#endif