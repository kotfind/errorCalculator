#pragma once

#include <iostream>

class Token {
    public:
        enum Type {
            NIL,    // invalid
            PLUS,   // +
            MINUS,  // -
            MUL,    // *
            DIV,    // /
            POW,    // ^
            LEFT,   // (
            RIGHT,  // )
            NUM,    // double number
            FUNC,   // function (sin, cos, log and etc.)
            VAR,    // variable (any string except func names)
        };

        Token();

        Type getType() const { return type; }
        void setType(Type);
        static Type charToType(char);

        double getNum() const;
        void setNum(double);

        std::string getFuncName() const;
        void setFuncName(const std::string&);

        std::string getVar() const;
        void setVar(const std::string&);

        bool isOperator() const;
        bool isNum() const;
        bool isNil() const;
        bool isFunc() const;
        bool isVar() const;

        int getPrecedence() const;
        bool leftAssociative() const;

        double apply(double, double) const; // for operator
        double apply(double) const; // for func

    private:
        Type type = NIL;
        double num; // for type == NUM only
        std::string funcName; // for type == FUNC only
        std::string var; // for type == VAR only
};

bool operator==(const Token& lhs, const Token& rhs);

std::istream& operator>>(std::istream& in, Token& rhs);
std::ostream& operator<<(std::ostream& in, const Token& rhs);
