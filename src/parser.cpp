#include "parser.hh"

const Token& Parser::peek(int i) const { 
    size_t idx = i + this->i;
    if (idx < tokens.size()) return tokens[idx];   
    return tokens.back();
}

void Parser::advance() {
    if (i < tokens.size()) {
        i++;
    }
}

bool Parser::CTokenType(TokenType Type) {
    return (peek(0).type == Type);
}

std::unique_ptr<Program> Parser::parse() {
    std::unique_ptr<Program> pro = std::make_unique<Program>();
    for (;!CTokenType(TokenType::EndOfFile);) {
        pro->statements.push_back(ParseStmt());
    }
    return pro;
}
void Parser::PrintAST() {

}


std::unique_ptr<Statement> Parser::ParseStmt() {
    if (CTokenType(TokenType::EndOfFile)) {
        return nullptr;
    }
    if (CTokenType(TokenType::fn)) {
        return ParseFnStmt();
    } else if (CTokenType(TokenType::let)) {
        return ParseLetStmt();   
    } else if (CTokenType(TokenType::KeywordIf)) {
        return ParseIfStmt();
    } else if (CTokenType(TokenType::KeywordWhile)) {
        return ParseWhileStmt();
    } else if (CTokenType(TokenType::KeywordReturn)) {
        return ParseReturnStmt();
    } else if (CTokenType(TokenType::Identifier) && peek(1).type == TokenType::Assign) {
        return ParseAssignStmt();
    } else if (CTokenType(TokenType::Identifier)) {
        return ParseExprStmt();
    } else if (CTokenType(TokenType::LBrace)) {
        return ParseBlock();
    } else if (CTokenType(TokenType::Semicolon)) {
        advance();
        return ParseStmt();
    }
    return std::make_unique<Statement>();
};

std::unique_ptr<Statement> Parser::ParseExprStmt() {
    std::unique_ptr<ExprStmt> Stmt = std::make_unique<ExprStmt>();
    Stmt->Expr = ParseExpr();
    advance();
    if (!CTokenType(TokenType::Semicolon)) Error();
    advance();
    return Stmt;
};

std::unique_ptr<Statement> Parser::ParseFnStmt() {
    std::unique_ptr<FunctionStmt> Stmt = std::make_unique<FunctionStmt>();
    advance();
    if (CTokenType(TokenType::Identifier)) {
        Stmt->name = peek(0).value;
        advance();
        if (!CTokenType(TokenType::LParen)) Error();
        advance();
        for (; !CTokenType(TokenType::RParen); advance()) {
            if (CTokenType(TokenType::Comma)) continue;
            std::string value = peek(0).value;
            Stmt->params.push_back(value);
        }
    }
    advance();
    if (!CTokenType(TokenType::LBrace)) Error();
    Stmt->Block = ParseBlock();
    return Stmt;
};
std::unique_ptr<Statement> Parser::ParseBlock() {
    std::unique_ptr<BlockStmt> Stmt = std::make_unique<BlockStmt>();
    advance();
    for (;  !CTokenType(TokenType::RBrace);) {
        if (CTokenType(TokenType::Semicolon)) {
            advance();
            continue;
        }
        Stmt->statements.push_back(ParseStmt());
        if (CTokenType(TokenType::RBrace)) break;
    }
    advance();
    return Stmt;
};
std::unique_ptr<Statement> Parser::ParseWhileStmt() {
    std::unique_ptr<WhileStmt> Stmt = std::make_unique<WhileStmt>();
    advance();
    if (CTokenType(TokenType::LParen)) {
        advance();
        Stmt->Expr = ParseExpr();
        if (!CTokenType(TokenType::RParen)) Error();
    } else {
        Error();
    }
    advance();
    if (CTokenType(TokenType::LBrace)) {
        Stmt->Block = ParseBlock();
    } else {
        Error();
    }
    return Stmt;
};



