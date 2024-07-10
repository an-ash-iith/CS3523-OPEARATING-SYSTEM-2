int compareAndSwap(int *val,int expected,int current)
{
    int store = *val;
    
    if(store == expected)
    {
        *val = current;
    }

    return store;
}


typedef struct __lock_t {
int flag;
} lock_t;

void init(lock_t *lock) {
// 0 indicates that lock is available, 1 that it is held
lock->flag = 0;
}

void lock(lock_t *lock) {
while (compareAndSwap(&lock->flag,0, 1) == 1)
; // spin-wait (do nothing)
}
void unlock(lock_t *lock) {
lock->flag = 0;
}