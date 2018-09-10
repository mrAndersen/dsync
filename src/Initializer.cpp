#include <stdexcept>
#include "Initializer.h"
#include "Util/util.h"

Initializer::Initializer(int argc, char **argv) {
    if (argc < 3) {
        printHelpMessage();
    }

    for (int i = 0; i < argc; ++i) {
        arguments.emplace_back(argv[i]);
    }

    for (const auto &argument:arguments) {
        if (argument.find("--help") != -1) {
            printHelpMessage();
        }

        if (argument.find("-vvv") != -1) {
            verbosity = VERBOSITY_DEBUG;
        }

        if (argument.find("--clear") != -1) {
            clearTarget = true;
        }
    }

    for (int j = 0; j < threads; ++j) {
        sources.emplace_back(Platform::createFromDsn(arguments[1]));
        targets.emplace_back(Platform::createFromDsn(arguments[2]));
    }
}

void Initializer::printHelpMessage() {
    printf(
            "Usage: dsync [SOURCE_DSN] [TARGET_DSN] options\n\n"
            "Syncs two databases between remote hosts from source to target\n"
            "dsn syntax is <type>://<username>:<password>@<host>:<port>/<dbname>\n"
            "for example pgsql://postgres:atm_root@localhost:54321/atm\n\n"
    );
    printf(
            "  --clear, clear target database\n"
            "  --help, help message\n"
            "  --vvv, verbose\n"
    );
    printf(
            "\n"
    );
    exit(0);
}

Initializer::~Initializer() {
    for (auto platform:sources) {
        delete platform;
    }

    for (auto platfrom:targets) {
        delete platfrom;
    }
}

int Initializer::getThreads() const {
    return threads;
}

void Initializer::setThreads(int threads) {
    Initializer::threads = threads;
}

const std::vector<Platform *> &Initializer::getSources() const {
    return sources;
}

void Initializer::setSources(const std::vector<Platform *> &sources) {
    Initializer::sources = sources;
}

const std::vector<Platform *> &Initializer::getTargets() const {
    return targets;
}

void Initializer::setTargets(const std::vector<Platform *> &targets) {
    Initializer::targets = targets;
}

Platform *Initializer::getFirstTarget() {
    return targets[0];
}

Platform *Initializer::getFirstSource() {
    return sources[0];
}