std::unique_ptr<Statement> Parser::ParseIfStmt() {
    std::unique_ptr<IfStmt> Stmt = std::make_unique<IfStmt>();
    advance();
    if (CTokenType(TokenType::LParen)) {
        advance();
        Stmt->Expr = ParseExpr();
        if (!CTokenType(TokenType::RParen)) Error();
    } else {
        Error();
    }
    advance();
    if (CTokenType(TokenType::LBrace)) {
        Stmt->Block = ParseBlock();
    } else {
        Error();
    }
    return Stmt;
};
std::unique_ptr<Statement> Parser::ParseReturnStmt() {
    std::unique_ptr<ReturnStmt> Stmt = std::make_unique<ReturnStmt>();
    advance();
    if (CTokenType(TokenType::Semicolon)) {
        Stmt->Expr = nullptr;
    } else {
        Stmt->Expr = ParseExpr();
    }
    advance();
    return Stmt;
};
std::unique_ptr<Statement> Parser::ParseAssignStmt() {
    std::unique_ptr<AssignStmt> Stmt = std::make_unique<AssignStmt>();
    if (CTokenType(TokenType::Identifier)) {
        Stmt->name = peek(0).value;
        advance();
        if (!CTokenType(TokenType::Assign)) Error();
        advance();
        Stmt->expr = ParseExpr();
    } else {
        Error();
    }
    if (!CTokenType(TokenType::Semicolon)) Error();
    return Stmt;
};
std::unique_ptr<Statement> Parser::ParseLetStmt() {
    std::unique_ptr<LetStmt> Stmt = std::make_unique<LetStmt>();
    advance();
    if (!CTokenType(TokenType::Identifier)) Error();
    Stmt->name = peek(0).value;
    advance();
    if (!CTokenType(TokenType::Assign)) Error();
    advance();
    Stmt->Expr = ParseExpr();
    return Stmt;
};
void Parser::Error() {
    throw std::runtime_error("Parse Error at " + peek(0).value);
}
std::unique_ptr<Expr> Parser::ParseExpr() {
    std::unique_ptr<Expr> expr = ParseEqualityExpr();   
    return expr;
};
std::unique_ptr<Expr> Parser::ParseEqualityExpr() {
    std::unique_ptr<Expr> expr = ParseComparisonExpr();
    while (CTokenType(TokenType::EqualEqual) || CTokenType(TokenType::NotEqual)) {
        std::string op = peek(0).value;
        advance();
        expr = std::make_unique<BinaryExpr>(std::move(expr), op, ParseComparisonExpr());
    }
    return expr;
};
std::unique_ptr<Expr> Parser::ParseComparisonExpr() {
    std::unique_ptr<Expr> expr = ParseTermExpr();
    while (CTokenType(TokenType::Greater) || CTokenType(TokenType::Less) || CTokenType(TokenType::GreaterEqual) || CTokenType(TokenType::LessEqual)) {
        std::string op = peek(0).value;
        advance();
        expr = std::make_unique<BinaryExpr>(std::move(expr), op, ParseTermExpr());
    }
    return expr;
};
std::unique_ptr<Expr> Parser::ParseTermExpr() {
    std::unique_ptr<Expr> expr = ParseFactorExpr();
    while (CTokenType(TokenType::Plus) || CTokenType(TokenType::Minus)) {
        std::string op = peek(0).value;
        advance();
        expr = std::make_unique<BinaryExpr>(std::move(expr), op, ParseFactorExpr());
    }
    return expr;
};
std::unique_ptr<Expr> Parser::ParseFactorExpr() {
    std::unique_ptr<Expr> expr = ParseUnaryExpr();
    while (CTokenType(TokenType::Star) || CTokenType(TokenType::Slash)) {
        std::string op = peek(0).value;
        advance();
        expr = std::make_unique<BinaryExpr>(std::move(expr), op, ParseUnaryExpr());

    }
    return expr;
};
std::unique_ptr<Expr> Parser::ParseUnaryExpr() {
    if (CTokenType(TokenType::Plus) || CTokenType(TokenType::Minus)) {
        std::string op = peek(0).value;
        advance();
        return std::make_unique<UnaryExpr>(op, ParseUnaryExpr());
    }
    return ParsePrimaryExpr();
};
std::unique_ptr<Expr> Parser::ParsePrimaryExpr() {
    if (CTokenType(TokenType::Identifier) && peek(1).type != TokenType::LParen) {
        return ParseVariableExpr();
    } else if (CTokenType(TokenType::StringLiteral)) {
        return ParseStringExpr();
    } else if (CTokenType(TokenType::NumberLiteral)) {
        return ParseNumberExpr();
    } else if (CTokenType(TokenType::LParen)) {
        advance();
        std::unique_ptr<Expr> expr = ParseExpr();
        advance();
        return expr;
    } else if (CTokenType(TokenType::Identifier) && peek(1).type == TokenType::LParen) {
        return ParseCallExpr();
    }
    Error();
    return nullptr;
};
std::unique_ptr<Expr> Parser::ParseVariableExpr() {
    std::unique_ptr<VariableExpr> expr = std::make_unique<VariableExpr>();
    expr->name = peek(0).value;
    advance();
    return expr;
};
std::unique_ptr<Expr> Parser::ParseStringExpr() {
    std::unique_ptr<StringExpr> expr = std::make_unique<StringExpr>(peek(0).value);
    advance();
    return expr;
};
std::unique_ptr<Expr> Parser::ParseNumberExpr() {
    std::unique_ptr<NumberExpr> expr = std::make_unique<NumberExpr>(std::stod(peek(0).value));
    advance();
    return expr;
};
std::unique_ptr<Expr> Parser::ParseCallExpr() {
    std::unique_ptr<CallExpr> expr = std::make_unique<CallExpr>();
    expr->name = peek(0).value;
    advance();
    if (CTokenType(TokenType::LParen)) {
        advance();
        for (; !CTokenType(TokenType::RParen);) {
            if (CTokenType(TokenType::Comma)) {
                advance();
                continue;
            } 
            expr->argu.push_back(ParseExpr());
        }
    }
    return expr;
};