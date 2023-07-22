#include "policies.h"
#include <algorithm>
#include <iostream>
#include <map>
#include <queue>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <cstddef> // for nullptr


void fifo(const char* file_name, unsigned int frame_num, std::string mode) {
    std::queue<int> page_table;
    std::map<int, char> memory;
    int event_counter = 0;
    int read_counter = 0;
    int write_counter = 0;
    unsigned address = 0;
    char read_write_flag = ' ';
    FILE* file_pointer = fopen(file_name, "r");

    if (file_pointer == nullptr) {
        std::cerr << "Failed to open file " << file_name << std::endl;
        std::exit(EXIT_FAILURE);
    }

    while (fscanf(file_pointer, "%x %c", &address, &read_write_flag) != EOF) {
        address /= 4096;

        if (memory.find(address) == memory.end()) {
            if (mode == "debug") {
                std::cout << "page miss" << std::endl;
            }
            read_counter++;

            if (memory.size() < frame_num) {
                page_table.push(address);
                memory[address] = read_write_flag;
            } else {
                if (memory[page_table.front()] == 'W') {
                    write_counter++;
                }

                memory.erase(page_table.front());
                page_table.pop();
                memory[address] = read_write_flag;
                page_table.push(address);
            }
        } else {
            if (mode == "debug") {
                std::cout << "page hit" << std::endl;
            }

            if (memory[address] == 'R') {
                memory[address] = read_write_flag;
            }
        }
        event_counter++;
    }
    fclose(file_pointer);

    std::cout << "total memory frames: " << frame_num << std::endl;
    std::cout << "events during in trace: " << event_counter << std::endl;
    std::cout << "total disk reads: " << read_counter << std::endl;
    std::cout << "total disk writes: " << write_counter << std::endl;
}
