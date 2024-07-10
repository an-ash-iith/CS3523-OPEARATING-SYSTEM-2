
//prdoucer consumer 
int buffer;
int count = 0; // initially, empty

void put(int value) {
assert(count == 0);
count = 1;
buffer = value;
}

int get() {
assert(count == 1);
count = 0;
return buffer;
}

void *producer(void *arg) {
int i;
int loops = (int) arg;
for (i = 0; i < loops; i++) {
put(i);
}
}

void *consumer(void *arg) {
int i;
while (1) {
int tmp = get();
printf("%d\n", tmp);
}
}

// ************** producer consumer using semaphore ************* 
// wait(S){
// while(S<=0);   // busy waiting
// S--;
// }

// signal(S){
// S++;
// }


// do{

// //produce an item

// wait(empty);
// wait(mutex);

// //place in buffer

// signal(mutex);
// signal(full);

// }while(true)

// do{

// wait(full);
// wait(mutex);

// // consume item from buffer

// signal(mutex);
// signal(empty);


// }while(true)

// ************** producer consumer using condition variable ************* 


/*
int buffer[MAX];
int fill = 0;
int use= 0;
int count = 0;

void put(int value) {
buffer[fill] = value;
fill = (fill + 1) % MAX;
count++;
}


int get() {
int tmp = buffer[use];
use = (use + 1) % MAX;
count--;
return tmp;
}

cond_t empty, fill;
mutex_t mutex;

void *producer(void *arg) {
int i;
for (i = 0; i < loops; i++) {
Pthread_mutex_lock(&mutex);
// p1
while (count == MAX)
// p2
Pthread_cond_wait(&empty, &mutex); // p3
put(i);
// p4
Pthread_cond_signal(&fill);
// p5
Pthread_mutex_unlock(&mutex);
// p6
}
}

void *consumer(void *arg) {
int i;
for (i = 0; i < loops; i++) {
Pthread_mutex_lock(&mutex);
while (count == 0)
Pthread_cond_wait(&fill, &mutex);
int tmp = get();
Pthread_cond_signal(&empty);
Pthread_mutex_unlock(&mutex);
printf("%d\n", tmp);
}
}

Figure 30.10: The Final Working Solution

*/


// 