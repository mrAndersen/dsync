#ifndef DSYNC_UTIL_H
#define DSYNC_UTIL_H


#include <algorithm>
#include <cstdarg>
#include <string>
#include <vector>
#include <memory>
#include <cstring>

bool in_vector(const std::string &value, const std::vector<std::string> &array) {
    return std::find(array.begin(), array.end(), value) != array.end();
}

template<typename ... Args>

std::string format(const std::string &format, Args ... args) {
    size_t size = snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
    std::unique_ptr<char[]> buf(new char[size]);
    snprintf(buf.get(), size, format.c_str(), args ...);
    return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}

#endif //DSYNC_UTIL_H
