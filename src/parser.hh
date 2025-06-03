#ifndef parser
#define parser
#include "AST.hh"
#include "tokenizer.hh"

class Parser {
private:
    size_t i;
    
    const std::vector<Token>& tokens;

    const Token& peek(int i) const;
    void Error();
    void advance();
    bool CTokenType(TokenType Type);
    std::unique_ptr<Statement> ParseBlock();
    std::unique_ptr<Statement> ParseStmt(); 
    std::unique_ptr<Statement> ParseLetStmt();
    std::unique_ptr<Statement> ParseFnStmt();
    std::unique_ptr<Statement> ParseWhileStmt();
    std::unique_ptr<Statement> ParseReturnStmt();
    std::unique_ptr<Statement> ParseExprStmt();
    std::unique_ptr<Statement> ParseAssignStmt();
    std::unique_ptr<Statement> ParseIfStmt();
    std::unique_ptr<Expr> ParseExpr();
    std::unique_ptr<Expr> ParseVariableExpr();
    std::unique_ptr<Expr> ParseCallExpr();
    std::unique_ptr<Expr> ParseStringExpr();
    std::unique_ptr<Expr> ParseNumberExpr();
    std::unique_ptr<Expr> ParseBinaryExpr();
    // Equality
    std::unique_ptr<Expr> ParseEqualityExpr();
    // Comparison
    std::unique_ptr<Expr> ParseComparisonExpr();
    // Term
    std::unique_ptr<Expr> ParseTermExpr();
    // Factor
    std::unique_ptr<Expr> ParseFactorExpr();
    // Unary
    std::unique_ptr<Expr> ParseUnaryExpr();
    // Primary
    std::unique_ptr<Expr> ParsePrimaryExpr();
    // Call

public:
    Parser(const std::vector<Token>& tokens) : tokens(tokens) {}
    std::unique_ptr<Program> parse();
    void PrintAST();
};

#endif