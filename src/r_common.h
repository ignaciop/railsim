#ifndef COMMON_H
#define COMMON_H

#include "r_control.h"
#include "r_time.h"

void process_input(int argc, char *argv[], double *prob_arrive, int *rounds);
void print_title(void);
void print_params(const double prob_arrive, const int rounds);
void print_summary(const double probability, const struct r_time *end, const struct r_time *start, const struct control *c);

#endif