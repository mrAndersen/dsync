#include "Postgresql.h"

Postgresql96::Postgresql96() {

}

void Postgresql96::execute(std::string sql) {


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
    return std::vector<std::string>();
}
