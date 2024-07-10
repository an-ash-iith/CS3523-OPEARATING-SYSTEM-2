#include <iostream>
#include <iomanip>
#include <pthread.h>
#include <vector>
#include <time.h>
#include <unistd.h>
#include <fstream>
#include <chrono>
#include <semaphore.h>
#include <random>

using namespace std;

typedef struct {
    int no_of_times;
    int mucs;
    int murem;
    int thread_id;
    vector<double>* time;
} arguments;

int readcount=0;
int writecount=0;

sem_t available_resource;
sem_t lock_reader;
sem_t lock_writer;
sem_t queueservice;

FILE* fptr;

double randCSTime(double muCS) {
    random_device rand;
    mt19937 gen(rand());
    exponential_distribution<double> dist(1.0 / muCS);
    return dist(gen);
}

double randRemTime(double muRem) {
    random_device rand;
    mt19937 gen(rand());
    exponential_distribution<double> dist(1.0 / muRem);
    return dist(gen);
}

string currenttime(double* time) {
    auto now = chrono::system_clock::now();
    auto us = chrono::time_point_cast<chrono::microseconds>(now);
    auto epoch = us.time_since_epoch();
    auto value = chrono::duration_cast<chrono::microseconds>(epoch);
    chrono::microseconds current_us = chrono::duration_cast<chrono::microseconds>(value);
    auto seconds = chrono::duration_cast<chrono::seconds>(current_us);
    current_us -= seconds;
    auto milliseconds = chrono::duration_cast<chrono::milliseconds>(current_us);
    current_us -= milliseconds;
    *time = milliseconds.count() * 1000 + current_us.count();
    return to_string(milliseconds.count()) + ":" + to_string(current_us.count());
}

void* reader(void* args) {
    arguments* threadArgs = (arguments*)args;
    int kr = threadArgs->no_of_times;
    int murem = threadArgs->murem;
    int mucs = threadArgs->mucs;
    int id = threadArgs->thread_id;
    vector<double> &time = *(threadArgs->time);

    double sum = 0;

    for (int i = 0; i < kr; i++) {
        double timeStamp1;
        string reqTime = currenttime(&timeStamp1);
        printf("%dth CS requested by Reader Thread %d at %s\n", i, id, reqTime.c_str());
        fprintf(fptr, "%dth CS requested by Reader Thread %d at %s\n", i, id, reqTime.c_str());
        sem_wait(&queueservice);
        sem_wait(&lock_reader);
        readcount++;

        if (readcount == 1)
            sem_wait(&available_resource);

        sem_post(&lock_reader);
        sem_post(&queueservice);

        double timeStamp2;
        string enterTime = currenttime(&timeStamp2);
        printf("%dth CS entered by Reader Thread %d at %s\n", i, id, enterTime.c_str());
        fprintf(fptr, "%dth CS entered by Reader Thread %d at %s\n", i, id, enterTime.c_str());
        usleep(randCSTime(mucs));

        sem_wait(&lock_reader);
        readcount--;

        if (readcount == 0)
            sem_post(&available_resource);

        sem_post(&lock_reader);

        double timeStamp3;
        string exitTime = currenttime(&timeStamp3);
        printf("%dth CS exited by Reader Thread %d at %s\n", i, id, exitTime.c_str());
        fprintf(fptr, "%dth CS exited by Reader Thread %d at %s\n", i, id, exitTime.c_str());
        usleep(randRemTime(murem));
        sum = sum + (timeStamp2 - timeStamp1);
    }

    time[id] = sum / kr;

    return NULL;
}

