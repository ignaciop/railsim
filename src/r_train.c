#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "r_train.h"
#include "r_time.h"

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

void print_status(struct train *t) {
    //cases for origin dest
    switch (t->origin) {
        case 'A':
            switch (t->destination) {
                case 'E':
                    printf("\033[1;97m| \U0001F535 Line 1 (A \U0001F872 E) | \U0001F687 [T%03d] | \U0001F557 TA: %02d:%02d:%02d | \U0001F557 TD: %02d:%02d:%02d |\033[0m\n", t->id, t->arrival_time->hour, t->arrival_time->min, t->arrival_time->sec, t->departure_time->hour, t->departure_time->min, t->departure_time->sec);
                break;
                
                case 'F':
                    printf("\033[1;97m| \U0001F7E3 Line 2 (A \U0001F872 F) | \U0001F687 [T%03d] | \U0001F557 TA: %02d:%02d:%02d | \U0001F557 TD: %02d:%02d:%02d |\033[0m\n", t->id, t->arrival_time->hour, t->arrival_time->min, t->arrival_time->sec, t->departure_time->hour, t->departure_time->min, t->departure_time->sec);
                break;
            }
            
            break;        
        case 'B':
            switch (t->destination) {
                case 'E':
                    printf("\033[1;97m| \U0001F534 Line 3 (B \U0001F872 E) | \U0001F687 [T%03d] | \U0001F557 TA: %02d:%02d:%02d | \U0001F557 TD: %02d:%02d:%02d |\033[0m\n", t->id, t->arrival_time->hour, t->arrival_time->min, t->arrival_time->sec, t->departure_time->hour, t->departure_time->min, t->departure_time->sec);
                break;
                
                case 'F':
                    printf("\033[1;97m| \U0001F7E0 Line 4 (B \U0001F872 F) | \U0001F687 [T%03d] | \U0001F557 TA: %02d:%02d:%02d | \U0001F557 TD: %02d:%02d:%02d |\033[0m\n", t->id, t->arrival_time->hour, t->arrival_time->min, t->arrival_time->sec, t->departure_time->hour, t->departure_time->min, t->departure_time->sec);
                break;
            }
            
            break;   
        
        case 'E':
            switch (t->destination) {
                case 'A':
                    printf("\033[1;97m| \U0001F535 Line 1 (A \U0001F870 E) | \U0001F687 [T%03d] | \U0001F557 TA: %02d:%02d:%02d | \U0001F557 TD: %02d:%02d:%02d |\033[0m\n", t->id, t->arrival_time->hour, t->arrival_time->min, t->arrival_time->sec, t->departure_time->hour, t->departure_time->min, t->departure_time->sec);
                break;
                
                case 'B':
                    printf("\033[1;97m| \U0001F534 Line 3 (B \U0001F870 E) | \U0001F687 [T%03d] | \U0001F557 TA: %02d:%02d:%02d | \U0001F557 TD: %02d:%02d:%02d |\033[0m\n", t->id, t->arrival_time->hour, t->arrival_time->min, t->arrival_time->sec, t->departure_time->hour, t->departure_time->min, t->departure_time->sec);
                break;
            }
            
            break;    
        case 'F':
            switch (t->destination) {
                case 'A':
                    printf("\033[1;97m| \U0001F7E3 Line 2 (A \U0001F870 F) | \U0001F687 [T%03d] | \U0001F557 TA: %02d:%02d:%02d | \U0001F557 TD: %02d:%02d:%02d |\033[0m\n", t->id, t->arrival_time->hour, t->arrival_time->min, t->arrival_time->sec, t->departure_time->hour, t->departure_time->min, t->departure_time->sec);
                break;
                
                case 'B':
                    printf("\033[1;97m| \U0001F7E0 Line 4 (B \U0001F870 F) | \U0001F687 [T%03d] | \U0001F557 TA: %02d:%02d:%02d | \U0001F557 TD: %02d:%02d:%02d |\033[0m\n", t->id, t->arrival_time->hour, t->arrival_time->min, t->arrival_time->sec, t->departure_time->hour, t->departure_time->min, t->departure_time->sec);
                break;
            }
            
            break;
    }
}