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
    
    int t_length[2] = {100, 200};
    nt->length = t_length[rand() % 2];
    
    nt->arrival_time = new_time();
    nt->departure_time = new_time();
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