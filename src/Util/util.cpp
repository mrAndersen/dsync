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

#endif //DSYNC_UTIL_H
