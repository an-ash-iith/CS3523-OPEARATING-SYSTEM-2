
#include <iostream>
#include <thread>
#include <atomic>
#include <bits/stdc++.h>

typedef struct __lock_t {
int ticket;
int turn;
} lock_t;

void lock_init(lock_t *lock) {
lock->ticket = 0;
lock->turn
= 0;
}

void lock(lock_t *lock) {
int myturn = FetchAndAdd(&lock->ticket);
while (lock->turn != myturn)
; // spin
}

void unlock(lock_t *lock) {
FetchAndAdd(&lock->turn);
}