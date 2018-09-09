#include "AbstractPlatform.h"
#include <regex>
#include "../Util/util.h"
#include "Postgresql.h"
#include "Mysql.h"

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
        auto platform = new Postgresql96();
        platform->setDsn(dsn);
        platform->setUsername(matches[2].str());
        platform->setPassword(matches[3].str());
        platform->setHost(matches[4].str());
        platform->setPort(std::stoi(matches[5].str()));
        platform->setDbname(matches[6].str());
        platform->connect();

        return platform;
    }

    if (type == "mysql") {
        auto platform = new Mysql();
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





