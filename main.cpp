#include <iostream>
#include "src/Initializer.h"
#include <pqxx/pqxx>


int main(int argc, char *argv[]) {
    Initializer initializer(argc, argv);

    auto source = initializer.getSource();
    auto target = initializer.getTarget();

//    if (source->getType() != target->getType()) {
//        throw std::runtime_error("Transfer between different platforms not yet supported");
//    }

    auto tables = initializer.getSource()->getTables();


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

    std::cout << "Hello, World!" << std::endl;
    return 0;
}