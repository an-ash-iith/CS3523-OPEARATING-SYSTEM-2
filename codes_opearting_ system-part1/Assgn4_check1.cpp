#include <iostream>
#include <fstream>
#include <pthread.h>
#include <vector>
#include <chrono>
#include <unistd.h> // for sleep function
using namespace std;

typedef struct
{
    int no_of_times;
    double mucs;
    double murem;
    int thread_id;
    vector<double> time;

} arguments;

void readdata(int &nw, int &nr, int &kw, int &kr, int &mucs, int &murem, string &file1)
{
    ifstream input(file1);

    if (!input.is_open())
    {
        cerr << "file opening failed ..." << endl;
        exit(EXIT_FAILURE);
    }

    input >> nw >> nr >> kw >> kr >> mucs >> murem;
    input.close();
}

void *reader(void *args)
{
    arguments *threadArgs = (arguments *)args;
    int kw = threadArgs->no_of_times;
    int randRemTime = threadArgs->murem;
    int randCSTime = threadArgs->mucs;
    int id = threadArgs->thread_id;

    cout << "Welcome to the reader" << endl;

    for (int i = 0; i < kw; i++)
    {
        auto reqMicros = chrono::high_resolution_clock::now();
        double reqTime = chrono::duration_cast<chrono::microseconds>(reqMicros.time_since_epoch()).count();
        cout << i << "th CS requested by Writer Thread " << id << " at " << reqTime << endl;

        auto enterTime = chrono::high_resolution_clock::now();
        double enterMicros = chrono::duration_cast<chrono::microseconds>(enterTime.time_since_epoch()).count();
        cout << i << "th CS entered by Writer Thread " << id << " at " << enterMicros << endl;
        usleep(randCSTime); // usleep is used for microseconds sleep

        auto exitTime = chrono::high_resolution_clock::now();
        double exitMicros = chrono::duration_cast<chrono::microseconds>(exitTime.time_since_epoch()).count();
        cout << i << "th CS exited by Writer Thread " << id << " at " << exitMicros << endl;

        usleep(randRemTime);
    }

    return NULL;
}

void *writer(void *args)
{
    arguments *threadArgs = (arguments *)args;
    int kw = threadArgs->no_of_times;
    int randRemTime = threadArgs->murem;
    int randCSTime = threadArgs->mucs;
    int id = threadArgs->thread_id;

    cout << "Welcome to the writer" << endl;

    for (int i = 0; i < kw; i++)
    {
        auto reqTime = chrono::high_resolution_clock::now();
        double reqMicros = chrono::duration_cast<chrono::microseconds>(reqTime.time_since_epoch()).count();
        cout << i << "th CS requested by Reader Thread " << id << " at " << reqMicros << endl;

        auto enterTime = chrono::high_resolution_clock::now();
        double enterMicros = chrono::duration_cast<chrono::microseconds>(enterTime.time_since_epoch()).count();
        cout << i << "th CS entered by Reader Thread " << id << " at " << enterMicros << endl;
        usleep(randCSTime);

        auto exitTime = chrono::high_resolution_clock::now();
        double exitMicros = chrono::duration_cast<chrono::microseconds>(exitTime.time_since_epoch()).count();
        cout << i << "th CS exited by Reader Thread " << id << " at " << exitMicros << endl;

        usleep(randRemTime);
    }

    return NULL;
}

int main()
{
    int nr, nw, kr, kw, mucs, murem;
    string filename = "inp-parms.txt";
    readdata(nr, nw, kw, kr, mucs, murem, filename);

    pthread_t thread[nr + nw];
    arguments arg_writer;
    arguments arg_reader;

    arg_reader.mucs = mucs;
    arg_reader.murem = murem;
    arg_reader.no_of_times = kr;

    arg_writer.mucs = mucs;
    arg_writer.murem = murem;
    arg_writer.no_of_times = kw;

    for (int i = 0; i < kw; i++)
    {
        if (pthread_create(&thread[i], NULL, &reader, (void *)&arg_writer) != 0)
            return 2;
    }

    for (int i = 0; i < nr; i++)
    {
        if (pthread_create(&thread[i + kw], NULL, &writer, (void *)&arg_reader) != 0)
            return 2;
    }

    pthread_exit(NULL);
    return 0;
}
