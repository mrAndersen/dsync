#ifndef DSYNC_INITIALIZER_H
#define DSYNC_INITIALIZER_H

#include <vector>
#include <string>
#include "Platform/AbstractPlatform.h"
#include "Util/util.h"

class Initializer {
private:
    int threads = 1;

    std::vector<Platform*> sources;
    std::vector<Platform*> targets;

    std::vector<std::string> arguments;
public:
    int verbosity = VERBOSITY_SILENT;

    bool clearTarget = false;

    Initializer(int argc, char *argv[]);

    Platform* getFirstSource();

    Platform* getFirstTarget();

    const std::vector<Platform *> &getSources() const;

    void setSources(const std::vector<Platform *> &sources);

    const std::vector<Platform *> &getTargets() const;

    void setTargets(const std::vector<Platform *> &targets);

    void printHelpMessage();

    virtual ~Initializer();

    int getThreads() const;

    void setThreads(int threads);
};


#endif //DSYNC_INITIALIZER_H
