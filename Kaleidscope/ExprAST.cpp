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
    if (lhs == nullptr) {
        return nullptr;
    }
    return parseBinaryOperatorRHS(0, std::move(lhs));
}

std::unique_ptr<ExprAST> parseBinaryOperatorRHS(int exprPrecedence, std::unique_ptr<ExprAST> lhs) {
    while (true) {
        auto tokenPrecedence = getTokenPrecedence();
        
        if (tokenPrecedence < exprPrecedence) {
            return lhs;
        }
        
        auto binaryOperator = cursorToken;
        getNextToken();
        
        auto rhs = parsePrimary();
        if (rhs == nullptr) {
            return nullptr;
        }
        
        auto nextPrecedence = getTokenPrecedence();
        if (tokenPrecedence < nextPrecedence) {
            rhs = parseBinaryOperatorRHS(tokenPrecedence + 1, std::move(rhs));
            if (rhs == nullptr) {
                return nullptr;
            }
        }
        
        lhs = std::make_unique<BinaryExprAST>(binaryOperator, std::move(lhs), std::move(rhs));
    }
}

std::unique_ptr<ExprAST> parsePrimary() {
    printf("%c %d\n", cursorToken, cursorToken);
    
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

std::unique_ptr<PrototypeAST> parsePrototype() {
    if (cursorToken != IDENTIFIER) {
        return LogErrorP("Expected function name in prototype");
    }
    
    auto functionName = g_identifierStr;
    getNextToken();
    
    if (cursorToken != '(') {
        return LogErrorP("Expected '(' in prototype.");
    }
    
    std::vector<std::string> argumentNames;
    while (getNextToken() == IDENTIFIER) {
        argumentNames.push_back(g_identifierStr);
    }
    
    if (cursorToken != ')') {
        return LogErrorP("Expected ')' in prototype");
    }
    
    getNextToken();
    
    return std::make_unique<PrototypeAST>(functionName, std::move(argumentNames));
}

std::unique_ptr<FunctionAST> parseDefinition() {
    getNextToken();
    auto prototype = parsePrototype();
    
    if (prototype == nullptr) {
        return nullptr;
    }
    
    if (auto e = parseExpression()) {
        return std::make_unique<FunctionAST>(std::move(prototype), std::move(e));
    }
    return nullptr;
}

std::unique_ptr<PrototypeAST> parseExtern() {
    getNextToken();
    return parsePrototype();
}

std::unique_ptr<FunctionAST> parseTopLevelExpr() {
    if (auto e = parseExpression()) {
        auto prototype = std::make_unique<PrototypeAST>("", std::vector<std::string>());
        return std::make_unique<FunctionAST>(std::move(prototype), std::move(e));
    }
    return nullptr;
}

int getNextToken() {
    return cursorToken = getToken();
}

int getTokenPrecedence() {
    if (!isascii(cursorToken)) {
        return -1;
    }
    
    int tokenPrecedence = g_binaryOperatorPrecedences[cursorToken];
    if (tokenPrecedence <= 0) return -1;
    return tokenPrecedence;
}
