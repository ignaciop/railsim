#include <stdio.h>
#include <stdlib.h>

#include "r_section.h"
#include "r_train.h"

struct section *new_section(const char header, const double prob_arrive) {
    struct section *ns = (struct section *)malloc(sizeof(struct section));
    
    if (ns == NULL) {
        perror("Error allocating memory for new section.\n");
        
        exit(EXIT_FAILURE);
    }
    
    ns->prob_arrive = prob_arrive;
    ns->header = header;
    ns->trains = sg_queue_new();
    
    ns->mtx = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
    
    if (ns->mtx == NULL) {
        perror("Error allocating memory for new section mutex.\n");
        
        exit(EXIT_FAILURE);
    }
    
    pthread_mutex_init(ns->mtx, NULL);
    
    ns->cv = (pthread_cond_t *)malloc(sizeof(pthread_cond_t));
    
    if (ns->cv == NULL) {
        perror("Error allocating memory for new section condvar.\n");
        
        exit(EXIT_FAILURE);
    }
    
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
    (*s)->cv = NULL;
    
    pthread_mutex_destroy((*s)->mtx);
    (*s)->mtx = NULL;

    free(*s);
    *s = NULL;
}