#include "AbstractPlatform.h"
#include <regex>
#include "Postgresql.h"
#include "Mysql.h"
#include "../Util/util.h"

Platform::Platform() {

}

const std::string &Platform::getHost() const {
    return host;
}

void Platform::setHost(const std::string &host) {
    Platform::host = host;
}

int Platform::getPort() const {
    return port;
}

void Platform::setPort(int port) {
    Platform::port = port;
}

const std::string &Platform::getUsername() const {
    return username;
}

void Platform::setUsername(const std::string &username) {
    Platform::username = username;
}

const std::string &Platform::getPassword() const {
    return password;
}

void Platform::setPassword(const std::string &password) {
    Platform::password = password;
}

const std::string &Platform::getDsn() const {
    return dsn;
}

void Platform::setDsn(const std::string &dsn) {
    Platform::dsn = dsn;
}

const std::string &Platform::getDbname() const {
    return dbname;
}

void Platform::setDbname(const std::string &dbname) {
    Platform::dbname = dbname;
}

/**
 *
 * @param dsn
 * @return
 */
Platform *Platform::createFromDsn(std::string dsn) {
    std::vector<std::string> platformTypes = {"pgsql", "mysql"};

    std::smatch matches;
    std::regex dsnRegex(
            R"(^(.*):\/\/(.*?):(.*)\@(.*):(.*)\/(.*)$)",
            std::regex_constants::ECMAScript | std::regex_constants::icase
    );

    std::regex_search(dsn, matches, dsnRegex);
    auto type = matches[1].str();

    if (
            matches[2].str().empty() ||
            matches[3].str().empty() ||
            matches[4].str().empty() ||
            matches[5].str().empty() ||
            matches[6].str().empty()
            ) {
        throw std::runtime_error("Specify correct dsn, all parameters must be set");
    }

    if (!in_vector(type, platformTypes)) {
        throw std::runtime_error("Unsupported platform type " + type);
    }

    type = matches[1].str();

    if (type == "pgsql") {
        auto platform = new Postgresql();
        platform->setType(type);
        platform->setDsn(dsn);
        platform->setUsername(matches[2].str());
        platform->setPassword(matches[3].str());
        platform->setHost(matches[4].str());
        platform->setPort(std::stoi(matches[5].str()));
        platform->setDbname(matches[6].str());

        return platform;
    }

    if (type == "mysql") {
        auto platform = new Mysql();
        platform->setType(type);
        platform->setDsn(dsn);
        platform->setUsername(matches[2].str());
        platform->setPassword(matches[3].str());
        platform->setHost(matches[4].str());
        platform->setPort(std::stoi(matches[5].str()));
        platform->setDbname(matches[6].str());

        return platform;
    }

    return nullptr;
}

bool Platform::isPostgresql() {
    return type == "pgsql";
}

bool Platform::isMysql() {
    return type == "mysql";
}

const std::string &Platform::getType() const {
    return type;
}

void Platform::setType(const std::string &type) {
    Platform::type = type;
}

std::vector<std::string> Platform::getTables() {
    return std::vector<std::string>();
}

void Platform::connect() {
    throw std::runtime_error("connect() called from abstract platform");
}

Platform::~Platform() {

}

std::vector<std::vector<std::string>> Platform::execute(const std::string &sql) {
    throw std::runtime_error("execute() called from abstract platform");
}

long int Platform::getTableSize(const std::string &) {
    throw std::runtime_error("getTableSize() called from abstract platform");
}

void Platform::refreshDatabaseInfo() {
    throw std::runtime_error("refreshDatabaseInfo() called from abstract platform");
}

bool Platform::isTableFieldNullable(const std::string &table, const int &fieldIndex) {
    throw std::runtime_error("isTableFieldNullable() called from abstract platform");
}

std::string Platform::implodeRow(const std::string &table, const std::vector<std::string> &array) {
    throw std::runtime_error("implodeRow() called from abstract platform");
}

const std::vector<std::vector<std::string>> &Platform::getDatabaseInfo() const {
    return databaseInfo;
}

std::string Platform::getTableDdl(const std::string &table) {
    throw std::runtime_error("getTableDdl() called from abstract platform");
}




