#ifndef CONTROL_H
#define CONTROL_H

#include <pthread.h>

#include "r_train.h"

#define NUM_SECTIONS 4

struct control {
    double prob_arrive;
    double prob_depart;
    struct section **sections;
    int l1_passed_trains;
    int l2_passed_trains;
    int l3_passed_trains;
    int l4_passed_trains;
    int overloads;
    int breakdowns;
    pthread_mutex_t *mtx;
    pthread_cond_t *cv;
};

struct control *new_control(double prob_arrive, double prob_depart);
void delete_control(struct control **c);

void *tunnel_control(void *arg);

void print_status(char *sign, struct train *t);

void set_train_destination(char header, struct train *t);
//void print_summary(struct control *c);

#endif