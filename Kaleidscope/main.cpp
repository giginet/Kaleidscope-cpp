#include <iostream>
#include "Lexer.hpp"
#include "ExprAST.hpp"
#include "GlobalVariables.h"

int main(int argc, const char * argv[]) {
    std::cout << "Hello, World!\n";
    g_binaryOperatorPrecedences['<'] = 10;
    g_binaryOperatorPrecedences['+'] = 20;
    g_binaryOperatorPrecedences['-'] = 20;
    g_binaryOperatorPrecedences['*'] = 40;
    return 0;
}
