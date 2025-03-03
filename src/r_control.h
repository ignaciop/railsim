#ifndef CONTROL_H
#define CONTROL_H

#include <pthread.h>

#include "r_train.h"
#include "r_section.h"

#define CONTROL_NUM_SECTIONS 4
#define CONTROL_PROB_BRANCH 0.5
#define CONTROL_QT_THRESHOLD 10
#define CONTROL_NEW_TRAINS_TIME 1
#define CONTROL_ROUNDS_MIN 1

#define SECTION_PROB_ARRIVE_MIN 0.0
#define SECTION_PROB_ARRIVE_MAX 1.0
#define SECTION_TRAVEL_TIME 1
#define SECTION_ARRIVE_TIME 1

#define TRAIN_PROB_BREAKDOWN 0.1
#define TRAIN_BREAKDOWN_TIME 4
#define TRAIN_LENGTH_1_TRAVEL_TIME 1
#define TRAIN_LENGTH_2_TRAVEL_TIME 2

struct control {
    pthread_mutex_t mtx;
    struct section **sections;
    int l1_passed_trains;
    int l2_passed_trains;
    int l3_passed_trains;
    int l4_passed_trains;
    int overloads;
    int queued_acc;
    int breakdowns;
    int rounds;
};

struct control *new_control(const double prob_arrive, const int rounds);
void delete_control(struct control **c);
void *tunnel_control(void *arg);
void *add_train(void *arg);

#endif