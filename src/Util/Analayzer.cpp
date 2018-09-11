#include "Analayzer.h"

const std::string &Analayzer::getCompareMessage() const {
    return compareMessage;
}

void Analayzer::validatePlatforms(Platform *source, Platform *target) {
    auto sourceTables = source->getTables();
    auto targetTables = target->getTables();


    for (const auto &sourceTable:sourceTables) {

        for (const auto &targetTable:targetTables) {

            if (sourceTable == targetTable) {
                auto sourceFields = source->getDatabaseInfo();


            }
        }
    }


}
