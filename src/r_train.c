#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "r_train.h"

struct train *new_train() {
    struct train *nt = (struct train *)malloc(sizeof(struct train));
    
    if (nt == NULL) {
        perror("Error allocating memory for new train.\n");
        
        exit(EXIT_FAILURE);
    }
    
    nt->id = rand() % 1000;
    
    double p = (double)rand() / RAND_MAX;
            
    nt->length = (p < PROB_LENGTH) ? 100 : 200;
    
    //time_t now = time(NULL);
    //nt->arrival_time = localtime(&now);
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