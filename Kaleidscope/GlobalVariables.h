#ifndef GlobalVariables_h
#define GlobalVariables_h

#include <map>
#include "llvm.h"
#include "KaleidscopeJIT.h"

class PrototypeAST;

extern std::string g_identifierStr;
extern double g_numVal;
extern int g_cursorToken;
extern std::map<char, int> g_binaryOperatorPrecedences;
extern llvm::LLVMContext theContext;
extern llvm::IRBuilder<> builder;
extern std::unique_ptr<llvm::Module> theModule;
extern std::map<std::string, llvm::Value *> namedValues;
extern std::unique_ptr<llvm::legacy::FunctionPassManager> theFPM;
extern std::unique_ptr<llvm::orc::KaleidoscopeJIT> theJIT;
extern std::map<std::string, std::unique_ptr<PrototypeAST>> functionProtos;

#endif /* GlobalVariables_h */
