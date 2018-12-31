#include "ExprAST.hpp"
#include "Lexer.hpp"
#include "GlobalVariables.h"
#include "Utility.h"
#include <vector>

std::unique_ptr<ExprAST> parseNumberExpr() {
    auto result = std::make_unique<NumberExprAST>(0);
    getNextToken();
    return result;
}

std::unique_ptr<ExprAST> parseParanExpr() {
    getNextToken();
    
    auto v = parseExpression();
    
    if (v == nullptr) {
        return nullptr;
    }
    
    if (cursorToken != ')') {
        return LogError("expected ')'");
    }
    getNextToken();
    return v;
}

std::unique_ptr<ExprAST> parseExpression() {
    auto lhs = parsePrimary();
    return nullptr;
}

std::unique_ptr<ExprAST> parsePrimary() {
    switch (cursorToken) {
        default:
            return LogError("unknown token when expecting an expression");
        case IDENTIFIER:
            return parseIdentifierExpr();
        case NUMBER:
            return parseNumberExpr();
        case '(':
            return parseParanExpr();
    }
}

std::unique_ptr<ExprAST> parseIdentifierExpr() {
    std::string idName = g_identifierStr;
    
    getNextToken();
    
    if (cursorToken != '(') {
        return std::make_unique<VariableExprAST>(idName);
    }
    
    getNextToken();
    std::vector<std::unique_ptr<ExprAST>> args;
    
    if (cursorToken != ')') {
        while(true) {
            if (auto arg = parseExpression()) {
                args.push_back(std::move(arg));
            } else {
                return nullptr;
            }
            
            if (cursorToken == ')') {
                break;
            }
            
            if (cursorToken != ',') {
                return LogError("Expected ')' or ',' in argument list");
            }
            getNextToken();
        }
    }
    
    getNextToken();
    
    return std::make_unique<CallExprAST>(idName, std::move(args));
    
}

int getNextToken() {
    return cursorToken = getToken();
}
