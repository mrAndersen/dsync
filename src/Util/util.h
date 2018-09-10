#ifndef DSYNC_UTIL_H
#define DSYNC_UTIL_H

#include <string>
#include <vector>

const int BATCH_SIZE = 1000;
const int VERBOSITY_SILENT = 0;
const int VERBOSITY_DEBUG = 4;

bool in_vector(const std::string &value, const std::vector<std::string> &array);

std::string implode(const std::vector<std::string> &array, const std::string &delimiter);

std::string implode_enclose(const std::vector<std::string> &array, const std::string &delimiter, const std::string &encloser);

#endif //DSYNC_UTIL_H
