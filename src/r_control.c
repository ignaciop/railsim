#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "r_control.h"
#include "r_section.h"
#include "r_train.h"
#include "r_symbols.h"
#include "pthread_sleep.h"

static void print_status(const char *sign, const struct train *t, const int opt_num);
static char set_destination(const char tn_origin);
static size_t queued_trains(const struct control *c);
static struct section *get_priority_section(const struct control *c);

pthread_mutex_t slowdown_mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t slowdown_cv = PTHREAD_COND_INITIALIZER;

bool slowdown_flag = false;

struct control *new_control(const double prob_arrive) {
    struct control *nc = (struct control *)malloc(sizeof(struct control));
    
    if (nc == NULL) {
        perror("Error allocating memory for new control center.\n");
        
        exit(EXIT_FAILURE);
    }
    
    nc->sections = (struct section **)malloc(sizeof(struct section *) * CONTROL_NUM_SECTIONS);
    
    if (nc->sections == NULL) {
        perror("Error allocating memory for new sections array.\n");
        
        exit(EXIT_FAILURE);
    }
    
    nc->sections[0] = new_section('A', prob_arrive);
    nc->sections[1] = new_section('B', 1 - prob_arrive);
    nc->sections[2] = new_section('E', prob_arrive);
    nc->sections[3] = new_section('F', prob_arrive);
    
    nc->l1_passed_trains = 0;
    nc->l2_passed_trains = 0;
    nc->l3_passed_trains = 0;
    nc->l4_passed_trains = 0;
    nc->overloads = 0;
    nc->breakdowns = 0;
    
    nc->mtx = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
    
    if (nc->mtx == NULL) {
        perror("Error allocating memory for new control mutex.\n");
        
        exit(EXIT_FAILURE);
    }
    
    pthread_mutex_init(nc->mtx, NULL);
    
    return nc;
}

void delete_control(struct control **c) {
    for (int i = 0; i < CONTROL_NUM_SECTIONS; i++) {
        delete_section(&((*c)->sections[i]));
    }
    
    free((*c)->sections);
    (*c)->sections = NULL;
    
    pthread_mutex_destroy((*c)->mtx);
    (*c)->mtx = NULL;
    
    free(*c);
    *c = NULL;
}

void *tunnel_control(void *arg) {
    struct control *c = (struct control *)arg;
    
    /* Counter for show overload sign only once */
    int overload_counter = 0;
    
    while (1) {
        pthread_mutex_lock(c->mtx);
        
        struct section *s_priority = get_priority_section(c);
        
        int qt = queued_trains(c);
      
        if (qt > CONTROL_QT_THRESHOLD) {
            pthread_mutex_lock(&slowdown_mtx);
            
            /* Flag to all sections to stop adding new trains to their queues */
            slowdown_flag = true;
            
            /*
             * Broadcast a signal to all sections to stop adding trains to their
             * respective queues
             */
            pthread_cond_broadcast(&slowdown_cv);
            
            pthread_mutex_unlock(&slowdown_mtx);
            
            /* Pause one second to sync broadcast signal to all threads */
            pthread_sleep(1);
            
            overload_counter++;
        } else if (qt == 0) {
            pthread_mutex_lock(&slowdown_mtx);
            
            /* Flag to all sections to continue adding new trains to their queues */
            slowdown_flag = false;
            
            /*
             * Broadcast a signal to all sections to continue adding trains to their
             * respective queues
             */
            pthread_cond_broadcast(&slowdown_cv);
            
            pthread_mutex_unlock(&slowdown_mtx);
            
            /* Reset overload sign counter */
            overload_counter = 0;
            
            print_status("Waiting For New Trains...", NULL, 0);

            /* Wait 1 second for new trains arrive to sections */
            pthread_sleep(CONTROL_NEW_TRAINS_TIME);
        }
        
        /*
         * Broadcasting slowdown_flag to all threads has a delay
         * This ensures showing the correct amount of queued trains only once
         */
        if (overload_counter == 1) {
            qt = queued_trains(c);
            
            print_status(OVERLOAD_SIGN, NULL, qt);
        }
        
        pthread_mutex_lock(s_priority->mtx);
        
        /* Pass trains through tunnel and set their destinations */
        if (sg_queue_size(s_priority->trains) != 0) {
            struct train *t = (struct train *)sg_queue_dequeue(s_priority->trains);
            
            t->destination = set_destination(t->origin);
            t->departure_time = new_time();

            /* Take 1 second to arrive from section to tunnel */
            pthread_sleep(SECTION_TRAVEL_TIME);
            
            print_status(PASSING_SIGN, t, 0);
            
            /* Train length determines time in tunnel (train speed is 100 m/s)
             * 100 m train takes 1 second, 200 m train takes 2 seconds
             */
            (t->length == TRAIN_LENGTH_1) ? pthread_sleep(TRAIN_LENGTH_1_TRAVEL_TIME) : pthread_sleep(TRAIN_LENGTH_2_TRAVEL_TIME);
            
            double p = (double)rand() / RAND_MAX;
            
            if (p < TRAIN_PROB_BREAKDOWN) {
                print_status(BREAKDOWN_SIGN, t, 0);
                
                /* Takes 4 additional second for the train to pass if it breaks down */
                pthread_sleep(TRAIN_BREAKDOWN_TIME);
            }
            
            /* Take 1 second to pass final section */
            pthread_sleep(SECTION_TRAVEL_TIME);
            
            delete_train(&t);
        }
        
        pthread_mutex_unlock(s_priority->mtx);
        
        pthread_mutex_unlock(c->mtx);
    }

    pthread_exit(NULL);
}

