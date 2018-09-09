#include <pqxx/pqxx>
#include "Postgresql.h"

Postgresql96::Postgresql96() {

}

std::vector<std::vector<std::string>> Postgresql96::execute(std::string sql) {
    std::vector<std::vector<std::string>> result;

    pqxx::work work(*connection);
    pqxx::result rawResult = work.exec(sql);

    for (auto row: rawResult) {
        std::vector<std::string> tmp;

        for (auto col: row) {
            tmp.emplace_back(col.c_str());
        }

        result.emplace_back(tmp);
    }


    return result;
}


void Postgresql96::connect() {
    auto dsn = "postgresql://"
               + getUsername() + ":"
               + getPassword() + "@"
               + getHost() + "/"
               + getDbname() + "?port="
               + std::to_string(getPort()) + "&"
               + "connect_timeout=3";
    connection = new pqxx::connection(dsn);
    printf("connected to %s\n", dsn.c_str());
}

std::vector<std::string> Postgresql96::getTables() {
    std::vector<std::string> result;
    auto tables = execute("select tablename from pg_catalog.pg_tables where schemaname = 'public'");

    for (auto row: tables) {
        result.emplace_back(row[0]);
    }

    return result;
}

Postgresql96::~Postgresql96() {
    delete connection;
}
