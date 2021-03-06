#ifndef ExprAST_hpp
#define ExprAST_hpp

#include <string>
#include <vector>
#include <map>
#include "llvm.h"
#include "Lexer.hpp"
#include "GlobalVariables.h"

extern int getNextToken();

class ExprAST {
public:
    virtual ~ExprAST() { }
    virtual llvm::Value *codegen() = 0;
};

class NumberExprAST: public ExprAST {
    double value;
    
public:
    NumberExprAST(double value)
    : value(value) { }
    llvm::Value *codegen() override;
};

class VariableExprAST: public ExprAST {
    std::string name;
    
public:
    VariableExprAST(const std::string &name)
    : name(name) { }
    llvm::Value *codegen() override;
};

class BinaryExprAST: public ExprAST {
    char op;
    std::unique_ptr<ExprAST> lhs, rhs;
    
public:
    BinaryExprAST(char op, std::unique_ptr<ExprAST> lhs, std::unique_ptr<ExprAST> rhs)
    : op(op)
    , lhs(std::move(lhs))
    , rhs(std::move(rhs)) { }
    llvm::Value *codegen() override;
};

class CallExprAST: public ExprAST {
    std::string callee;
    
public:
    CallExprAST(const std::string &callee, std::vector<std::unique_ptr<ExprAST>> args)
    : callee(callee)
    , args(std::move(args)) { }
    llvm::Value *codegen() override;
    std::vector<std::unique_ptr<ExprAST>> args;
};


class IfExprAST: public ExprAST {
    std::unique_ptr<ExprAST> conditionAST;
    std::unique_ptr<ExprAST> thenAST;
    std::unique_ptr<ExprAST> elseAST;
    
public:
    IfExprAST(std::unique_ptr<ExprAST> condition, std::unique_ptr<ExprAST> then, std::unique_ptr<ExprAST> elseAST)
    : conditionAST(std::move(condition))
    , thenAST(std::move(then))
    , elseAST(std::move(elseAST)) { }
    
    llvm::Value *codegen() override;
};

class PrototypeAST {
    std::string name;
    
public:
    PrototypeAST(const std::string &name, std::vector<std::string> args)
    : name(name)
    , args(std::move(args)) { }
    
    const std::string &getName() const { return name; };
    llvm::Function *codegen();
    std::vector<std::string> args;

};

class FunctionAST {
    std::unique_ptr<PrototypeAST> prototype;
    std::unique_ptr<ExprAST> body;
    
public:
    FunctionAST(std::unique_ptr<PrototypeAST> prototype, std::unique_ptr<ExprAST> body)
    : prototype(std::move(prototype))
    , body(std::move(body)) { }
    
    llvm::Function *codegen();
};

extern std::unique_ptr<ExprAST> parseNumberExpr();
extern std::unique_ptr<ExprAST> parseParenExpr();
extern std::unique_ptr<ExprAST> parseExpression();
extern std::unique_ptr<ExprAST> parsePrimary();
extern std::unique_ptr<ExprAST> parseIdentifierExpr();
extern std::unique_ptr<ExprAST> parseIfExpr();
extern std::unique_ptr<ExprAST> parseBinaryOperatorRHS(int exprPrecedence, std::unique_ptr<ExprAST> lhs);
extern std::unique_ptr<PrototypeAST> parsePrototype();
extern std::unique_ptr<FunctionAST> parseDefinition();
extern std::unique_ptr<PrototypeAST> parseExtern();
extern std::unique_ptr<FunctionAST> parseTopLevelExpr();
extern int getTokenPrecedence();

#endif /* ExprAST_hpp */
