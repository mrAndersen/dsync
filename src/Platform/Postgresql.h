#ifndef DSYNC_POSTGRESQL96_H
#define DSYNC_POSTGRESQL96_H

#include "AbstractPlatform.h"
#include <pqxx/connection.hxx>

class Postgresql : public Platform {
private:
    pqxx::connection *connection = nullptr;
public:
    std::vector<std::vector<std::string>> execute(const std::string &sql);

    std::vector<std::string> getTables();

    long getTableSize(const std::string &table) override;

    void refreshDatabaseInfo() override;

    bool isTableFieldNullable(const std::string &table, const int &fieldIndex) override;

    std::string implodeRow(const std::string &table, const std::vector<std::string> &array) override;

    std::string getTableDdl(const std::string &table) override;

    void connect();

    Postgresql();

    virtual ~Postgresql();
};


#endif //DSYNC_POSTGRESQL96_H
