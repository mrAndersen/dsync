#include <pqxx/pqxx>
#include <fmt/format.h>
#include <iomanip>
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
    std::string sql = R"(
SELECT table_name
FROM information_schema.tables
WHERE table_schema = 'public'
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

void Postgresql::refreshDatabaseInfo() {
    std::string sql = R"(
SELECT * FROM information_schema.columns
WHERE table_schema = 'public'
    )";
    databaseInfo = execute(sql);
}

bool Postgresql::isTableFieldNullable(const std::string &table, const int &fieldIndex) {
    std::vector<std::vector<std::string>> tableColumns;

    for (auto node:databaseInfo) {
        if (node[2] == table) {
            tableColumns.emplace_back(node);
        }
    }

    return tableColumns[fieldIndex][6] == "YES";
}

std::string Postgresql::implodeRow(
        const std::string &table,
        const std::vector<std::string> &array
) {
    std::string result;
    std::string delimiter = ",";

    for (int i = 0; i < array.size(); ++i) {
        if (array[i].empty()) {
            if (isTableFieldNullable(table, i)) {
                result.append("null");
            } else {
                result.append("''");
            }
        } else {
            auto escaped = array[i];

            std::stringstream ss;
            ss << std::quoted(array[i], '\'', '\'');
            escaped = ss.str();

            result.append(escaped);
        }

        if (i != array.size() - 1) {
            result.append(delimiter);
        }
    }

    return result;
}
