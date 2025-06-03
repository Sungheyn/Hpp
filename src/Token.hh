#ifndef TokenTypes
#define TokenTypes
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cctype>
#include <memory>
// 키워드 :
// int, float, char, if, else, while, return
enum class TokenType {
    Identifier,
    fn,
    let,
    KeywordWhile,
    KeywordIf,
    KeywordReturn,
    Plus,
    Minus,
    Star,
    Slash,
    Assign,
    EqualEqual,
    NotEqual,
    Greater,
    Less,
    GreaterEqual,
    LessEqual,
    StringLiteral,
    NumberLiteral,
    LParen,
    RParen,
    LBrace,
    RBrace,
    Comma,     
    Semicolon,
    EndOfFile,
};

struct Token {
    TokenType type;
    std::string value;
};


#endif