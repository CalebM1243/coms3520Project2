#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "buffer.h"

#define NUM_ITEMS 20

static circbuf_t test_buf;

void *producer_thread(void *arg) {
    for (int i = 0; i < NUM_ITEMS; i++) {
        printf("producer putting %d\n", i);
        buffer_put(&test_buf, i);
        usleep(100000);
    }
    return NULL;
}

void *consumer_thread(void *arg) {
    for (int i = 0; i < NUM_ITEMS; i++) {
        int v = buffer_get(&test_buf);
        printf("consumer got %d\n", v);
        usleep(150000);
    }
    return NULL;
}

int main(void) {
    pthread_t prod, cons;

    buffer_init(&test_buf);

    pthread_create(&prod, NULL, producer_thread, NULL);
    pthread_create(&cons, NULL, consumer_thread, NULL);

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    printf("Done.\n");
    return 0;
}
