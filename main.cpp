#include <iostream>
#include "src/Initializer.h"
#include "src/Util/util.h"
#include <pqxx/pqxx>
#include <thread>
#include "fmt/format.h"
#include "vendor/fmt/src/format.cc"
#include "src/Util/Timer.h"

int main(int argc, char *argv[]) {
    Timer timer;
    Initializer initializer(argc, argv);

    auto sources = initializer.getSources();
    auto targets = initializer.getTargets();
    auto threads = initializer.getThreads();

    auto firstSource = initializer.getFirstSource();
    auto firstTarget = initializer.getFirstTarget();

    for (auto platform:sources) {
        platform->connect();

        if (initializer.verbosity == VERBOSITY_DEBUG) {
            fmt::print("[debug] connected to source {}\n", platform->getDsn());
        }
    }

    for (auto platform:targets) {
        platform->connect();

        if (initializer.verbosity == VERBOSITY_DEBUG) {
            fmt::print("[debug] connected to target {}\n", platform->getDsn());
        }
    }

    fmt::print_colored(
            fmt::green,
            "{} connections established via {} -> {}\n",
            threads * 2,
            firstSource->getType(),
            firstTarget->getType()
    );

    auto sourceTables = firstSource->getTables();

    fmt::print(
            "source has {} tables, starting in {} threads\n",
            std::to_string(sourceTables.size()),
            std::to_string(threads)
    );

    for (const auto &table:sourceTables) {
        timer.restart();

        int tableRows = stoi(firstSource->execute(fmt::format("select count(*) from {}", table))[0][0]);
        float tableSizeBytes = firstSource->getTableSize(table);

        fmt::print(
                "processing table = {}, rows = {}, size = {:02.2f}Mb\n",
                table,
                tableRows,
                tableSizeBytes / 1024 / 1024
        );

        if (initializer.clearTarget) {
            fmt::print_colored(
                    fmt::yellow,
                    "clear option passed, target table {} will be truncated\n",
                    table
            );

            firstTarget->execute(
                    fmt::format(
                            "truncate table {}",
                            table
                    )
            );
        }

        for (int threadIndex = 0; threadIndex < threads; ++threadIndex) {
            std::thread thread([&threadIndex, &tableRows, &table, &threads, &initializer]() {

                auto threadPortion = std::floor(tableRows / threads);
                auto startingOffset = threadIndex * threadPortion;
                auto endOffset = startingOffset + threadPortion;
                bool downloading = true;

                auto threadSource = initializer.getSources()[threadIndex];
                auto threadTarget = initializer.getTargets()[threadIndex];
                std::string pattern = "select * from {} limit {} offset {}";

                if (initializer.verbosity == VERBOSITY_DEBUG) {
                    fmt::print(
                            "[debug] thread = {}, portion = {}, start = {}, end = {}\n",
                            threadIndex,
                            threadPortion,
                            startingOffset,
                            endOffset
                    );
                }

                auto offset = startingOffset;

                while (downloading) {
                    std::string sql = "";

                    sql = fmt::format(pattern, table, BATCH_SIZE, offset);
                    auto buffer = threadSource->execute(sql);

                    if (initializer.verbosity == VERBOSITY_DEBUG) {
                        fmt::print(
                                "[debug] buffer fetched, size = {}\n",
                                buffer.size()
                        );
                    }

                    std::string insertSql = "insert into {} values ";
                    std::vector<std::string> tmp;

                    for (const auto &row:buffer) {
                        auto rowSql = "(" + implode_enclose(row, ",", "'") + ")";
                        tmp.emplace_back(rowSql);
                    }

                    insertSql.append(implode(tmp, ","));
                    sql = fmt::format(insertSql, table);
                    threadTarget->execute(sql);

                    if (initializer.verbosity == VERBOSITY_DEBUG) {
                        fmt::print(
                                "[debug] {} values inserted\n",
                                buffer.size()
                        );
                    }

                    offset += BATCH_SIZE;
                    downloading = buffer.size() == BATCH_SIZE;
                }
            });
            thread.join();
        }

        fmt::print(
                "table {} done in {:02.2f}s\n",
                table,
                timer.elapsedSeconds()
        );
    }


    return 0;
}