#include <iostream>
#include "src/Initializer.h"
#include "src/Util/util.h"
#include <pqxx/pqxx>
#include <thread>
#include "fmt/format.h"
#include "vendor/fmt/src/format.cc"
#include "src/Util/Timer.h"
#include "src/Util/Analayzer.h"

int main(int argc, char *argv[]) {
    Timer timer;
    Initializer initializer(argc, argv);
    Analayzer analyzer;

    auto sources = initializer.getSources();
    auto targets = initializer.getTargets();
    auto threads = initializer.getThreads();

    auto firstSource = initializer.getFirstSource();
    auto firstTarget = initializer.getFirstTarget();

    analyzer.validatePlatforms(firstSource, firstTarget);
    fmt::print(analyzer.getCompareMessage());

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

        firstTarget->refreshTableInfo(table);
        auto escapedTable = std::string("\"").append(table).append("\"");

        int tableRows = stoi(firstSource->execute(fmt::format("select count(*) from {}", escapedTable))[0][0]);
        float tableSizeBytes = firstSource->getTableSize(table);

        fmt::print_colored(fmt::green, "\tO {}...", table);

        if (initializer.verbosity == VERBOSITY_DEBUG) {
            fmt::print(
                    "processing table = {}, rows = {}, size = {:.2f}Mb\n",
                    table,
                    tableRows,
                    tableSizeBytes / 1024 / 1024
            );
        }

        if (initializer.clearTarget) {

            if (initializer.verbosity == VERBOSITY_DEBUG) {
                fmt::print_colored(
                        fmt::yellow,
                        "clear option passed, target table {} will be truncated\n",
                        table
                );
            }

            firstTarget->execute(
                    fmt::format(
                            "truncate table {} cascade",
                            escapedTable
                    )
            );
        }

        if (firstTarget->isPostgresql()) {
            firstTarget->execute(fmt::format("alter table {} disable trigger all", escapedTable));

            if (initializer.verbosity == VERBOSITY_DEBUG) {
                fmt::print(
                        "[debug] disabled triggers for postgres for table {}\n",
                        table
                );
            }
        }

        auto medianDownloadSpeed = 0.f;
        auto downloadIterations = 0;

        for (int threadIndex = 0; threadIndex < threads; ++threadIndex) {
            std::thread thread([
                                       &threadIndex,
                                       &tableRows,
                                       &table,
                                       &escapedTable,
                                       &threads,
                                       &initializer,
                                       firstTarget,
                                       &medianDownloadSpeed,
                                       &downloadIterations
                               ]() {

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

                    Timer fetchTimer;
                    auto selectSql = fmt::format(pattern, escapedTable, BATCH_SIZE, offset);

                    fetchTimer.start();
                    auto buffer = threadSource->execute(selectSql);
                    fetchTimer.stop();

                    float bufferSizeMbytes = vector_vector_size_bytes(buffer) / 1024.f / 1024.f;
                    float fetchTime = fetchTimer.elapsedSeconds();
                    float speed = (1 / fetchTime * bufferSizeMbytes);
                    medianDownloadSpeed += speed;
                    downloadIterations++;

                    if (initializer.verbosity == VERBOSITY_DEBUG) {
                        fmt::print(
                                "[debug] buffer fetched, size = {}, speed = {:.4f}Mb/s\n",
                                buffer.size(),
                                speed
                        );
                    }

                    if (buffer.empty()) {
                        break;
                    }

                    std::string insertSql = "insert into {} values ";
                    insertSql = fmt::format(insertSql, escapedTable);
                    std::vector<std::string> tmp;

                    for (const auto &row:buffer) {
                        auto rowSql = "(" + firstTarget->implodeRow(table, row) + ")";
                        tmp.emplace_back(rowSql);
                    }

                    insertSql.append(implode(tmp, ","));
                    threadTarget->execute(insertSql);

                    if (initializer.verbosity == VERBOSITY_DEBUG) {
                        fmt::print(
                                "[debug] {} values inserted\n",
                                buffer.size()
                        );
                    }

                    offset += BATCH_SIZE;
                    downloading = buffer.size() == BATCH_SIZE;

                    if (initializer.verbosity == VERBOSITY_DEBUG) {
                        fmt::print(
                                "[debug] offset = {}\n",
                                offset
                        );
                    }
                }
            });
            thread.join();
        }

        if (firstTarget->isPostgresql()) {
            firstTarget->execute(fmt::format("alter table {} enable trigger all", escapedTable));

            if (initializer.verbosity == VERBOSITY_DEBUG) {
                fmt::print(
                        "[debug] enabled triggers for postgres for table {}\n",
                        table
                );
            }
        }

        fmt::print_colored(fmt::green, "\r\t✔ ");
        fmt::print(
                "{} [{:.2f}s] [{} rows] [{:.4f}Mb/s] ",
                table,
                timer.elapsedSeconds(),
                tableRows,
                (medianDownloadSpeed / downloadIterations)
        );

        if (initializer.clearTarget) {
            fmt::print_colored(fmt::yellow, "[T]\n");
        } else {
            fmt::print_colored(fmt::green, "[A]\n");
        }
    }


    return 0;
}