void *add_train(void *arg) {
    struct section *s = (struct section *)arg;
  
    while (1) {
        pthread_mutex_lock(&slowdown_mtx);
       
        /* Check global flag to continue or stop adding new trains to section queue */
        while (slowdown_flag) {
            pthread_cond_wait(&slowdown_cv, &slowdown_mtx);
        }
        
        pthread_mutex_unlock(&slowdown_mtx);
            
        /* At each second, a train arrives at section with probability "prob_arrive" */
        pthread_sleep(SECTION_ARRIVE_TIME);
        
        pthread_mutex_lock(s->mtx);
        
        double p = (double)rand() / RAND_MAX;
        
        if (p < s->prob_arrive) {
            struct train *nt = new_train();
            nt->origin = s->header;

            sg_queue_enqueue(s->trains, (void *)nt);
        }
        
        pthread_mutex_unlock(s->mtx);
    }
    
    pthread_exit(NULL);
}

static void print_status(const char *sign, const struct train *t, const int opt_num) {
    if (strcmp(sign, OVERLOAD_SIGN) == 0) {
        printf("%s %s | %d %s %s |%s\n\n", sign, BOLD_FACE, opt_num, (opt_num == 1) ? "Train" : "Trains", "Waiting Passage", RESET_COLOR);
        printf("%s%s Slowing Down All Trains... %s%s\n\n", BOLD_FACE, TRAFFIC_LIGHT_ICON, TRAFFIC_LIGHT_ICON, RESET_COLOR);
    } else if (strcmp(sign, "Waiting For New Trains...") == 0) {
        printf("\n%s%s %s %s%s\n\n", BOLD_FACE, LOAD_ICON, sign, LOAD_ICON, RESET_COLOR);
    } else {
        /* Buffer variables to hold correct constant values defined in r_symbols.h */
        /* Buffer length is constant max size + 1 */
        char t_length_s[3] = "";
        char t_length_p[3] = "";
        char arrow_icon[11] = "";
        char line_sign[18] = "";
        char header1 = ' ';
        char header2 = ' ';
        
        strncpy(t_length_p, (t->length == TRAIN_LENGTH_1) ? "[" : "[[", 2);
        strncpy(t_length_s, (t->length == TRAIN_LENGTH_1) ? "]" : "]]", 2);
        
        switch (t->origin) {
            case 'A':
            case 'B':
                header1 = t->origin;
                header2 = t->destination;
                
                strncpy(arrow_icon, RIGHT_ARROW_ICON, 10);
                
                if (t->destination == 'E') {
                    strncpy(line_sign, (t->origin == 'A') ? LINE1_SIGN : LINE3_SIGN, 17);
                } else if (t->destination == 'F') {
                    strncpy(line_sign, (t->origin == 'A') ? LINE2_SIGN : LINE4_SIGN, 17);
                }
                
            break;
            
            case 'E':
            case 'F':
                header1 = t->destination;
                header2 = t->origin;
                
                strncpy(arrow_icon, LEFT_ARROW_ICON, 10);
                
                if (t->destination == 'A') {
                    strncpy(line_sign, (t->origin == 'E') ? LINE1_SIGN : LINE2_SIGN, 17);
                } else if (t->destination == 'B') {
                    strncpy(line_sign, (t->origin == 'E') ? LINE3_SIGN : LINE4_SIGN, 17);
                }
                
            break;
        }
        
        printf("%s %s | %s (%c %s %c) | %s %sT%03d%s | %s TA: %02d:%02d:%02d | %s TD: %02d:%02d:%02d | %s\n", sign, BOLD_FACE, line_sign, header1, arrow_icon, header2, TRAIN_ICON, t_length_p, t->id , t_length_s, CLOCK_ICON, t->arrival_time->hour, t->arrival_time->min, t->arrival_time->sec, CLOCK_ICON, t->departure_time->hour, t->departure_time->min, t->departure_time->sec, RESET_COLOR);
        
        if (strcmp(sign, BREAKDOWN_SIGN) == 0) {
            printf("\n%s%s%s%s Repair in Progress... %s%s%s%s\n\n", BOLD_FACE, BARRIER_ICON, " ", REPAIR_ICON, REPAIR_ICON, " ", BARRIER_ICON, RESET_COLOR);
        }
    }
}

static char set_destination(const char tn_origin) {
    char td = ' ';
    
    double p = (double)rand() / RAND_MAX;
            
    if (tn_origin == 'A' || tn_origin == 'B') {
        td = (p < CONTROL_PROB_BRANCH) ? 'E' : 'F';
    } else if (tn_origin == 'E' || tn_origin == 'F') {
        td = (p < CONTROL_PROB_BRANCH) ? 'A' : 'B';
    }
    
    return td;
}

static size_t queued_trains(const struct control *c) {
    size_t total = 0;
    
    for (int i = 0; i < CONTROL_NUM_SECTIONS; i++) {
        total += sg_queue_size(c->sections[i]->trains);
    }

    return total;
}


static struct section *get_priority_section(const struct control *c) {
    struct section *s_priority = c->sections[0];
    
    for (int i = 1; i < CONTROL_NUM_SECTIONS; i++) {
        if (sg_queue_size(c->sections[i]->trains) > sg_queue_size(s_priority->trains)) {
            s_priority = c->sections[i];
        }
    }
    
    return s_priority;
}

/*
void print_summary(struct control *c) {

}
*/