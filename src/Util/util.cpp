#ifndef DSYNC_UTIL_H
#define DSYNC_UTIL_H


#include <algorithm>
#include <cstdarg>
#include <string>
#include <vector>
#include <memory>
#include <cstring>
#include "util.h"


bool in_vector(const std::string &value, const std::vector<std::string> &array) {
    return std::find(array.begin(), array.end(), value) != array.end();
}

std::string implode(const std::vector<std::string> &array, const std::string &delimiter) {
    std::string result;

    for (int i = 0; i < array.size(); ++i) {
        if (i == array.size() - 1) {
            result.append(array[i]);
        } else {
            result.append(array[i]).append(delimiter);
        }
    }

    return result;
}

#endif //DSYNC_UTIL_H
