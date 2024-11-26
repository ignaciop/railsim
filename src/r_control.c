#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "r_control.h"
#include "r_section.h"
#include "r_train.h"
#include "r_symbols.h"

struct control *new_control(double prob_arrive, double prob_depart) {
    struct control *nc = (struct control *)malloc(sizeof(struct control));
    
    if (nc == NULL) {
        perror("Error allocating memory for new control center.\n");
        
        exit(EXIT_FAILURE);
    }
    
    nc->prob_arrive = prob_arrive;
    nc->prob_depart = prob_depart;
    
    nc->sections = (struct section **)malloc(sizeof(struct section *) * NUM_SECTIONS);
    
    if (nc->sections == NULL) {
        perror("Error allocating memory for new sections array.\n");
        
        exit(EXIT_FAILURE);
    }
    
    nc->sections[0] = new_section('A', prob_arrive);
    nc->sections[1] = new_section('B', prob_arrive);
    nc->sections[2] = new_section('E', prob_arrive);
    nc->sections[3] = new_section('F', prob_arrive);
    
    nc->l1_passed_trains = 0;
    nc->l2_passed_trains = 0;
    nc->l3_passed_trains = 0;
    nc->l4_passed_trains = 0;
    nc->overloads = 0;
    nc->breakdowns = 0;
    
    nc->mtx = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(nc->mtx, NULL);
    
    nc->cv = (pthread_cond_t *)malloc(sizeof(pthread_cond_t));
    pthread_cond_init(nc->cv, NULL);
    
    return nc;
}

void delete_control(struct control **c) {
    for (int i = 0; i < NUM_SECTIONS; i++) {
        delete_section(&((*c)->sections[i]));
    }
    
    free((*c)->sections);
    (*c)->sections = NULL;
    
    pthread_cond_destroy((*c)->cv);
    pthread_mutex_destroy((*c)->mtx);
    
    free(*c);
    *c = NULL;
}

void *tunnel_control(void *arg) {
    struct control *c = (struct control *)arg;
    
    struct section *s_AC = c->sections[0];
    struct section *s_BC = c->sections[1];
    struct section *s_DE = c->sections[2];
    struct section *s_DF = c->sections[3];
    
    struct section *s_priority = NULL;
    struct section *s_temp1 = NULL;
    struct section *s_temp2 = NULL;
    
    while (1) {
        if (sg_queue_size(s_AC->trains) < sg_queue_size(s_BC->trains)) {
            s_temp1 = s_BC;
        } else {
            s_temp1 = s_AC;
        }
        
        if (sg_queue_size(s_DE->trains) < sg_queue_size(s_DF->trains)) {
            s_temp2 = s_DF;
        } else {
            s_temp2 = s_DE;
        }
        
        if (sg_queue_size(s_temp1->trains) < sg_queue_size(s_temp2->trains)) {
            s_priority = s_temp2;
        } else {
            s_priority = s_temp1;
        }
        
        pthread_mutex_lock(s_priority->mtx);

        while (sg_queue_size(s_priority->trains) == 0) {
            pthread_cond_wait(s_priority->cv, s_priority->mtx);
        }

        struct train *t = (struct train *)sg_queue_dequeue(s_priority->trains);
        
        set_train_destination(s_priority->header, t);
            
        print_status(PASSING_SIGN, t);
        
        printf("Train %d dispatched..., %d left\n", t->id, sg_queue_size(s_priority->trains));
        
        delete_train(&t);

        pthread_mutex_unlock(s_priority->mtx);
        
        sleep(3);
    }
    
    return NULL;
}

void print_status(char *sign, struct train *t) {
    /* Buffer variables to hold correct constant values defined in r_symbols.h */
    /* Buffer length is constant max size + 1 */
    char t_length_s[3] = "";
    char t_length_p[3] = "";
    char arrow_icon[11] = "";
    char line_sign[18] = "";
    char header1 = ' ';
    char header2 = ' ';

    strncpy(t_length_p, (t->length == 100) ? "[" : "[[", 2);
    strncpy(t_length_s, (t->length == 100) ? "]" : "]]", 2);
    
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
}

void set_train_destination(char header, struct train *t) {
    double p = (double)rand() / RAND_MAX;
            
    if (header == 'A' || header == 'B') {
        t->destination = (p < 0.5) ? 'E' : 'F';
    } else if (header == 'E' || header == 'F') {
        t->destination = (p < 0.5) ? 'A' : 'B';
    }
} 

/*
void print_summary(struct control *c) {

}
*/