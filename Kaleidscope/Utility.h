#ifndef Utility_h
#define Utility_h

#include <iostream>
#include "ExprAST.hpp"

template<class T>
std::unique_ptr<T> logError(const char *string) {
    fprintf(stderr, "LogError %s\n", string);
    return nullptr;
}

#endif /* Utility_h */
