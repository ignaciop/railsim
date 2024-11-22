#ifndef EVENT_H
#define EVENT_H

struct tm_event {
    int hour;
    int min;
    int sec;
};

struct tm_event *new_event(void);
void delete_event(struct tm_event **e);

#endif