#ifndef DSYNC_ANALAYZER_H
#define DSYNC_ANALAYZER_H


#include <string>
#include "../Platform/AbstractPlatform.h"

class Analayzer {
protected:
    std::string compareMessage;
public:

    void validatePlatforms(Platform *source, Platform *target);

    const std::string &getCompareMessage() const;
};


#endif //DSYNC_ANALAYZER_H
