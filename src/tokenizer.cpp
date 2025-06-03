#include "tokenizer.hh"
std::vector<Token>& Tokenizer::token() {
    return tokens;
}

void Tokenizer::Tokenize() {
    size_t size = code.size();
    for (int i = 0; i < size; i++) {
        if (isspace(code.at(i))) { continue; }

        if (code.at(i) == '\"') { 
            std::string value;
            if (i+1 < size) {
                i++;
            }
            for (;i < size && !(code.at(i) == '\"');i++) {
                value.push_back(code.at(i));
            }
            if (i == size) {
                std::cerr << "String Literal is not Closed" << std::endl;
                exit(1);
            }
            tokens.push_back({TokenType::StringLiteral, value});
        }
        else if (isalpha(code.at(i))) {
            std::string value;
            for (; i < size && (isalnum(code.at(i)) || code.at(i) == '_'); i++) {
                if (i < size) { value.push_back(code.at(i)); }
            }
            i--;
            bool isSuccess = Tokenize_String(value);
            if (isSuccess) continue;
        } else if (code.at(i) == ';') {
            tokens.push_back({TokenType::Semicolon, std::string(1, code.at(i))});
            continue;
        } else if (code.at(i) == '(') {
            tokens.push_back({TokenType::LParen, std::string(1, code.at(i))});
            continue;
        } else if (code.at(i) == ')') {
            tokens.push_back({TokenType::RParen, std::string(1, code.at(i))});
            continue;
        } else if (code.at(i) == '{') {
            tokens.push_back({TokenType::LBrace, std::string(1, code.at(i))});
            continue;
        } else if (code.at(i) == '}') {
            tokens.push_back({TokenType::RBrace, std::string(1, code.at(i))});
            continue;
        } else if (code.at(i) == ',') {
            tokens.push_back({TokenType::Comma, std::string(1, code.at(i))});
            continue;
        }else if (code.at(i) == '+') {
            tokens.push_back({ TokenType::Plus, std::string(1, code.at(i))});
            continue;
        } else if (code.at(i) == '-') {
            tokens.push_back({TokenType::Minus, std::string(1, code.at(i))});
            continue;
        } else if (code.at(i) == '/') {
            tokens.push_back({TokenType::Slash, std::string(1, code.at(i))});
            continue;
        } else if (code.at(i) == '*') {
            tokens.push_back({TokenType::Star, std::string(1, code.at(i))});
            continue;
        } else if (code.at(i) == '=') { 
            if (code.at(i+1) == '=') {
                std::string value;
                value.push_back(code.at(i));
                value.push_back(code.at(i+1));
                tokens.push_back({TokenType::EqualEqual, value});
                i++;
                continue;
            }
            tokens.push_back({TokenType::Assign, std::string(1, code.at(i))});
            continue;
        } else if (code.at(i) == '<') {
            if (code.at(i+1) == '=') {
                std::string value;
                value.push_back(code.at(i));
                value.push_back(code.at(i+1));
                tokens.push_back({TokenType::LessEqual, value});
                i++;
                continue;
            }
            tokens.push_back({TokenType::Less, std::string(1, code.at(i))});
            continue;
        } else if (code.at(i) == '>') {
            if (code.at(i+1) == '=') {
                std::string value;
                value.push_back(code.at(i));
                value.push_back(code.at(i+1));
                tokens.push_back({TokenType::GreaterEqual, value});
                i++;
                continue;
            }
            tokens.push_back({TokenType::Greater, std::string(1, code.at(i))});
            continue;
        } else if (isdigit(code.at(i))) {
            std::string value;
            for (; i < size && isdigit(code.at(i)); i++) {
                value.push_back(code.at(i));
            }
            i--;
            tokens.push_back({TokenType::NumberLiteral, value});
            continue;
        } else if (code.at(i) == '!') {
            if (code.at(i+1) == '=') {
                std::string value;
                value.push_back(code.at(i));
                value.push_back(code.at(i+1));
                tokens.push_back({TokenType::NotEqual, value});
                i++;
                continue;
            }
            std::cerr << "Invalid Token" << std::endl;
            exit(1);
        } else {
            std::cerr << "Invalid Token" << std::endl;
            exit(1);
        }
    }
    tokens.push_back({TokenType::EndOfFile, ""});
};

bool Tokenizer::Tokenize_String(std::string& code) {
    if (code == "let") {
        tokens.push_back({TokenType::let, code});
        return true;
    } else if (code == "fn") {
        tokens.push_back({TokenType::fn, code});
        return true;
    } else if (code == "while") {
        tokens.push_back({TokenType::KeywordWhile, code});
        return true;
    } else if (code == "if") {
        tokens.push_back({TokenType::KeywordIf, code});
        return true;
    } else if (code == "return") {
        tokens.push_back({TokenType::KeywordReturn, code});
        return true;
    } else {
        tokens.push_back({TokenType::Identifier, code});
        return true;
    }
    return false;
}