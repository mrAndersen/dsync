#ifndef DSYNC_POSTGRESQL96_H
#define DSYNC_POSTGRESQL96_H

#include "AbstractPlatform.h"
#include <pqxx/connection.hxx>

class Postgresql96 : public Platform {
private:
    pqxx::connect_async *connection;
public:
    std::vector<std::vector<std::string>> execute(std::string sql);

    std::vector<std::string> getTables();

    void connect();

    Postgresql96();

    virtual ~Postgresql96();
};


#endif //DSYNC_POSTGRESQL96_H
