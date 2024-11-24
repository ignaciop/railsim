#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "r_section.h"
#include "r_train.h"

struct section *new_section(char header) {
    struct section *ns = (struct section *)malloc(sizeof(struct section));
    
    if (ns == NULL) {
        perror("Error allocating memory for new section.\n");
        
        exit(EXIT_FAILURE);
    }
    
    ns->header = header;
    ns->trains = sg_queue_new();
    ns->dispatched_trains = 0;
    
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
    
    free(*s);
    *s = NULL;
}

void add_train(struct section *s) {
    struct train *nt = new_train();
    nt->origin = s->header;
    
    sg_queue_enqueue(s->trains, (void *)nt);
}

void dispatch_train(struct section *s, char dest) {
    struct train *t = (struct train *)sg_queue_dequeue(s->trains);
    
    if (t != NULL) {
        t->destination = dest;
        
        print_status(t); // Only temp to output. This will be managed by control center once the train passes tunnel
        
        s->dispatched_trains++;
        
        delete_train(&t);
    }
}