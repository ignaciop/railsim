#include <stdio.h>
#include <stdlib.h>

#include "r_section.h"
#include "r_train.h"

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cv = PTHREAD_COND_INITIALIZER;

struct section *new_section(const char header, const double prob_arrive) {
    struct section *ns = (struct section *)malloc(sizeof(struct section));
    
    if (ns == NULL) {
        perror("Error allocating memory for new section.\n");
        
        exit(EXIT_FAILURE);
    }
    
    ns->prob_arrive = prob_arrive;
    ns->trains = sg_queue_new();
    
    ns->mtx = mtx;
    ns->cv = cv;
    
    ns->header = header;
    
    return ns;
}

void delete_section(struct section **s) {
    if (s != NULL) {
        if ((*s)->trains != NULL) {
            for (size_t i = 0; i < (*s)->trains->size; i++) {
                struct train *t = (struct train *)sg_queue_dequeue((*s)->trains);
                
                delete_train(&t);
            }
            
            sg_queue_delete(&((*s)->trains));
        }

        free(*s);
        *s = NULL;
    }
}