#include "policies.h"
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cstddef> // for nullptr
#include <cmath>

using namespace std;

void segmented_fifo(const char* file_name, int frame_count, int percentage, const std::string& mode)
{
    vector<int> fifo_queue;
    vector<int> lru_queue;
    map<int, char> fifo_memory;
    map<int, char> lru_memory;

    int event_counter = 0;
    int read_counter = 0;
    int write_counter = 0;

    double lru_frame_count = static_cast<double>(frame_count * percentage) / 100;
    if(lru_frame_count > 0 && lru_frame_count < 1) {
        percentage = 0;
        lru_frame_count = 0;
    } else {
        lru_frame_count = round(static_cast<int>(lru_frame_count));
    }
    int fifo_frame_count = frame_count - lru_frame_count;
    cout << "lru: " << lru_frame_count << endl;
    cout << "fifo: " << fifo_frame_count << endl;

    if (percentage == 0) {
       fifo(file_name, fifo_frame_count, mode);
    } else if (percentage == 100) {
       lru(file_name, lru_frame_count, mode);
    } else {
        unsigned address = 0;
        char rw = ' ';
        FILE *file_ptr = fopen(file_name, "r");

        if (file_ptr == nullptr) {
            cerr << "Failed to open file " << file_name << endl;
            exit(EXIT_FAILURE);
        }

        while (fscanf(file_ptr,"%x %c", &address, &rw) != EOF) {
            address /= 4096;

            if (fifo_memory.find(address) == fifo_memory.end()) {   
                if (fifo_memory.size() < fifo_frame_count) {
                    if (mode == "debug") {
                        cout << "Page Miss" << endl;
                    }
                    read_counter++;
                    fifo_queue.push_back(address);
                    fifo_memory[address] = rw;
                } else {
                    vector<int>::iterator iter = find(lru_queue.begin(), lru_queue.end(), address);
                    if (lru_memory.find(address) == lru_memory.end()) { 
                        if (mode == "debug") {
                            cout << "Page Miss" << endl;
                        }
                        if(lru_memory.size() < lru_frame_count) { 
                            read_counter++;

                            lru_queue.push_back(fifo_queue.front());
                            lru_memory[fifo_queue.front()] = fifo_memory[fifo_queue.front()];

                            fifo_memory.erase(fifo_queue.front());
                            fifo_queue.erase(fifo_queue.begin());

                            fifo_memory[address] = rw;
                            fifo_queue.push_back(address);
                        }
                        else {
                            read_counter++;
                            if (lru_memory[lru_queue.front()] == 'W')
                                write_counter++;

                            lru_memory.erase(lru_queue.front());
                            lru_queue.erase(lru_queue.begin());

                            lru_queue.push_back(fifo_queue.front());
                            lru_memory[fifo_queue.front()] = fifo_memory[fifo_queue.front()];

                            fifo_memory.erase(fifo_queue.front());
                            fifo_queue.erase(fifo_queue.begin());

                            fifo_memory[address] = rw;
                            fifo_queue.push_back(address);
                        }
                    } else {
                        if (mode == "debug") {
                            cout << "Page Hit" << endl;
                        }
                        if (rw == 'W')
                            lru_memory[address] = 'W';

                        fifo_memory[address] = lru_memory[address];
                        fifo_queue.push_back(address);

                        lru_memory.erase(address);
                        lru_queue.erase(iter);

                        lru_queue.push_back(fifo_queue.front());
                        lru_memory[fifo_queue.front()] = fifo_memory[fifo_queue.front()];

                        fifo_memory.erase(fifo_queue.front());
                        fifo_queue.erase(fifo_queue.begin());
                    }
                }
            } else {    //in FIFO
                if (mode == "debug") {
                    cout << "Page Hit" << endl;
                }

                if (rw == 'W') {
                    fifo_memory[address] = rw;
                }
            }

            event_counter++;
        }

        fclose(file_ptr);

        cout << "Total Memory Frames: " << frame_count << endl;
        cout << "Events in trace: " << event_counter << endl;
        cout << "Total Disk Reads: " << read_counter << endl;
        cout << "Total Disk Writes: " << write_counter << endl;

        // cout << "Hits: " << event_counter - read_counter << endl;
        // cout << "Misses: " << read_counter << endl; 
        // Uncomment above lines for debugging
    }
}
