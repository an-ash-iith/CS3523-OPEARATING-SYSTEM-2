#include <iostream>
#include <fstream>
#include <queue>
#include <vector>

using namespace std;

int FIFO(const vector<int>& address, int N) {
    int faults = 0;
    int size_queue = 0;
    vector<int> current;
    queue<int> q;

    for (int i = 0; i < address.size(); i++) {
        if (i != 0) {
            bool in_queue = false;
            for (int s = 0; s < current.size(); s++) {
                if (current[s] == address[i]) {
                    in_queue = true;
                    break;
                }
            }
            if (!in_queue) {
                if (size_queue == N) {
                    int remove = q.front();
                    q.pop();
                    q.push(address[i]);
                    for (int s = 0; s < current.size(); s++) {
                        if (current[s] == remove) {
                            current[s] = -1;
                            break;
                        }
                    }
                    faults++;
                    current.push_back(address[i]);
                } else {
                    q.push(address[i]);
                    faults++;
                    size_queue++;
                    current.push_back(address[i]);
                }
            }
        }
        if (i == 0) {
            faults++;
            size_queue++;
            q.push(address[i]);
            current.push_back(address[i]);
        }
    }
    return faults;
}

int LRU(const vector<int>& address, int N) {
    int frames[N];
    int frames_occupied = 0;
    int last_references[N];
    int faults = 0;

    for (int i = 0; i < N; i++) frames[i] = -1;

    for (int i = 0; i < address.size(); i++) {
        if (i != 0) {
            bool in_frames = false;
            for (int s = 0; s < N; s++) {
                if (frames[s] == address[i]) {
                    in_frames = true;
                    last_references[s] = i;
                    break;
                }
            }
            if (!in_frames) {
                if (frames_occupied == N) {
                    int least_max_refer = 1e8;
                    int index_to_replace = -1;
                    for (int s = 0; s < N; s++) {
                        if (last_references[s] < least_max_refer) {
                            index_to_replace = s;
                            least_max_refer = last_references[s];
                        }
                    }
                    frames[index_to_replace] = address[i];
                    last_references[index_to_replace] = i;
                    faults++;
                } else {
                    frames[frames_occupied] = address[i];
                    last_references[frames_occupied] = i;
                    frames_occupied++;
                    faults++;
                }
            }
        } else {
            frames[frames_occupied] = address[i];
            last_references[frames_occupied] = i;
            faults++;
        }
    }
    return faults;
}

int OPT(const vector<int>& address, int N) {
    int frames[N];
    int frames_occupied = 0;
    int faults = 0;

    for (int i = 0; i < N; i++) frames[i] = -1;

    for (int i = 0; i < address.size(); i++) {
        if (i != 0) {
            bool in_frames = false;
            for (int s = 0; s < N; s++) {
                if (frames[s] == address[i]) {
                    in_frames = true;
                    break;
                }
            }
            if (!in_frames) {
                if (frames_occupied == N) {
                    int next_references[N];
                    for (int s = 0; s < N; s++) next_references[s] = 1e7;
                    for (int elem = 0; elem < N; elem++) {
                        for (int s = i + 1; s < address.size(); s++) {
                            if (frames[elem] == address[s]) {
                                next_references[elem] = s;
                                break;
                            }
                        }
                    }
                    int max_ref = -1e6;
                    int index_to_replace = -1;
                    for (int s = 0; s < N; s++) {
                        if (next_references[s] > max_ref) {
                            max_ref = next_references[s];
                            index_to_replace = s;
                        }
                    }
                    frames[index_to_replace] = address[i];
                    faults++;
                } else {
                    frames[frames_occupied] = address[i];
                    frames_occupied++;
                    faults++;
                }
            }
        } else {
            frames[frames_occupied] = address[i];
            frames_occupied++;
            faults++;
        }
    }
    return faults;
}

int main() {
    string input_file = "input.txt";
    ifstream indata;
    indata.open(input_file);

    if (!indata) {
        cout << "Error: File not Found!" << endl;
        return 0;
    }

    int N, pagesize;
    indata >> N >> pagesize;

    vector<int> address{};
    int a;
    while (true) {
        indata >> a;
        if (a == -1) break;
        address.push_back(a / pagesize);
    }
    indata.close();

    printf("The number of page faults with FIFO is %d\n", FIFO(address, N));
    printf("The number of page faults with LRU is %d\n", LRU(address, N));
    printf("The number of page faults with OPT is %d\n", OPT(address, N));

    return 0;
}
