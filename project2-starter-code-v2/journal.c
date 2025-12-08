#include <stdio.h>
#include "journal.h"
#include "buffer.h"
#include <pthread.h>
#include <semaphore.h>


/*
Author: Caleb Moe
Todo:
Buffers
Buffer Functions
Threads
Thread Functions
*/

static circbuf_t request_buf;       // buffer 1
static circbuf_t journal_done_buf;  // buffer 2
static circbuf_t commit_done_buf;   // buffer 3

static pthread_t journal_metadata_thread_t; 
static pthread_t journal_commit_thread_t;
static pthread_t checkpoint_thread_t;

static sem_t stage1_sem;
static sem_t stage2_sem;
static sem_t stage3_sem;

//stage 1 thread
static void *journal_metadata_thread(void *arg) {
        while (1) {
                int write_id = buffer_get(&request_buf);  // buffer_get blocks on empty

                issue_write_data(write_id);
                issue_journal_txb(write_id);
                issue_journal_bitmap(write_id);
                issue_journal_inode(write_id);

                // wait until 4 completions have happened
                for (int i = 0; i < 4; i++) {
                        sem_wait(&stage1_sem);
                }
                buffer_put(&journal_done_buf, write_id); // buffer_put blocks on full
        }
        return NULL;
}
//stage 2 thread
static void *journal_commit_thread(void *arg) {
        while (1) {
                int write_id = buffer_get(&journal_done_buf);  // buffer_get blocks on empty

                issue_journal_txe(write_id);

                // wait until completion
                sem_wait(&stage2_sem);
                buffer_put(&commit_done_buf, write_id); // buffer_put blocks on full
        }
        return NULL;
}
//stage 3 thread
static void *checkpoint_thread(void *arg) {
        while (1) {
                int write_id = buffer_get(&commit_done_buf);  // buffer_get blocks on empty

                issue_write_bitmap(write_id);
                issue_write_inode(write_id);

                // wait until 2 completions completed
                for (int i = 0; i < 2; i++) {
                        sem_wait(&stage3_sem);
                }
                write_complete(write_id); //stage 3 done
        }
        return NULL;
}
/* This function can be used to initialize the buffers and threads.
 */
void init_journal() {
        //buffers
        buffer_init(&request_buf, "request_buf"); //stage 1
        buffer_init(&journal_done_buf, "journal_done_buf"); //stage 2
        buffer_init(&commit_done_buf, "commit_done_buf"); //stage 3
        //semaphores
        sem_init(&stage1_sem, 0, 0);
        sem_init(&stage2_sem, 0, 0);
        sem_init(&stage3_sem, 0, 0);

        //threads
        pthread_create(&journal_metadata_thread_t,   NULL, journal_metadata_thread, NULL);
        pthread_create(&journal_commit_thread_t,     NULL, journal_commit_thread,   NULL);
        pthread_create(&checkpoint_thread_t, NULL, checkpoint_thread,       NULL);
}

//new request write simply adds request to request buffer
void request_write(int write_id){
        buffer_put(&request_buf, write_id);
}

void journal_txb_complete(int write_id) {
        sem_post(&stage1_sem);
}

void journal_bitmap_complete(int write_id) {
        sem_post(&stage1_sem);
}

void journal_inode_complete(int write_id) {
        sem_post(&stage1_sem);
}

void write_data_complete(int write_id) {
        sem_post(&stage1_sem);
}

void journal_txe_complete(int write_id) {
        sem_post(&stage2_sem);
}

void write_bitmap_complete(int write_id) {
        sem_post(&stage3_sem);

}

void write_inode_complete(int write_id) {
        sem_post(&stage3_sem);
}

