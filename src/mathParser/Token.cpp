#include "Token.h"

#include "funcs.h"

#include <iostream>
#include <cassert>
#include <cctype>
#include <math.h>
#include <stdexcept>

using namespace std;

Token::Token() {}

void Token::setType(Type t) {
    type = t;
}
Token::Type Token::charToType(char c) {
    switch (c) {
        case '+': return PLUS;
        case '-': return MINUS;
        case '*': return MUL;
        case '/': return DIV;
        case '^': return POW;
        case '(': return LEFT;
        case ')': return RIGHT;
    }
    throw invalid_argument("Cannot convert char '" + string(1, c) + "' to Token::Type.");
}

double Token::getNum() const {
    assert(type == NUM);
    return num;
}
void Token::setNum(double x) {
    assert(type == NUM);
    num = x;
}

string Token::getVar() const {
    assert(type == VAR);
    return var;
}
void Token::setVar(const string& s) {
    assert(type == VAR);
    var = s;
}

string Token::getFuncName() const {
    assert(isFunc());
    return funcName;
}
void Token::setFuncName(const string& name) {
    assert(isFunc());
    if (!funcs.count(name)) throw invalid_argument(name + " is not a function name");
    funcName = name;
}

bool Token::isOperator() const {
    switch (type) {
        case Token::PLUS: 
        case Token::MINUS:
        case Token::MUL:  
        case Token::DIV:  
        case Token::POW:  
            return true;

        default:
            return false;
    }
}
bool Token::isNum() const {
    return type == NUM;
}
bool Token::isNil() const {
    return type == NIL;
}
bool Token::isFunc() const {
    return type == FUNC;
}
bool Token::isVar() const {
    return type == VAR;
}

int Token::getPrecedence() const {
    assert(isOperator());
    switch (type) {
        case POW:   return 3;
        case MUL:   return 2;
        case DIV:   return 2;
        case PLUS:  return 1;
        case MINUS: return 1;
        default:    assert(false);
    }
}
bool Token::leftAssociative() const {
    assert(isOperator());
    switch (type) {
        case POW:   return false;
        case MUL:   return true;
        case DIV:   return true;
        case PLUS:  return true;
        case MINUS: return true;
        default:    assert(false);
    }
}

double Token::apply(double x, double y) const {
    assert(isOperator());
    switch (type) {
        case PLUS:  return x + y;
        case MINUS: return x - y;
        case MUL:   return x * y;
        case DIV:   return x / y;
        case POW:   return pow(x, y);
        default:    assert(false);
    }
}
double Token::apply(double x) const {
    assert(isFunc());
    return funcs.at(funcName)(x);
}

bool operator==(const Token& lhs, const Token& rhs) {
    if (lhs.getType() == Token::NUM && lhs.getType() == Token::NUM) {
        return lhs.getNum() == rhs.getNum();
    }
    return lhs.getType() == rhs.getType();
}

istream& operator>>(istream& in, Token& rhs) {
    rhs.setType(Token::NIL);

    // whitespace
    while (!in.eof() && isspace(in.peek())) {
        in.get();
    }

    if (in.eof()) {
        return in;
    }

    // num
    if (isdigit(in.peek())) {
        string num = "";
        while (!in.eof() && (isdigit(in.peek()) || (char)in.peek() == '.')) {
            num += in.get();
        }
        rhs.setType(Token::NUM);
        rhs.setNum(stod(num));
        return in;
    }

    // func or var
    if (isalpha(in.peek())) {
        string word = "";
        while (!in.eof() && isalpha(in.peek())) {
            word += in.get();
        }
        try {
            rhs.setType(Token::FUNC);
            rhs.setFuncName(word);
        } catch (const invalid_argument&) {
            rhs.setType(Token::VAR);
            rhs.setVar(word);
        }
        return in;
    }

    // operator or bracket
    rhs.setType(Token::charToType(in.get()));
    return in;
}
ostream& operator<<(ostream& out, const Token& rhs) {
    switch (rhs.getType()) {
        case Token::PLUS:  out << '+'; break;
        case Token::MINUS: out << '-'; break;
        case Token::MUL:   out << '*'; break;
        case Token::DIV:   out << '/'; break;
        case Token::POW:   out << '^'; break;
        case Token::LEFT:  out << '('; break;
        case Token::RIGHT: out << ')'; break;
        case Token::NIL:   out << "NIL"; break;
        case Token::NUM:   out << rhs.getNum(); break;
        case Token::VAR:   out << rhs.getVar(); break;
        case Token::FUNC:  out << rhs.getFuncName(); break;
    }
    return out;
}
