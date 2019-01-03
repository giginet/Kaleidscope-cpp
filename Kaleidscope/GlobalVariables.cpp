#include "GlobalVariables.h"
#include <string>
#include <map>
#include "ExprAST.hpp"

std::string g_identifierStr = "";
double g_numVal = 0;
int g_cursorToken = -1;
std::map<char, int> g_binaryOperatorPrecedences = {};
llvm::LLVMContext theContext;
llvm::IRBuilder<> builder(theContext);
std::unique_ptr<llvm::Module> theModule = nullptr;
std::map<std::string, llvm::Value *> namedValues = {};
std::unique_ptr<llvm::legacy::FunctionPassManager> theFPM = nullptr;
std::unique_ptr<llvm::orc::KaleidoscopeJIT> theJIT = nullptr;
std::map<std::string, std::unique_ptr<PrototypeAST>> functionProtos = {};
