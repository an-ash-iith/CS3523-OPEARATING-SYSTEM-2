

//    Write a solution to the benchmark synchronization problem of producers and consumers
//     as described below. give c++ code 
 
//     Shared memory: 1 bounded buffer data structure implemented as shared memory
 
//     Startup process:  creates bounded buffer
//                                       creates needed semaphores
//                                       and exits
 
//     Producer process: do 5 times: produce, sleep for 1 second enddo
 
//     Consumer process: do 5 times: consume, sleep for 1 second enddo

//                 any number of producer and consumer processes can be started after the startup process.

//      Cleanup process:  deletes semaphores
                                        //  deletes shared memory 




#include <iostream>
#include <unistd.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <fcntl.h>

const int BUFFER_SIZE = 10; // Size of the bounded buffer

struct BoundedBuffer {
    int buffer[BUFFER_SIZE];
    int in;
    int out;
    sem_t mutex;
    sem_t empty;
    sem_t full;
};

void producer(BoundedBuffer* buf) {
    for (int i = 0; i < 5; ++i) {
        // Produce item
        int item = i;
        
        sem_wait(&buf->empty);
        sem_wait(&buf->mutex);
        
        // Add item to buffer
        buf->buffer[buf->in] = item;
        buf->in = (buf->in + 1) % BUFFER_SIZE;
        
        sem_post(&buf->mutex);
        sem_post(&buf->full);
        
        std::cout << "Produced: " << item << std::endl;
        
        sleep(1); // Sleep for 1 second
    }
}

void consumer(BoundedBuffer* buf) {
    for (int i = 0; i < 5; ++i) {
        int item;
        
        sem_wait(&buf->full);
        sem_wait(&buf->mutex);
        
        // Consume item from buffer
        item = buf->buffer[buf->out];
        buf->out = (buf->out + 1) % BUFFER_SIZE;
        
        sem_post(&buf->mutex);
        sem_post(&buf->empty);
        
        std::cout << "Consumed: " << item << std::endl;
        
        sleep(1); // Sleep for 1 second
    }
}

int main() {
    // Create shared memory
    int shm_fd = shm_open("/bounded_buffer", O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(BoundedBuffer));
    BoundedBuffer* buf = (BoundedBuffer*)mmap(0, sizeof(BoundedBuffer), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    // Initialize bounded buffer
    buf->in = 0;
    buf->out = 0;
    sem_init(&buf->mutex, 1, 1);
    sem_init(&buf->empty, 1, BUFFER_SIZE);
    sem_init(&buf->full, 1, 0);

    // Create producer and consumer processes
    pid_t pid = fork();
    if (pid == 0) {
        producer(buf);
    } else {
        pid_t pid2 = fork();
        if (pid2 == 0) {
            consumer(buf);
        } else {
            // Wait for child processes to finish
            waitpid(pid, NULL, 0);
            waitpid(pid2, NULL, 0);
            
            // Cleanup shared memory and semaphores
            sem_destroy(&buf->mutex);
            sem_destroy(&buf->empty);
            sem_destroy(&buf->full);
            shm_unlink("/bounded_buffer");
        }
    }

    return 0;
}
