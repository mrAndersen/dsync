#ifndef DSYNC_POSTGRESQL96_H
#define DSYNC_POSTGRESQL96_H

#include "AbstractPlatform.h"
#include <pqxx/connection.hxx>

class Postgresql96 : public Platform {
private:
    pqxx::connection *connection;
public:
    void execute(std::string sql);

    std::vector<std::string> getTables();

    void connect();

    Postgresql96();
};


#endif //DSYNC_POSTGRESQL96_H
