#ifndef Utility_h
#define Utility_h

#include <iostream>
#include "ExprAST.hpp"

std::unique_ptr<ExprAST> LogError(const char *string) {
    fprintf(stderr, "LogError %s\n", string);
    return nullptr;
}

std::unique_ptr<PrototypeAST> LogErrorP(const char *string) {
    LogError(string);
    return nullptr;
}

#endif /* Utility_h */
