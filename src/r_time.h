#ifndef R_TIME_H
#define R_TIME_H

struct r_time {
    int hour;
    int min;
    int sec;
};

struct r_time *new_time(void);
void delete_time(struct r_time **t);
struct r_time *delta_time(const struct r_time *ft, const struct r_time *st);

#endif