#ifndef POLICIES_H
#define POLICIES_H

#include <string>
void lru(const char* file_name, unsigned int frame_count, const std::string& mode);
void fifo(const char* file_name, unsigned int frame_num, std::string mode);
void segmented_fifo(const char* file_name, int frame_count, int percentage, const std::string& mode);

#endif // POLICIES_H
