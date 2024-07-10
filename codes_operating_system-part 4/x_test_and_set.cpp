// int TestAndSet(int *ptr, int new) {
// int old = *ptr; // fetch old value at ptr
// // store ’new’ into ptr
// *ptr = new;
// return old;
// // return the old value
// }

// typedef struct __lock_t {
// int flag;
// } lock_t;

// void init(lock_t *lock) {
// // 0 indicates that lock is available, 1 that it is held
// lock->flag = 0;
// }

// void lock(lock_t *lock) {
// while (TestAndSet(&lock->flag, 1) == 1)
// ; // spin-wait (do nothing)
// }

// void unlock(lock_t *lock) {
// lock->flag = 0;
// }