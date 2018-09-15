#ifndef DSYNC_UTIL_H
#define DSYNC_UTIL_H

#include <string>
#include <vector>
#include "../Platform/AbstractPlatform.h"

const int BATCH_SIZE = 10000;
const int VERBOSITY_SILENT = 0;
const int VERBOSITY_DEBUG = 4;

bool in_vector(const std::string &value, const std::vector<std::string> &array);

std::string implode(const std::vector<std::string> &array, const std::string &delimiter);

long vector_size_bytes(const std::vector<std::string> &array);

long vector_vector_size_bytes(const std::vector<std::vector<std::string>> &array);

#endif //DSYNC_UTIL_H
