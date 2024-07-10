// #include <semaphore.h>
// sem_t s;
// sem_init(&s, 0, 1);

// //wait and post definition of semaphore 
// int sem_wait(sem_t *s) {
// decrement the value of semaphore s by one
// wait if value of semaphore s is negative
// }

// int sem_post(sem_t *s) {
// increment the value of semaphore s by one
// if there are one or more threads waiting, wake one
// }


//binary semaphore 

// sem_t m;
// sem_init(&m, 0, X); // initialize semaphore to X; what should X be?

// sem_wait(&m);
// // critical section here
// sem_post(&m);



//producer consumer using semaphore -- adding mutual exclusion


sem_t empty;
sem_t full;
sem_t mutex;

void *producer(void *arg) {
int i;
for (i = 0; i < loops; i++) {
sem_wait(&empty);
sem_wait(&mutex);
put(i);
sem_post(&mutex);
sem_post(&full);
}
}
// line p1
// line p1.5 (MOVED MUTEX HERE...)
// line p2
// line p2.5 (... AND HERE)
// line p3

void *consumer(void *arg) {
int i;
for (i = 0; i < loops; i++) {
sem_wait(&full);
sem_wait(&mutex);
int tmp = get();
sem_post(&mutex);
sem_post(&empty);
printf("%d\n", tmp);
}
}
// line c1
// line c1.5 (MOVED MUTEX HERE...)
// line c2
// line c2.5 (... AND HERE)
// line c3

int main(int argc, char *argv[]) {
// ...
sem_init(&empty, 0, MAX); // MAX buffers are empty to begin with...
sem_init(&full, 0, 0);
// ... and 0 are full
sem_init(&mutex, 0, 1);
// mutex=1 because it is a lock
// ...
}