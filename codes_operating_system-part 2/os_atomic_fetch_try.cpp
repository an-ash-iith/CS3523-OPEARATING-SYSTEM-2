#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct __lock_t {
    int ticket;
    int turn;
} lock_t;

void lock_init(lock_t *lock) {
    lock->ticket = 0;
    lock->turn = 0;
}

void lock1(lock_t *lock) {
    int myturn = __sync_fetch_and_add(&lock->ticket, 1);
    while (lock->turn != myturn)
        ; // spin
}

void unlock(lock_t *lock) {
    __sync_fetch_and_add(&lock->turn, 1);
}

void *thread_func(void *arg) {
    lock_t *lock = (lock_t *)arg;
    lock1(lock);
    printf("Thread acquired the lock.\n");
    unlock(lock);
    printf("Thread released the lock.\n");
    return NULL;
}

int main() {
    lock_t lock;
    lock_init(&lock);

    pthread_t threads[10];
    for (int i = 0; i < 10; ++i) {
        pthread_create(&threads[i], NULL, thread_func, &lock);
    }

    for (int i = 0; i < 10; ++i) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
