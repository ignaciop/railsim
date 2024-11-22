#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "r_event.h"

struct tm_event *new_event(void) {
    struct tm_event *ne = (struct tm_event *)malloc(sizeof(struct tm_event));
    
    if (ne == NULL) {
        perror("Error allocating memory for new time event.\n");
        
        exit(EXIT_FAILURE);
    }
    
    time_t tevent = 0;               
    time(&tevent);
 
    struct tm *te = localtime(&tevent);
    
    ne->hour = te->tm_hour;
    ne->min = te->tm_min;
    ne->sec = te->tm_sec;
    
    return ne;
}

void delete_event(struct tm_event **e) {
    free(*e);
    *e = NULL;
}