#ifndef Utility_h
#define Utility_h

#include <iostream>
#include "ExprAST.hpp"

std::unique_ptr<ExprAST> LogError(const char *str) {
    fprintf(stderr, "LogError %s\n", str);
    return nullptr;
}

std::unique_ptr<PrototypeAST> LogErrorP(const char *string) {
    LogError(str);
    return nullptr;
}

#endif /* Utility_h */
