#include "buffer.h"

//initialize buffers
void buffer_init(circbuf_t *b) {
        b->head  = 0;
        b->tail  = 0;
        b->count = 0;
        pthread_mutex_init(&b->mtx, NULL);
        pthread_cond_init(&b->not_empty, NULL);
        pthread_cond_init(&b->not_full, NULL);
}
//function which producers use
void buffer_put(circbuf_t *b, int write_id) {
        pthread_mutex_lock(&b->mtx);

        // wait while buffer is full
        while (b->count == BUFFER_SIZE) {
            printf("thread stuck because of full buffer\n");
            pthread_cond_wait(&b->not_full, &b->mtx);
        }

        // insert at tail
        b->buf[b->tail] = write_id;
        b->tail = (b->tail + 1) % BUFFER_SIZE;
        b->count++;

        // wake consumer waiting for not_empty
        pthread_cond_signal(&b->not_empty);

        pthread_mutex_unlock(&b->mtx);
}
//function which consumers use
int buffer_get(circbuf_t *b) {
        pthread_mutex_lock(&b->mtx);

        // wait while buffer is empty
        while (b->count == 0) {
            printf("thread stuck because of empty buffer\n");
            pthread_cond_wait(&b->not_empty, &b->mtx);
        }

        int write_id = b->buf[b->head];
        b->head = (b->head + 1) % BUFFER_SIZE;
        b->count--;

        // wake producer waiting for not_full
        pthread_cond_signal(&b->not_full);

        pthread_mutex_unlock(&b->mtx);

        return write_id;
}