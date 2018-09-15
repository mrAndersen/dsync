#ifndef DSYNC_PLATFORM_H
#define DSYNC_PLATFORM_H

#include <string>
#include <vector>

class Platform {
protected:
    std::string dsn;

    std::string host;
    std::string dbname;
    int port;

    std::string username;
    std::string password;

    std::string type;
    std::vector<std::vector<std::string>> databaseInfo;
public:
    Platform();

    virtual ~Platform();

    const std::vector<std::vector<std::string>> &getDatabaseInfo() const;

    virtual std::vector<std::string> getTables();

    virtual void connect();

    virtual long int getTableSize(const std::string &table);

    virtual void refreshDatabaseInfo();

    virtual std::vector<std::vector<std::string>> execute(const std::string &sql);

    virtual bool isTableFieldNullable(const std::string &table, const int &fieldIndex);

    virtual std::string implodeRow(const std::string &table, const std::vector<std::string> &array);

    virtual std::string getTableDdl(const std::string &table);

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
