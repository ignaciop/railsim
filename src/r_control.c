#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "r_control.h"
#include "r_section.h"
#include "r_train.h"
#include "r_symbols.h"


void *tunnel_control(void *arg) {
    struct section *s = (struct section *)arg;
    
    while (1) {
        pthread_mutex_lock(s->mtx);

        while (sg_queue_size(s->trains) == 0) {
            pthread_cond_wait(s->cv, s->mtx);
        }

        struct train *t = (struct train *)sg_queue_dequeue(s->trains);
        
        double p = (double)rand() / RAND_MAX;
            
        if (p < 0.5) {
            t->destination = 'E';
        } else {
            t->destination = 'F';
        }
            
        print_status(t); // Only temp to output. This will be managed by control center once the train passes tunnel
            
        s->dispatched_trains++;
        
        printf("Train %d dispatched..., %d left\n", t->id, sg_queue_size(s->trains));
        
        delete_train(&t);

        pthread_mutex_unlock(s->mtx);
        
        sleep(3);
    }
}

void *add_train(void *arg) {
    struct section *s = (struct section *)arg;
    
    while (1) {
        pthread_mutex_lock(s->mtx);

        double p = (double)rand() / RAND_MAX;
            
        if (p < s->prob_arrive) {
            struct train *nt = new_train();
            nt->origin = s->header;

            sg_queue_enqueue(s->trains, (void *)nt);
            
            printf("Train %d in queue...\n", nt->id);
            
            pthread_cond_signal(s->cv);
        }
        
        pthread_mutex_unlock(s->mtx);
        
        sleep(1);
    }
}

/*

void print_line_info(struct control *c) {

}

void print_summary(struct control *c) {

}
*/