#ifndef BUFFER_H
#define BUFFER_H

#include <pthread.h>
#include "journal.h"   

typedef struct {
    int buf[BUFFER_SIZE];
    int head;
    int tail;
    int count;

    const char *name; 

    pthread_mutex_t mtx;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
} circbuf_t;

void buffer_init(circbuf_t *b, const char *name);
void buffer_put(circbuf_t *b, int value);
int  buffer_get(circbuf_t *b);

#endif