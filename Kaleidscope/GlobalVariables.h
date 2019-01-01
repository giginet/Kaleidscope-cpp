#ifndef GlobalVariables_h
#define GlobalVariables_h

#include <map>
#include "llvm.h"

extern std::string g_identifierStr;
extern double g_numVal;
extern int g_cursorToken;
extern std::map<char, int> g_binaryOperatorPrecedences;
extern llvm::LLVMContext theContext;
extern llvm::IRBuilder<> builder;
extern std::unique_ptr<llvm::Module> theModule;
extern std::map<std::string, llvm::Value *> namedValues;

#endif /* GlobalVariables_h */
