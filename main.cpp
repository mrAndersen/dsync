#include <iostream>
#include "src/Initializer.h"
#include "src/Util/util.h"
#include <pqxx/pqxx>
#include <thread>
#include "fmt/format.h"
#include "vendor/fmt/src/format.cc"

const int BATCH_SIZE = 1000;

int main(int argc, char *argv[]) {
    Initializer initializer(argc, argv);

    auto source = initializer.getSource();
    auto target = initializer.getTarget();
    auto threads = initializer.getThreads();

//    if (source->getType() != target->getType()) {
//        throw std::runtime_error("Transfer between different platforms not yet supported");
//    }



    auto tables = initializer.getSource()->getTables();
    fmt::print("source has {} tables, starting in {} threads\n", std::to_string(tables.size()),
               std::to_string(threads));

    for (const auto &table:tables) {
        int tableSize = stoi(source->execute(fmt::format("select count(*) from {}", table))[0][0]);

        if (initializer.verbosity == VERBOSITY_DEBUG) {
            fmt::print("[debug] table = {}, size = {}\n", table, tableSize);
        }

        for (int threadIndex = 0; threadIndex < threads; ++threadIndex) {
            std::thread thread([&threadIndex, &tableSize, &table, &threads, &initializer]() {
                auto threadPortion = tableSize / threads;
                auto startingOffset = (threadIndex - 1) * threadPortion;
                auto endOffset = startingOffset + threadPortion;
                bool downloading = true;
                std::string pattern = "select * from {} limit {} offset {}";

                if (initializer.verbosity == VERBOSITY_DEBUG) {
                    fmt::print("[debug] thread = {}, portion = {}, start = {}, end = {}\n", threadIndex, threadPortion,
                               startingOffset, endOffset);
                }

                auto offset = startingOffset;
                while (downloading) {
                    auto sql = fmt::format(pattern, table, BATCH_SIZE, offset);
                    auto buffer = initializer.getSource()->execute(sql);

                    offset += BATCH_SIZE;
                    downloading = buffer.size() == BATCH_SIZE;
                    exit(1);
                }


            });
            thread.detach();
        }


        exit(0);
//        bool downloading = true;
//        int offset = 0;
//
//        std::string pattern = "select * from %s limit %d offset %d";
//
//
//        while (downloading) {
//            auto sql = format(pattern, table.c_str(), BATCH_SIZE, offset);
//            auto buffer = source->execute(sql);
//
//
//            offset += BATCH_SIZE;
//            downloading = buffer.size() == BATCH_SIZE;
//        }


    }


//    try {
//        pqxx::connection C;
//        std::cout << "Connected to " << C.dbname() << std::endl;
//        pqxx::work W(C);
//
//        pqxx::result R = W.exec("SELECT name FROM employee");
//
//        std::cout << "Found " << R.size() << "employees:" << std::endl;
//        for (auto row: R)
//            std::cout << row[0].c_str() << std::endl;
//
//        std::cout << "Doubling all employees' salaries..." << std::endl;
//        W.exec("UPDATE employee SET salary = salary*2");
//
//        std::cout << "Making changes definite: ";
//        W.commit();
//        std::cout << "OK." << std::endl;
//    }
//    catch (const std::exception &e) {
//        std::cerr << e.what() << std::endl;
//        return 1;
//    }



    return 0;
}