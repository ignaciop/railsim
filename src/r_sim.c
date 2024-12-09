#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "r_control.h"
#include "r_common.h"
#include "r_time.h"

int main(int argc, char *argv[]) {
    double prob_arrive = -1.0;
    int rounds = 0;
   
    process_input(argc, argv, &prob_arrive, &rounds);
    
    print_title();
    print_params(prob_arrive, rounds);
    
    /* Seed for the random numbers used across the entire program */
    srand(time(NULL));
    
    struct control *cc = new_control(prob_arrive, rounds);
    
    pthread_t st[CONTROL_NUM_SECTIONS] = {0};
    pthread_t ct = 0;
    
    struct r_time *start_sim = new_time();
    
    pthread_create(&ct, NULL, tunnel_control, (void *)cc);
    
    /*
     * cc->sections[0] represents Section AC
     * cc->sections[1] represents Section BC
     * cc->sections[2] represents Section DE
     * cc->sections[3] represents Section DF
     */
    for (int i = 0; i < CONTROL_NUM_SECTIONS; i++) {
        pthread_create(&st[i], NULL, add_train, (void *)(cc->sections[i]));
    }
    
    for (int i = 0; i < CONTROL_NUM_SECTIONS; i++) {
        pthread_join(st[i], NULL);
    }
    
    pthread_join(ct, NULL);
    
    struct r_time *end_sim = new_time();
    
    print_summary(prob_arrive, end_sim, start_sim, cc);
    
    delete_time(&start_sim);
    delete_time(&end_sim);
    
    delete_control(&cc);
    
    return EXIT_SUCCESS;
}