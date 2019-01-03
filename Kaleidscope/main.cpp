#include <iostream>
#include "Lexer.hpp"
#include "ExprAST.hpp"
#include "GlobalVariables.h"
#include "KaleidscopeJIT.h"


static void initializeModuleAndPassManager() {
    theModule = std::make_unique<llvm::Module>("my cool jit", theContext);
    theModule->setDataLayout(theJIT->getTargetMachine().createDataLayout());
    
    // why legacy?
    theFPM = std::make_unique<llvm::legacy::FunctionPassManager>(theModule.get());
    
    theFPM->add(llvm::createInstructionCombiningPass());
    theFPM->add(llvm::createReassociatePass());
    theFPM->add(llvm::createGVNPass());
    theFPM->add(llvm::createCFGSimplificationPass());
    
    theFPM->doInitialization();
}

static void handleDefinition() {
    if (auto FnAST = parseDefinition()) {
        if (auto *FnIR = FnAST->codegen()) {
            fprintf(stderr, "Read function definition:");
            FnIR->print(llvm::errs());
            fprintf(stderr, "\n");
            theJIT->addModule(std::move(theModule));
            initializeModuleAndPassManager();
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
            functionProtos[ProtoAST->getName()] = std::move(ProtoAST);
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
            
            auto h = theJIT->addModule(std::move(theModule));
            initializeModuleAndPassManager();
            
            auto exprSymbol = theJIT->findSymbol("__anon_expr");
            assert(exprSymbol && "Function not found");
            
            double (*FP)() = (double (*)())(intptr_t)cantFail(exprSymbol.getAddress());
            fprintf(stderr, "Evaluated to %f\n", FP());
            
            theJIT->removeModule(h);
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
    
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::InitializeNativeTargetAsmParser();
    
    g_binaryOperatorPrecedences['<'] = 10;
    g_binaryOperatorPrecedences['+'] = 20;
    g_binaryOperatorPrecedences['-'] = 20;
    g_binaryOperatorPrecedences['*'] = 40;
    
    // Prime the first token.
    fprintf(stderr, "ready> ");
    getNextToken();
    
    theJIT = std::make_unique<llvm::orc::KaleidoscopeJIT>();
    
    initializeModuleAndPassManager();
    
    // Run the main "interpreter loop" now.
    runMainLoop();
    
    theModule->print(llvm::errs(), nullptr);
    
    return 0;
}
