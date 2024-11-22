#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "r_train.h"
#include "r_event.h"

struct train *new_train(void) {
    struct train *nt = (struct train *)malloc(sizeof(struct train));
    
    if (nt == NULL) {
        perror("Error allocating memory for new train.\n");
        
        exit(EXIT_FAILURE);
    }
    
    nt->id = rand() % RAND_MAX;
    
    int t_length[2] = {100, 200};
    nt->length = t_length[rand() % 2];
    
    nt->arrival_time = new_event();
    nt->departure_time = new_event();
    
    char t_points[2] = {'A', 'E'};
    nt->origin = t_points[rand() % 2];
    nt->destination = (nt->origin == 'A') ? 'E' : 'A';
    
    return nt;
}

void delete_train(struct train **t) {
    delete_event(&((*t)->arrival_time));
    delete_event(&((*t)->departure_time));
    
    free(*t);
    *t = NULL;
}

void print_train(struct train *t) {
    printf("Train %d ~~> Length: %d m, Origin: %c, Dest: %c, Arrival Time: %02d:%02d:%02d, Departure Time: %02d:%02d:%02d\n",
           t->id, t->length, t->origin, t->destination, 
           t->arrival_time->hour, t->arrival_time->min, t->arrival_time->sec,
           t->departure_time->hour, t->departure_time->min, t->departure_time->sec);
}