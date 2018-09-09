#ifndef DSYNC_UTIL_H
#define DSYNC_UTIL_H

#include <string>
#include <vector>
#include <algorithm>

bool in_vector(const std::string &value, const std::vector<std::string> &array) {
    return std::find(array.begin(), array.end(), value) != array.end();
}

#endif //DSYNC_UTIL_H
