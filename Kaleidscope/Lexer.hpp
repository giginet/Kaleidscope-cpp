#ifndef Lexer_hpp
#define Lexer_hpp

#include <iostream>
#include "GlobalVariables.h"

enum Token {
    END_OF_FILE = -1, // EOF may conflict
    DEF = -2,
    EXTERN = -3,
    IDENTIFIER = -4,
    NUMBER = -5,
    IF = -6,
    THEN = -7,
    ELSE = -8,
};

static bool isnumberliteral(int c) {
    return isdigit(c) || c == '.';
}

static inline int getToken() {
    static int lastChar = ' ';
    
    while (isspace(lastChar)) {
        lastChar = getchar();
    }
    
    if (isalpha(lastChar)) {
        g_identifierStr = lastChar;
        while (isalnum(lastChar = getchar())) {
            g_identifierStr += lastChar;
        }
        
        if (g_identifierStr == "def") {
            return DEF;
        }
        if (g_identifierStr == "extern") {
            return EXTERN;
        }
        if (g_identifierStr == "if") return IF;
        if (g_identifierStr == "then") return THEN;
        if (g_identifierStr == "else") return ELSE;
        return IDENTIFIER;
    }
    
    if (isnumberliteral(lastChar)) {
        std::string numberString;
        do {
            numberString += lastChar;
            lastChar = getchar();
        } while(isnumberliteral(lastChar));
        
        g_numVal = strtod(numberString.c_str(), nullptr);
        return NUMBER;
    }
    
    if (lastChar == '#') {
        do {
            lastChar = getchar();
        } while (lastChar != EOF && lastChar != '\n' && lastChar != '\r');
        
        if (lastChar != EOF) {
            return getToken();
        }
    }
    
    if (lastChar == EOF) {
        return END_OF_FILE;
    }
    
    int thisChar = lastChar;
    lastChar = getchar();
    return thisChar;
}

#endif /* Lexer_hpp */
