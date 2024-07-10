
//reader writer using semaphore --reader preference

// typedef struct _rwlock_t {
// sem_t lock;
// // binary semaphore (basic lock)
// sem_t writelock; // used to allow ONE writer or MANY readers
// int
// readers;
// // count of readers reading in critical section
// } rwlock_t;

// void rwlock_init(rwlock_t *rw) {
// rw->readers = 0;
// sem_init(&rw->lock, 0, 1);
// sem_init(&rw->writelock, 0, 1);
// }

// void rwlock_acquire_readlock(rwlock_t *rw) {
// sem_wait(&rw->lock);
// rw->readers++;
// if (rw->readers == 1)
// sem_wait(&rw->writelock); // first reader acquires writelock
// sem_post(&rw->lock);
// }

// void rwlock_release_readlock(rwlock_t *rw) {
// sem_wait(&rw->lock);
// rw->readers--;
// if (rw->readers == 0)
// sem_post(&rw->writelock); // last reader releases writelock
// sem_post(&rw->lock);
// }

// void rwlock_acquire_writelock(rwlock_t *rw) {
// sem_wait(&rw->writelock);
// }

// void rwlock_release_writelock(rwlock_t *rw) {
// sem_post(&rw->writelock);
// }

//***********************reader writer writer preference ************************************ 

// int readcount, writecount;                   //(initial value = 0)
// semaphore rmutex, wmutex, readTry, resource; //(initial value = 1)

// //READER
// reader() {
// <ENTRY Section>
//   readTry.P();                 //Indicate a reader is trying to enter
//   rmutex.P();                  //lock entry section to avoid race condition with other readers
//   readcount++;                 //report yourself as a reader
//   if (readcount == 1)          //checks if you are first reader
//     resource.P();              //if you are first reader, lock  the resource
//   rmutex.V();                  //release entry section for other readers
//   readTry.V();                 //indicate you are done trying to access the resource

// <CRITICAL Section>
// //reading is performed

// <EXIT Section>
//   rmutex.P();                  //reserve exit section - avoids race condition with readers
//   readcount--;                 //indicate you're leaving
//   if (readcount == 0)          //checks if you are last reader leaving
//     resource.V();              //if last, you must release the locked resource
//   rmutex.V();                  //release exit section for other readers
// }

// //WRITER
// writer() {
// <ENTRY Section>
//   wmutex.P();                  //reserve entry section for writers - avoids race conditions
//   writecount++;                //report yourself as a writer entering
//   if (writecount == 1)         //checks if you're first writer
//     readTry.P();               //if you're first, then you must lock the readers out. Prevent them from trying to enter CS
//   wmutex.V();                  //release entry section
//   resource.P();                //reserve the resource for yourself - prevents other writers from simultaneously editing the shared resource
// <CRITICAL Section>
//   //writing is performed
//   resource.V();                //release file

// <EXIT Section>
//   wmutex.P();                  //reserve exit section
//   writecount--;                //indicate you're leaving
//   if (writecount == 0)         //checks if you're the last writer
//     readTry.V();               //if you're last writer, you must unlock the readers. Allows them to try enter CS for reading
//   wmutex.V();                  //release exit section
// }



// //************************ reader writer without starvation ************************************

// int readcount;                // init to 0; number of readers currently accessing resource

// // all semaphores initialised to 1
// semaphore resource;           // controls access (read/write) to the resource. Binary semaphore.
// semaphore rmutex;             // for syncing changes to shared variable readcount
// semaphore serviceQueue;       // FAIRNESS: preserves ordering of requests (signaling must be FIFO)

// //READER
// reader() {
// <ENTRY Section>
//   serviceQueue.P();           // wait in line to be serviced
//   rmutex.P();                 // request exclusive access to readcount
//   readcount++;                // update count of active readers
//   if (readcount == 1)         // if I am the first reader
//     resource.P();             // request resource access for readers (writers blocked)
//   serviceQueue.V();           // let next in line be serviced
//   rmutex.V();                 // release access to readcount
    
// <CRITICAL Section>
// //reading is performed
    
// <EXIT Section>
//   rmutex.P();                 // request exclusive access to readcount
//   readcount--;                // update count of active readers
//   if (readcount == 0)         // if there are no readers left
//     resource.V();             // release resource access for all
//   rmutex.V();                 // release access to readcount
// }

// //WRITER
// writer() {
// <ENTRY Section>
//   serviceQueue.P();           // wait in line to be serviced
//   resource.P();               // request exclusive access to resource
//   serviceQueue.V();           // let next in line be serviced
    
// <CRITICAL Section>
// // writing is performed
    
// <EXIT Section>
//   resource.V();               // release resource access for next reader/writer
// }


// ****************************** reader writing using monitors ******************************** 

// #include <iostream>
// #include <pthread.h>
// #include <unistd.h>
// using namespace std;

