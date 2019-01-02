#include <iostream>
#include "Lexer.hpp"
#include "ExprAST.hpp"
#include "GlobalVariables.h"

static void handleDefinition() {
    if (auto FnAST = parseDefinition()) {
        if (auto *FnIR = FnAST->codegen()) {
            fprintf(stderr, "Read function definition:");
            FnIR->print(llvm::errs());
            fprintf(stderr, "\n");
        }
    } else {
        // Skip token for error recovery.
        getNextToken();
    }
}

static void handleExtern() {
    if (auto ProtoAST = parseExtern()) {
        if (auto *FnIR = ProtoAST->codegen()) {
            fprintf(stderr, "Read extern: ");
            FnIR->print(llvm::errs());
            fprintf(stderr, "\n");
        }
    } else {
        // Skip token for error recovery.
        getNextToken();
    }
}

static void handleTopLevelExpression() {
    // Evaluate a top-level expression into an anonymous function.
    if (auto FnAST = parseTopLevelExpr()) {
        if (auto *FnIR = FnAST->codegen()) {
            fprintf(stderr, "Read top-level expression:");
            FnIR->print(llvm::errs());
            fprintf(stderr, "\n");
        }
    } else {
        // Skip token for error recovery.
        getNextToken();
    }
}

static void runMainLoop() {
    while (true) {
        fprintf(stderr, "ready> ");
        switch (g_cursorToken) {
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
    
    theModule = std::make_unique<llvm::Module>("my cool jit", theContext);
    
    // why legacy?
    theFPM = std::make_unique<llvm::legacy::FunctionPassManager>(theModule.get());
    
    theFPM->add(llvm::createInstructionCombiningPass());
    theFPM->add(llvm::createReassociatePass());
    theFPM->add(llvm::createGVNPass());
    theFPM->add(llvm::createCFGSimplificationPass());
    
    theFPM->doInitialization();
    
    // Run the main "interpreter loop" now.
    runMainLoop();
    
    theModule->print(llvm::errs(), nullptr);
    
    return 0;
}
