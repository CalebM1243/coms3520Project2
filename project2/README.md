# README — Project 2: Metadata Journaling


Caleb Moe


## 1. Overview
This project was essentially asking us to create a one consumer one producer buffer solution while incorporating some aspects of file management that OS's do. In terms of how I did this, I created 2 new files, 3 new threads, and 3 semaphores. I mainly though of the threads as like "stages" so each thread had to call certain functions and the requests flowed from one stage to the next, so if you see "stage" in my code that is why. 

New Files:

- buffer.c - contains code for thread safe buffer
- buffer.h - contains function and struct declarations

Threads:
- journal_metadata_thread - prepares all the meta data in the journal and writes the data
- journal_commit_thread - commits all the meta data into the journal by issuing the journal transaction end
- checkpoint_thread - actually writes the meta data


---

## 2.1 Design Description

So in terms of the threads, each thread had a semaphore which counted up (in a for loop) until the relevant amount of signals were posted from the "completion" functions, for "stage 1"/ journal_metadata_thread it was 4, for "stage 2"/ journal_commit_thread it was 1, and for "stage 3"/checkpoint_thread it was 2. 


Overall, since the buffer is thread safe and the semaphores ensure that the threads progress only when they are supposed to, that's how I know that there are no race conditions. 
---

### 2.2 Buffer Design
Buffer Functions:
- Buffer_init - initializes the Mutex, the conditional variables, the buffer name, and the size,tail, and head variables.
- Buffer_get - gets the write_id of the write_id at the head and then decrements the count variable of the buffer, will wait until not empty if there are no elements
- Buffer_put - inserts a new write_id and increments the count variable, will wait until not full if at BUFFER_SIZE (found in journal.h)

The whole buffer data structure is designed to be thread safe since we lock and unlock the mutex as well as use the conditional variables to wake up or put the threads to sleep until they are needed. There are no spin locks. Also the design is circular in nature so the buffer is not dynamic. 

## Testing

I have the reccomended test built in, except the "buffer full" output triggers for all buffers, and print's each buffer's names as well.



requesting test write 0
requesting test write 1
requesting test write 2
requesting test write 3
requesting test write 4
requesting test write 5
requesting test write 6
requesting test write 7
requesting test write 8
requesting test write 9
thread stuck because of full request_buf buffer
issue write data 0
issue journal txb 0
issue journal bitmap 0
issue journal inode 0
issue write data 1
issue journal txb 1
issue journal txe 0
issue journal bitmap 1
issue journal inode 1
issue write data 2
issue journal txb 2
issue journal bitmap 2
issue journal inode 2
issue write data 3
issue journal txb 3
issue journal bitmap 3
issue journal inode 3
issue write data 4
issue journal txb 4
issue journal bitmap 4
issue journal inode 4
issue write data 5
issue journal txb 5
issue journal bitmap 5
issue journal inode 5
issue write data 6
issue journal txb 6
issue journal bitmap 6
issue journal inode 6
issue write data 7
issue journal txb 7
issue journal bitmap 7
issue journal inode 7
requesting test write 10
requesting test write 11
requesting test write 12
issue write data 8
issue journal txb 8
issue journal bitmap 8
issue journal inode 8
issue write data 9
issue journal txb 9
issue journal bitmap 9
issue journal inode 9
thread stuck because of full journal_done_buf buffer
requesting test write 13
requesting test write 14
requesting test write 15
Wait over 
issue journal txe 1
issue journal txe 2
issue write bitmap 0
issue write inode 0
test write complete 0
issue write bitmap 1
issue write inode 1
test write complete 1
issue write bitmap 2
issue write inode 2
test write complete 2
issue write data 10
issue journal txb 10
issue journal bitmap 10
issue journal inode 10
issue write data 11
issue journal txb 11
issue journal bitmap 11
issue journal inode 11
issue write data 12
issue journal txb 12
issue journal bitmap 12
issue journal inode 12
thread stuck because of full journal_done_buf buffer
issue journal txe 3
issue journal txe 4
issue journal txe 5
issue journal txe 6
issue journal txe 7
issue journal txe 8
issue write data 13
issue journal txb 13
issue journal bitmap 13
issue journal inode 13
issue write data 14
issue journal txb 14
issue write bitmap 3
issue write inode 3
test write complete 3
issue write bitmap 4
issue write inode 4
test write complete 4
issue write bitmap 5
issue write inode 5
test write complete 5
issue write bitmap 6
issue write inode 6
test write complete 6
issue write bitmap 7
issue write inode 7
test write complete 7
issue write bitmap 8
issue write inode 8
test write complete 8
issue journal txe 9
issue journal bitmap 14
issue journal inode 14
issue write data 15
issue journal txb 15
issue journal bitmap 15
issue journal inode 15
issue write bitmap 9
issue write inode 9
test write complete 9
issue journal txe 10
issue journal txe 11
issue journal txe 12
issue journal txe 13
issue journal txe 14
issue journal txe 15
issue write bitmap 10
issue write inode 10
test write complete 10
issue write bitmap 11
issue write inode 11
test write complete 11
issue write bitmap 12
issue write inode 12
test write complete 12
issue write bitmap 13
issue write inode 13
test write complete 13
issue write bitmap 14
issue write inode 14
test write complete 14
issue write bitmap 15
issue write inode 15
test write complete 15