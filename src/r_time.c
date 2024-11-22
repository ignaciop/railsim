#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "r_time.h"

struct r_time *new_time(void) {
    struct r_time *nt = (struct r_time *)malloc(sizeof(struct r_time));
    
    if (nt == NULL) {
        perror("Error allocating memory for new time event.\n");
        
        exit(EXIT_FAILURE);
    }
    
    time_t tevent = 0;               
    time(&tevent);
 
    struct tm *te = localtime(&tevent);
    
    nt->hour = te->tm_hour;
    nt->min = te->tm_min;
    nt->sec = te->tm_sec;
    
    return nt;
}

void delete_time(struct r_time **t) {
    free(*t);
    *t = NULL;
}