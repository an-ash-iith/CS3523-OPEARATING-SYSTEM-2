#include <iostream>
#include <iomanip> //using it for setting the precision
#include <pthread.h>
#include <vector>
#include <time.h>
#include <unistd.h>
#include <fstream>
#include <chrono>
#include <semaphore.h>
#include <random>

using namespace std;

typedef struct
{
    int no_of_times;
    int mucs;
    int murem;
    int thread_id;
    vector<double> *time;
} arguments;

int readcount = 0;  //(initial value = 0)
int writecount = 0; //(initial value = 0)
sem_t available_reseource;
sem_t lock_reader;
sem_t lock_writer;
sem_t queueservice;

FILE *fptr = fopen("RW-log.txt", "w");

double randCSTime(double muCS)
{
    // Create a random number generator engine
    random_device rand;
    mt19937 gen(rand());

    // Create an exponential distribution with average muCS
    exponential_distribution<double> dist(1.0 / muCS);

    // Generate a random delay time for CS
    return dist(gen);
}

// Function to generate a random delay time for Rem (Remaining Time)
double randRemTime(double muRem)
{
    // Create a random number generator engine
    random_device rand;
    mt19937 gen(rand());

    // Create an exponential distribution with average muRem
    exponential_distribution<double> dist(1.0 / muRem);

    // Generate a random delay time for Rem
    return dist(gen);
}

string currenttime(double *time)
{
    // Get the current time point
    auto now = chrono::system_clock::now();

    // Convert to microseconds since epoch
    auto us = chrono::time_point_cast<chrono::microseconds>(now);
    auto epoch = us.time_since_epoch();
    auto value = chrono::duration_cast<chrono::microseconds>(epoch);
    chrono::microseconds current_us = chrono::duration_cast<chrono::microseconds>(value);

    auto seconds = chrono::duration_cast<chrono::seconds>(current_us);
    current_us -= seconds;
    auto milliseconds = chrono::duration_cast<chrono::milliseconds>(current_us);
    current_us -= milliseconds;

    *time = milliseconds.count() * 1000 + current_us.count();

    // Format the time stamp
    return to_string(milliseconds.count()) + ":" + to_string(current_us.count());
}

void *reader(void *args)
{
    arguments *threadArgs = (arguments *)args;
    int kr = threadArgs->no_of_times;
    int murem = threadArgs->murem;
    int mucs = threadArgs->mucs;
    int id = threadArgs->thread_id;
    vector<double> &time = *(threadArgs->time);
    // printf("Welcome to the reader\n");
// printf("id is %d\n",id);
    double sum = 0; // take the sum of the time taken by each thread from request to getting of critical section

    for (int i = 0; i < kr; i++)
    {
        double timeStamp1;
        string reqTime = currenttime(&timeStamp1);

        printf("%dth CS requested by Reader Thread %d at %s\n", i, id, reqTime.c_str());
        fprintf(fptr, "%dth CS requested by Reader Thread %d at %s\n", i, id, reqTime.c_str());
        sem_wait(&queueservice);
        sem_wait(&lock_reader);
        readcount++;

        if (readcount == 1)
            sem_wait(&available_reseource);

        sem_post(&lock_reader);
        sem_post(&queueservice);

        // this the citical section and at a type many readers can run in critical section

        double timeStamp2;
        string enterTime = currenttime(&timeStamp2);
        printf("%dth CS entered by Reader Thread %d at %s\n", i, id, enterTime.c_str());
        fprintf(fptr, "%dth CS entered by Reader Thread %d at %s\n", i, id, enterTime.c_str());

        // sleepint to simulate similar to the work in critical section
        usleep(randCSTime(mucs)); // usleep is used for microseconds sleep

        // code for thread to exit the criticalsection

        sem_wait(&lock_reader);
        readcount--;

        if (readcount == 0)
            sem_post(&available_reseource);

        sem_post(&lock_reader);

        double timeStamp3;
        string exitTime = currenttime(&timeStamp3);
        printf("%dth CS exited by Reader Thread %d at %s\n", i, id, exitTime.c_str());
        fprintf(fptr, "%dth CS exited by Reader Thread %d at %s\n", i, id, exitTime.c_str());
        usleep(randRemTime(murem));

        // cout << exitTime[1] - reqTime[1];

        // nothing the time taken between request and time to enter the critical section
        sum = sum + (timeStamp2 - timeStamp1);
    }

    // average time taken by this thread for critical section
    time[id] = sum / kr;

    return NULL;
}

