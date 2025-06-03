#ifndef ASTHH
#define ASTHH
#include "Token.hh"

struct ASTNode {
    virtual void Print() const = 0;  
    ~ASTNode() { }
};

struct Statement : ASTNode { 
    void Print() const override { } 
};

struct Program : ASTNode { 
    std::vector<std::unique_ptr<Statement>> statements;
    void Print() const override {
        std::cout << "Program: " << std::endl;
        for (const auto& stmt : statements) {
            stmt->Print();
        }
    }
};
struct LetStmt : Statement {
    std::string name;
    std::unique_ptr<ASTNode> Expr;
    void Print() const override {
        std::cout << "LetStmt: " << name << std::endl;
        this->Expr->Print();    
    }
};

struct IfStmt : Statement {
    std::unique_ptr<ASTNode> Expr;
    std::unique_ptr<ASTNode> Block;
    void Print() const override {
        std::cout << "IfStmt: " << std::endl;
        this->Expr->Print();
        this->Block->Print();
    }
};

struct ReturnStmt : Statement {
    std::unique_ptr<ASTNode> Expr;
    void Print() const override {
        std::cout << "ReturnStmt: " << std::endl;
        this->Expr->Print();
    }
};

struct ExprStmt : Statement {
    std::unique_ptr<ASTNode> Expr;
    void Print() const override {
        std::cout << "ExprStmt: " << std::endl;
        this->Expr->Print();
    }
};

struct WhileStmt : Statement {
    std::unique_ptr<ASTNode> Expr;
    std::unique_ptr<ASTNode> Block;
    void Print() const override {
        std::cout << "WhileStmt: " << std::endl;
        this->Expr->Print();
        this->Block->Print();
    }
};

struct AssignStmt : Statement {
    std::string name;
    std::unique_ptr<ASTNode> expr;
    void Print() const override {
        std::cout << "AssignStmt: " << name << std::endl;
        this->expr->Print();
    }
};
struct BlockStmt : Statement {
    std::vector<std::unique_ptr<Statement>> statements;
    void Print() const override {
        std::cout << "BlockStmt: " << std::endl;
        for (const auto& stmt : statements) {
            stmt->Print();
        }
    }
};

struct FunctionStmt : Statement {
    std::string name;
    std::vector<std::string> params;
    std::unique_ptr<ASTNode> Block;
    void Print() const override {
        std::cout << "FunctionStmt: " << name << std::endl;
        for (const auto& param : params) {
            std::cout << "Param: " << param << std::endl;
        }
        this->Block->Print();
    }
};
struct Expr : ASTNode { 
};

struct StringExpr : Expr {
    std::string name;
    StringExpr(std::string v) : name(std::move(v)) {}
    void Print() const override {
        std::cout << "StringExpr: " << name << std::endl;
    }
};

struct NumberExpr : Expr {
    double value;
    NumberExpr(double v) : value(v) {}
    void Print() const override {
        std::cout << "NumberExpr: " << value << std::endl;
    }
};

struct BinaryExpr : Expr {
    BinaryExpr(std::unique_ptr<Expr> l, std::string o, std::unique_ptr<Expr> r)
        : left(std::move(l)), op(std::move(o)), right(std::move(r)) {}
    std::unique_ptr<Expr> left;
    std::string op;
    std::unique_ptr<Expr> right;
    void Print() const override {
        std::cout << "BinaryExpr: " << op << std::endl;
        left->Print();
        right->Print();
    }
};
struct VariableExpr : Expr {
    std::string name;
    void Print() const override {
        std::cout << "VariableExpr: " << name << std::endl;
    }
};
struct UnaryExpr : Expr {
    UnaryExpr(std::string o, std::unique_ptr<Expr> r)
        : op(std::move(o)), right(std::move(r)) {}
    std::string op;
    std::unique_ptr<Expr> right;
    void Print() const override {
        std::cout << "UnaryExpr: " << op << std::endl;
        right->Print();
    }
};

struct CallExpr : Expr {
    std::string name;
    std::vector<std::unique_ptr<Expr>> argu;
    void Print() const override {
        std::cout << "CallExpr: " << name << std::endl;
        for (const auto& arg : argu) {
            arg->Print();
        }
    }
};
#endif