#include <pqxx/pqxx>
#include <fmt/format.h>
#include "Postgresql.h"

Postgresql::Postgresql() {

}

std::vector<std::vector<std::string>> Postgresql::execute(const std::string &sql) {
    if (connection == nullptr) {
        throw std::runtime_error(
                fmt::format("connection to {} has not been established call connect() first", getDsn()));
    }

    std::vector<std::vector<std::string>> result;

    pqxx::work work(*connection);
    pqxx::result rawResult = work.exec(sql);
    work.commit();

    for (auto row: rawResult) {
        std::vector<std::string> tmp;

        for (auto col: row) {
            tmp.emplace_back(col.c_str());
        }

        result.emplace_back(tmp);
    }

    return result;
}


void Postgresql::connect() {
    auto dsn = fmt::format(
            "postgresql://{}:{}@{}/{}?port={}&connect_timeout=3",
            getUsername(),
            getPassword(),
            getHost(),
            getDbname(),
            getPort()
    );

    connection = new pqxx::connection(dsn);
}

std::vector<std::string> Postgresql::getTables() {
    std::vector<std::string> result;
    auto tables = execute("select tablename from pg_catalog.pg_tables where schemaname = 'public'");

    for (auto row: tables) {
        result.emplace_back(row[0]);
    }

    return result;
}

Postgresql::~Postgresql() {
    delete connection;
}

long Postgresql::getTableSize(const std::string &table) {
    auto size = stoi(execute(fmt::format("select pg_total_relation_size('{}')", table))[0][0]);
    return size;
}
