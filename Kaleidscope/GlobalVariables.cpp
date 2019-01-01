#include "GlobalVariables.h"
#include <string>
#include <map>

std::string g_identifierStr = "";
double g_numVal = 0;
int g_cursorToken = -1;
std::map<char, int> g_binaryOperatorPrecedences = {};
llvm::LLVMContext theContext;
llvm::IRBuilder<> builder(theContext);
std::unique_ptr<llvm::Module> theModule = 0;
std::map<std::string, llvm::Value *> namedValues = {};
