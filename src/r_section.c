#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "r_section.h"
#include "r_train.h"

struct section *new_section(char header, double prob_arrive) {
    struct section *ns = (struct section *)malloc(sizeof(struct section));
    
    if (ns == NULL) {
        perror("Error allocating memory for new section.\n");
        
        exit(EXIT_FAILURE);
    }
    
    ns->header = header;
    ns->prob_arrive = prob_arrive;
    ns->trains = sg_queue_new();
    ns->dispatched_trains = 0;
    
    ns->mtx = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(ns->mtx, NULL);
    
    ns->cv = (pthread_cond_t *)malloc(sizeof(pthread_cond_t));
    pthread_cond_init(ns->cv, NULL);
    
    return ns;
}

void delete_section(struct section **s) {
    if ((*s)->trains != NULL) {
        for (size_t i = 0; i < (*s)->trains->size; i++) {
            struct train *t = (struct train *)sg_queue_dequeue((*s)->trains);
            
            delete_train(&t);
        }
        
        free((*s)->trains);
        (*s)->trains = NULL;
    }
    
    pthread_cond_destroy((*s)->cv);
    pthread_mutex_destroy((*s)->mtx);
    
    free(*s);
    *s = NULL;
}

/*
void dispatch_train(struct section *s, char dest) {
    if (sg_queue_size(s->trains) != 0) {
        struct train *t = (struct train *)sg_queue_dequeue(s->trains);
        t->destination = dest;
        
        print_status(t); // Only temp to output. This will be managed by control center once the train passes tunnel
        
        s->dispatched_trains++;
        
        delete_train(&t);
    }
}
*/