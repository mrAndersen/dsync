#include <stdexcept>
#include "Initializer.h"

Initializer::Initializer(int argc, char **argv) {
    if (argc < 3) {
        printHelpMessage();
    }

    for (int i = 0; i < argc; ++i) {
        arguments.emplace_back(argv[i]);
    }

    for (const auto &argument:arguments) {
        if (argument.find("--help") != std::string::npos) {
            printHelpMessage();
        }
    }

    source = Platform::createFromDsn(arguments[1]);
//    target = Platform::createFromDsn(arguments[2]);
}

Platform *Initializer::getSource() const {
    return source;
}

Platform *Initializer::getTarget() const {
    return target;
}

void Initializer::printHelpMessage() {
    printf(
            "Usage: dsync [SOURCE_DSN] [TARGET_DSN] options\n"
            "Syncs two databases between remote hosts from source to target\n\n"
    );
    printf(
            "\tdsn syntax - <type>://<username>:<password>@<host>:<port>/<dbname>\n"
            "\tfor example pgsql://postgres:atm_root@localhost:54321/atm\n"
    );
    exit(0);
}

Initializer::~Initializer() {
    delete source;
    delete target;
}

int Initializer::getThreads() const {
    return threads;
}

void Initializer::setThreads(int threads) {
    Initializer::threads = threads;
}
