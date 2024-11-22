#ifndef TRAIN_H
#define TRAIN_H

#include <time.h>

#include "r_event.h"

struct train {
    int id;
    int length;
    struct tm_event *arrival_time;
    struct tm_event *departure_time;
    char origin;
    char destination;
};

struct train *new_train(void);
void delete_train(struct train **t);
void print_train(struct train *t);

#endif