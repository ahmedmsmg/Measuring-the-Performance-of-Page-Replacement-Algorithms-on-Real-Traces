#include "policies.h"
#include <iostream>
#include <deque>
#include <map>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstddef> // for nullptr


void lru(const char* file_name, unsigned int frame_count, const std::string& mode) {

    std::deque<int> page_table;
    std::map<int, char> memory;

    unsigned int event_counter = 0;
    unsigned int read_counter = 0;
    unsigned int write_counter = 0;
    unsigned address = 0;
    char rw = ' ';
    FILE* file_ptr = fopen(file_name, "r");

    if (file_ptr == nullptr) {
        std::cerr << "Failed to open file " << file_name << std::endl;
        std::exit(EXIT_FAILURE);
    }

    while (fscanf(file_ptr, "%x %c", &address, &rw) != EOF) {
        address /= 4096;
        auto iter = std::find(page_table.begin(), page_table.end(), address);

        if (iter == page_table.end()) {
            if (mode == "debug") {
                std::cout << "Page miss" << std::endl;
            }
            read_counter++;

            if (memory.size() < frame_count) {
                page_table.push_back(address);
                memory[address] = rw;
            } else {
                if (memory[page_table.front()] == 'W') {
                    write_counter++;
                }
                memory.erase(page_table.front());
                page_table.pop_front();
                memory[address] = rw;
                page_table.push_back(address);
            }
        } else {
            if (mode == "debug") {
                std::cout << "Page hit" << std::endl;
            }
            page_table.erase(iter);
            page_table.push_back(address);
            if (memory[address] == 'R') {
                memory[address] = rw;
            }
        }
        event_counter++;
    }
    fclose(file_ptr);

    std::cout << "Total memory frames: " << frame_count << std::endl;
    std::cout << "Events during in trace: " << event_counter << std::endl;
    std::cout << "Total disk reads: " << read_counter << std::endl;
    std::cout << "Total disk writes: " << write_counter << std::endl;
}