// class Monitor {
// private:
//     int readerCount;
//     int writerCount;
//     int readerWaiting;
//     int writerWaiting;
//     pthread_cond_t canRead;
//     pthread_cond_t canWrite;
//     pthread_mutex_t lock;

// public:
//     Monitor() {
//         readerCount = 0;
//         writerCount = 0;
//         readerWaiting = 0;
//         writerWaiting = 0;
//         pthread_cond_init(&canRead, NULL);
//         pthread_cond_init(&canWrite, NULL);
//         pthread_mutex_init(&lock, NULL);
//     }

//     void beginRead(int i) {
//         pthread_mutex_lock(&lock);
//         if (writerCount == 1 || writerWaiting > 0) {
//             readerWaiting++;
//             pthread_cond_wait(&canRead, &lock);
//             readerWaiting--;
//         }
//         readerCount++;
//         cout << "Reader " << i << " is reading\n";
//         pthread_mutex_unlock(&lock);
//         pthread_cond_broadcast(&canRead);
//     }

//     void endRead(int i) {
//         pthread_mutex_lock(&lock);
//         if (--readerCount == 0)
//             pthread_cond_signal(&canWrite);
//         pthread_mutex_unlock(&lock);
//     }

//     void beginWrite(int i) {
//         pthread_mutex_lock(&lock);
//         if (writerCount == 1 || readerCount > 0) {
//             writerWaiting++;
//             pthread_cond_wait(&canWrite, &lock);
//             writerWaiting--;
//         }
//         writerCount = 1;
//         cout << "Writer " << i << " is writing\n";
//         pthread_mutex_unlock(&lock);
//     }

//     void endWrite(int i) {
//         pthread_mutex_lock(&lock);
//         writerCount = 0;
//         if (readerWaiting > 0)
//             pthread_cond_signal(&canRead);
//         else
//             pthread_cond_signal(&canWrite);
//         pthread_mutex_unlock(&lock);
//     }
// };

// Monitor M;

// void* reader(void* id) {
//     int c = 0;
//     int i = *(int*)id;
//     while (c < 5) {
//         usleep(1);
//         M.beginRead(i);
//         M.endRead(i);
//         c++;
//     }
// }

// void* writer(void* id) {
//     int c = 0;
//     int i = *(int*)id;
//     while (c < 5) {
//         usleep(1);
//         M.beginWrite(i);
//         M.endWrite(i);
//         c++;
//     }
// }

// int main() {
//     pthread_t r[5], w[5];
//     int id[5];
//     for (int i = 0; i < 5; i++) {
//         id[i] = i;
//         pthread_create(&r[i], NULL, &reader, &id[i]);
//         pthread_create(&w[i], NULL, &writer, &id[i]);
//     }
//     for (int i = 0; i < 5; i++) {
//         pthread_join(r[i], NULL);
//     }
//     for (int i = 0; i < 5; i++) {
//         pthread_join(w[i], NULL);
//     }
// }



// ****************** reader writer using condition variable **************************** 

/* Global variables */
// pthread_mutex_t m;
// pthread_cond_t turn; /* Event: it's our turn */
// int writing;
// int reading;
// void init(void) {
// pthread_mutex_init(&m, NULL);
// pthread_cond_init(&turn, NULL);
// reading = 0;
// writing = 0;
// }

// void reader(void)
// {
// mutex_lock(&m);
// while (writing)
// cond_wait(&turn, &m);
// reading++;
// mutex_unlock(&m);
// /* Reading here */
// mutex_lock(&m);
// reading--;
// cond_signal(&turn);
// mutex_unlock(&m);
// }


// void writer(void)
// {
// mutex_lock(&m);
// while (reading || writing)
// cond_wait(&turn, &m);
// writing++;
// mutex_unlock(&m);
// /* Writing here */
// mutex_lock(&m);
// writing--;
// cond_signal(&turn);
// mutex_unlock(&m);
// }


// ****************** reader writer using condition variable starvatrion free  **************************** 

/* Global variables */
// pthread_mutex_t m;
// pthread_cond_t turn; /* Event: someone else's turn */
// int reading;
// int writing;
// int writers;
// void init(void) {
// pthread_mutex_init(&m, NULL);
// pthread_cond_init(&turn, NULL);
// reading = 0;
// writing = 0;
// writers = 0;
// }

// void reader(void)
// {
// mutex_lock(&m);
// if (writers)
// cond_wait(&turn, &m);
// while (writing)
// cond_wait(&turn, &m);
// reading++;
// mutex_unlock(&m);
// /* Reading here */
// mutex_lock(&m);
// reading--;
// cond_signal(&turn);
// mutex_unlock(&m);
// }


// void writer(void)
// {
// mutex_lock(&m);
// writers++;
// while (reading || writing)
// cond_wait(&turn, &m);
// writing++;
// mutex_unlock(&m);
// /* Writing here */
// mutex_lock(&m);
// writing--;
// writers--;
// cond_signal(&turn);
// mutex_unlock(&m);
// }