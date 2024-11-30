#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "r_train.h"

struct train *new_train(void) {
    struct train *nt = (struct train *)malloc(sizeof(struct train));
    
    if (nt == NULL) {
        perror("Error allocating memory for new train.\n");
        
        exit(EXIT_FAILURE);
    }
    
    nt->id = rand() % TRAIN_MAX_IDS;
    
    double p = (double)rand() / RAND_MAX;
            
    nt->length = (p < TRAIN_PROB_LENGTH) ? TRAIN_LENGTH_1 : TRAIN_LENGTH_2;
    
    nt->arrival_time = new_time();
    nt->departure_time = NULL;
    nt->origin = ' ';
    nt->destination = ' ';
    
    return nt;
}

void delete_train(struct train **t) {
    delete_time(&((*t)->arrival_time));
    delete_time(&((*t)->departure_time));
    
    free(*t);
    *t = NULL;
}