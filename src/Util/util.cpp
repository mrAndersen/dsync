#include <algorithm>
#include <cstdarg>
#include <string>
#include <vector>
#include <memory>
#include <cstring>
#include <sstream>
#include <iomanip>
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

std::string implode_enclose_nulls(
        const std::vector<std::string> &array,
        const std::string &delimiter,
        const std::string &platform
) {
    std::string result;

    for (int i = 0; i < array.size(); ++i) {
        if (array[i].empty()) {
            result.append("null");
        } else {
            auto escaped = array[i];

            if (platform == "pgsql") {
                std::stringstream ss;
                ss << std::quoted(array[i], '\'', '\'');
                escaped = ss.str();
            }

            result.append(escaped);
        }

        if (i != array.size() - 1) {
            result.append(delimiter);
        }
    }

    return result;
}

long vector_size_bytes(const std::vector<std::string> &array) {
    long size = 0;
    size = sizeof(std::vector<std::string>) + (sizeof(std::string) * array.size());

    return size;
}

long vector_vector_size_bytes(const std::vector<std::vector<std::string>> &array) {
    long size = 0;

    for (const auto &row:array) {
        size += vector_size_bytes(row);
    }

    return size;
}
