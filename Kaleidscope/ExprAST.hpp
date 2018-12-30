#ifndef ExprAST_hpp
#define ExprAST_hpp

#include <string>
#include <vector>
#include "Lexer.hpp"

extern int cursorToken;

static inline int getNextToken() {
    return cursorToken = getToken();
}

class ExprAST {
public:
    virtual ~ExprAST() { }
};

class NumberExprAST: public ExprAST {
    double value;
    
public:
    NumberExprAST(double value)
    : value(value) { }
};

class VariableExprAST: public ExprAST {
    std::string name;
    
public:
    VariableExprAST(const std::string &name)
    : name(name) { }
};

class BinaryExprAST: public ExprAST {
    char op;
    std::unique_ptr<ExprAST> lhs, rhs;
    
public:
    BinaryExprAST(char op, std::unique_ptr<ExprAST> lhs, std::unique_ptr<ExprAST> rhs)
    : op(op)
    , lhs(std::move(lhs))
    , rhs(std::move(rhs)) { }
};

class CallExprAST: public ExprAST {
    std::string callee;
    std::vector<std::unique_ptr<ExprAST>> args;
    
public:
    CallExprAST(const std::string &callee, std::vector<std::unique_ptr<ExprAST>> args)
    : callee(callee)
    , args(std::move(args)) { }
};

class PrototypeAST {
    std::string name;
    std::vector<std::string> args;
    
public:
    PrototypeAST(const std::string &name, std::vector<std::string> args)
    : name(name)
    , args(std::move(args)) { }
    
    const std::string &getName() const { return name; };
};

class FunctionAST {
    std::unique_ptr<PrototypeAST> prototype;
    std::unique_ptr<ExprAST> body;
    
public:
    FunctionAST(std::unique_ptr<PrototypeAST> prototype, std::unique_ptr<ExprAST> body)
    : prototype(std::move(prototype))
    , body(std::move(body)) { }
};

#endif /* ExprAST_hpp */
