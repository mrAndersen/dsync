#ifndef DSYNC_PLATFORM_H
#define DSYNC_PLATFORM_H

#include <string>
#include <vector>

class Platform {
private:
    std::string dsn;

    std::string host;
    std::string dbname;
    int port;

    std::string username;
    std::string password;

    std::string type;
public:
    Platform();

    std::vector<std::string> getTables();

    void connect();

    bool isPostgresql();

    bool isMysql();

    const std::string &getType() const;

    void setType(const std::string &type);

    const std::string &getDsn() const;

    const std::string &getDbname() const;

    void setDbname(const std::string &dbname);

    void setDsn(const std::string &dsn);

    static Platform *createFromDsn(std::string dsn);

    const std::string &getHost() const;

    void setHost(const std::string &host);

    int getPort() const;

    void setPort(int port);

    const std::string &getUsername() const;

    void setUsername(const std::string &username);

    const std::string &getPassword() const;

    void setPassword(const std::string &password);
};


#endif //DSYNC_PLATFORM_H
