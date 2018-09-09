#ifndef DSYNC_UTIL_H
#define DSYNC_UTIL_H

#include <string>
#include <vector>

template<typename ... Args>

std::string format(const std::string &format, Args ... args);

bool in_vector(const std::string &value, const std::vector<std::string> &array);


#endif //DSYNC_UTIL_H
