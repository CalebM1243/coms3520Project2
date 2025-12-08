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