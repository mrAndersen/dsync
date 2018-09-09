#ifndef DSYNC_INITIALIZER_H
#define DSYNC_INITIALIZER_H

#include <vector>
#include <string>
#include "Platform/AbstractPlatform.h"

class Initializer {
private:
    int threads = 4;

    Platform *source = nullptr;
    Platform *target = nullptr;

    std::vector<std::string> arguments;
public:
    Initializer(int argc, char *argv[]);

    Platform *getSource() const;

    Platform *getTarget() const;

    void printHelpMessage();

    virtual ~Initializer();

    int getThreads() const;

    void setThreads(int threads);
};


#endif //DSYNC_INITIALIZER_H