void* writer(void* args) {
    arguments* threadArgs = (arguments*)args;
    vector<double> &time = *(threadArgs->time);
    int kw = threadArgs->no_of_times;
    int murem = threadArgs->murem;
    int mucs = threadArgs->mucs;
    int id = threadArgs->thread_id;
    double sum = 0;

    for (int i = 0; i < kw; i++) {
        double timeStamp1;
        string reqTime = currenttime(&timeStamp1);
        printf("%dth CS requested by Writer Thread %d at %s\n", i, id, reqTime.c_str());
        fprintf(fptr, "%dth CS requested by Writer Thread %d at %s\n", i, id, reqTime.c_str());
        sem_wait(&lock_writer);
        writecount++;

        if (writecount == 1)
            sem_wait(&queueservice);

        sem_post(&lock_writer);
        sem_wait(&available_resource);

        double timeStamp2;
        string enterTime = currenttime(&timeStamp2);
        printf("%dth CS entered by Writer Thread %d at %s\n", i, id, enterTime.c_str());
        fprintf(fptr, "%dth CS entered by Writer Thread %d at %s\n", i, id, enterTime.c_str());
        usleep(randCSTime(mucs));

        sem_post(&available_resource);
        sem_wait(&lock_writer);
        writecount--;

        if (writecount == 0)
            sem_post(&queueservice);

        sem_post(&lock_writer);
        double timeStamp3;
        string exitTime = currenttime(&timeStamp3);
        printf("%dth CS exited by writer Thread %d at %s\n", i, id, exitTime.c_str());
        fprintf(fptr, "%dth CS exited by writer Thread %d at %s\n", i, id, exitTime.c_str());
        usleep(randRemTime(murem));
        sum = sum + timeStamp2 - timeStamp1;
    }

    time[id] = sum / kw;
    return NULL;
}

int main() {
    int nr, nw, kr, kw, mucs, murem;
    string filename = "inp-parms.txt";
    ifstream input(filename);

    if (!input.is_open()) {
        cerr << "File opening failed..." << endl;
        return EXIT_FAILURE;
    }

    input >> nw >> nr >> kw >> kr >> mucs >> murem;
    input.close();

    cout << nw<< " " << nr << " " << kw << " " << kr << endl;
    sem_init(&available_resource, 0, 1);
    sem_init(&lock_reader, 0, 1);
    sem_init(&lock_writer, 0, 1);
    sem_init(&queueservice, 0, 1);
    fptr = fopen("sample2.txt", "w");

    pthread_t thread[nr + nw];
    vector<double> time_reader(nr, 0);
    vector<double> time_writer(nw, 0);
    arguments arg_writer[nw];
    arguments arg_reader[nr];

    for (int i = 0; i < nw; i++) {
        arg_writer[i].mucs = mucs;
        arg_writer[i].murem = murem;
        arg_writer[i].no_of_times = kw;
        arg_writer[i].time = &time_writer;
        arg_writer[i].thread_id = i;

        if (pthread_create(thread + i, NULL, &writer, (void*)&arg_writer[i]) != 0)
            return 2;
    }

    for (int j = 0; j < nr; j++) {
        arg_reader[j].mucs = mucs;
        arg_reader[j].murem = murem;
        arg_reader[j].no_of_times = kr;
        arg_reader[j].time = &time_reader;
        arg_reader[j].thread_id = j;

        if (pthread_create(&thread[j + nw], NULL, &reader, (void*)&arg_reader[j]) != 0)
            return 2;
    }

    for (int i = 0; i < nw + nr; i++) {

        if (pthread_join(thread[i], NULL) != 0)
            
            {
            return 3;
            }
    }

    double averagetime_writer = 0;
    double averagetime_reader = 0;
    double worsttime_writer = 0;
    double worsttime_reader = 0;

    for (int i = 0; i < nw; i++) {
        // cout << time_writer[i] << " ";
        averagetime_writer = averagetime_writer + time_writer[i];
        if (worsttime_writer < time_writer[i])
            worsttime_writer = time_writer[i];
    }

    cout << endl;
    cout << "average time taken by writer " << averagetime_writer / nw << endl;
    cout << "worst time taken by writer " << worsttime_writer << endl;


//this is a readre section 
    for (int i = 0; i < nr; i++) {
        // cout << time_reader[i] << " ";
        averagetime_reader = averagetime_reader + time_reader[i];
        if (worsttime_reader < time_reader[i])
            worsttime_reader = time_reader[i];
    }

    cout << endl;
    cout << "average time taken by reader " << averagetime_reader / nr << endl;
    cout << "worst time taken by reader " << worsttime_reader << endl;

    fclose(fptr);

    return 0;
}
