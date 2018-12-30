#ifndef Lexer_hpp
#define Lexer_hpp

#include <stdio.h>

enum Token {
    END_OF_FILE, // EOF may conflict
    DEF,
    EXTERN,
    IDENTIFIER,
    NUMBER
};

#endif /* Lexer_hpp */
