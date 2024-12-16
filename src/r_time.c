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
    if (t != NULL) {
        free(*t);
        *t = NULL;
    }
}

struct r_time *delta_time(const struct r_time *ft, const struct r_time *st) {
    struct r_time *dt = (struct r_time *)malloc(sizeof(struct r_time));
    
    if (dt == NULL) {
        perror("Error allocating memory for new time event.\n");
        
        exit(EXIT_FAILURE);
    }
    
    if (ft != NULL && st != NULL) {
        int ft_hour = ft->hour;
        int ft_min = ft->min;
        int ft_sec = ft->sec;
        int st_hour = st->hour;
        int st_min = st->min;
        int st_sec = st->sec;
                                                
        dt->sec = ft_sec - st_sec;
        
        /* Adjust seconds if negative */
        if (dt->sec < 0) {
            dt->sec += 60;
            ft_min -= 1;
        }

        dt->min = ft_min - st_min;
        
        /* Adjust minutes if negative */
        if (dt->min < 0) {
            dt->min += 60;
            ft_hour -= 1;
        }

        // Subtract hours, adjust if negative
        dt->hour = ft_hour - st_hour;
        
        /* Adjust hours if negative by adding 24 hours (24-hour format) */
        if (dt->hour < 0) {
            dt->hour += 24;
        }
    } else {
        dt->hour = dt->min = dt->sec = 0; 
    }
    
    return dt;
}