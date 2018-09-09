#ifndef DSYNC_INITIALIZER_H
#define DSYNC_INITIALIZER_H

#include <vector>
#include <string>
#include "Platform/AbstractPlatform.h"

class Initializer {
private:
    Platform *source = nullptr;
    Platform *target = nullptr;

    std::vector<std::string> arguments;
public:
    Initializer(int argc, char *argv[]);

    Platform *getSource() const;

    Platform *getTarget() const;

    void printHelpMessage();
};


#endif //DSYNC_INITIALIZER_H
