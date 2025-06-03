#ifndef tokenizer
#define tokenizer
#include "Token.hh"

class Tokenizer {
public:
    Tokenizer(const std::string& code) : code(code) {}
    void Tokenize();
    std::vector<Token>& token();
private:
    bool Tokenize_String(std::string& code);
    bool IsOperator(int i, size_t size);
    std::vector<Token> tokens;
    std::string code;
};

#endif