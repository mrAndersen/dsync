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

/**
 * Get tables in relative order, first tables with 0 foreign keys, than 1, etc..
 * this is needed to correctly import table data
 * @return
 */
std::vector<std::string> Postgresql::getTables() {
    std::vector<std::string> result;
    std::string sql = R"(
SELECT DISTINCT tablename,
                fkeys
FROM   (SELECT pgt1.tablename,
               Count(*) AS fkeys
        FROM   pg_tables pgt1
               LEFT JOIN pg_class pgc
                      ON pgt1.tablename = pgc.relname
               LEFT JOIN pg_constraint pgct
                      ON pgc.relfilenode = pgct.confrelid
        WHERE  pgt1.schemaname = 'public'
               AND contype = 'f'
        GROUP  BY pgt1.tablename
        UNION
        SELECT pgtl.tablename,
               0
        FROM   pg_tables pgtl
        WHERE  pgtl.schemaname = 'public'
               AND tablename NOT IN (SELECT pgt1.tablename
                                     FROM   pg_tables pgt1
                                            LEFT JOIN pg_class pgc
                                                   ON pgt1.tablename =
                                                      pgc.relname
                                            LEFT JOIN pg_constraint pgct
                                                   ON pgc.relfilenode =
                                                      pgct.confrelid
                                     WHERE  pgt1.schemaname = 'public'
                                            AND contype = 'f'
                                     GROUP  BY pgt1.tablename)) _subq
ORDER  BY fkeys ASC
)";
    auto tables = execute(sql);

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
