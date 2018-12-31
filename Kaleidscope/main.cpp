#include <iostream>
#include "Lexer.hpp"
#include "ExprAST.hpp"
#include "GlobalVariables.h"

static void handleDefinition() {
    if (parseDefinition()) {
        fprintf(stderr, "Parsed a function definition.\n");
    } else {
        // Skip token for error recovery.
        getNextToken();
    }
}

static void handleExtern() {
    if (parseExtern()) {
        fprintf(stderr, "Parsed an extern\n");
    } else {
        // Skip token for error recovery.
        getNextToken();
    }
}

static void handleTopLevelExpression() {
    // Evaluate a top-level expression into an anonymous function.
    if (parseTopLevelExpr()) {
        fprintf(stderr, "Parsed a top-level expr\n");
    } else {
        // Skip token for error recovery.
        getNextToken();
    }
}


static void runMainLoop() {
    while (true) {
        fprintf(stderr, "ready> ");
        
        switch (cursorToken) {
            case END_OF_FILE:
                return;
            case ';':
                getNextToken();
                break;
            case DEF:
                handleDefinition();
                break;
            case EXTERN:
                handleExtern();
                break;
            default:
                handleTopLevelExpression();
                break;
        }
    }
}

int main(int argc, const char * argv[]) {
    std::cout << "Hello, World!\n";
    g_binaryOperatorPrecedences['<'] = 10;
    g_binaryOperatorPrecedences['+'] = 20;
    g_binaryOperatorPrecedences['-'] = 20;
    g_binaryOperatorPrecedences['*'] = 40;
    
    // Prime the first token.
    fprintf(stderr, "ready> ");
    getNextToken();
    
    // Run the main "interpreter loop" now.
    runMainLoop();
    
    return 0;
}