void *writer(void *args)
{
    arguments *threadArgs = (arguments *)args;
    vector<double> &time = *(threadArgs->time);
    int kw = threadArgs->no_of_times;
    int murem = threadArgs->murem;
    int mucs = threadArgs->mucs;
    int id = threadArgs->thread_id;
  
    // printf("Welcome to the writer\n");
    double sum = 0;

    for (int i = 0; i < kw; i++)
    {
        double timeStamp1;
        string reqTime = currenttime(&timeStamp1);
        printf("%dth CS entered by Writer Thread %d at %s\n", i, id, reqTime.c_str());
        fprintf(fptr, "%dth CS entered by Writer Thread %d at %s\n", i, id, reqTime.c_str());

        sem_wait(&lock_writer);
        writecount++;

        if (writecount == 1)
            sem_wait(&queueservice);
        sem_post(&lock_writer);

        sem_wait(&available_reseource);

        double timeStamp2;
        string enterTime = currenttime(&timeStamp2);
        printf("%dth CS entered by Writer Thread %d at %s\n", i, id, enterTime.c_str());
        fprintf(fptr, "%dth CS entered by Writer Thread %d at %s\n", i, id, enterTime.c_str());
        usleep(randCSTime(mucs)); // usleep is used for microseconds sleep

        sem_post(&available_reseource);

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

        // noting the time taken between request and critical section
        sum = sum + timeStamp2 - timeStamp1;
    }

    // average time taken by this thread
    time[id] = sum / kw;
    return NULL;
}

int main()
{
    int nw, nr, kr, kw, mucs, murem;
  

    string filename = "inp-parms.txt";

    ifstream input(filename);

    if (!input.is_open())
    {
        cerr << "File opening failed..." << endl;
        return EXIT_FAILURE;
    }

    input >> nw >> nr >> kw >> kr >> mucs >> murem;
    input.close();
    
    cout<<nw<<" "<<nr<<" "<<kw<<" "<<kr<<" "<<endl;
    sem_init(&available_reseource, 0, 1);
    sem_init(&lock_reader, 0, 1);
    sem_init(&lock_writer, 0, 1);
    sem_init(&queueservice, 0, 1);

    pthread_t thread[nr + nw];

    vector<double> time_reader(nr, 0);
    vector<double> time_writer(nw, 0);

    arguments arg_writer[nw];
    arguments arg_reader[nr];


    for (int i = 0; i < nw; i++)
    {   
    arg_writer[i].mucs = mucs;
    arg_writer[i].murem = murem;
    arg_writer[i].no_of_times = kw;
    arg_writer[i].time = &time_writer;
    arg_writer[i].thread_id = i;

        if (pthread_create(thread + i, NULL, &writer, (void *)&arg_writer[i]) != 0)
            return 2;
    }

    for (int j = 0; j < nr; j++)
    {   arg_reader[j].mucs = mucs;
        arg_reader[j].murem = murem;
        arg_reader[j].no_of_times = kr;
        arg_reader[j].time = &time_reader;
        arg_reader[j].thread_id = j;
        if (pthread_create(&thread[j + nw], NULL, &reader, (void *)&arg_reader[j]) != 0)
            return 2;
    }


    for (int i = 0; i < nw + nr; i++)
    {

        if (pthread_join(thread[i], NULL) != 0)
            return 3;
    }
    
    fclose(fptr);
    

  double averagetime_writer=0;
  double averagetime_reader=0;

  double worsttime_writer=0;
  double worsttime_reader=0;


    // average time taken by writer threads 

    for (int i = 0; i < nw; i++)
    {
    //    cout<<time_writer[i]<<" ";
       averagetime_writer=averagetime_writer+time_writer[i];

       if(worsttime_writer<time_writer[i])
        worsttime_writer=time_writer[i]; 
    }
    
        // average time taken by read0s 

    for (int i = 0; i < nr; i++)
    {
    //    cout<<time_reader[i]<<" ";
       averagetime_reader=averagetime_reader+time_reader[i];

       if(worsttime_reader<time_reader[i])
        worsttime_reader=time_reader[i]; 
    }
   

    ofstream outputFile("Average time.txt");

    // Check if the file is opened successfully
    if (outputFile.is_open()) {
        // Write to the file
        outputFile << "average time taken by writer " << averagetime_writer / nw <<endl;
        outputFile << "worst time taken by writer " << worsttime_writer <<endl;
        outputFile<<"average time taken by reader "<<averagetime_reader/nr<<endl;
        outputFile<<"worst time taken by reader "<<worsttime_reader<<endl;

        // Close the file
        outputFile.close();
    }
    

    return 0;
}