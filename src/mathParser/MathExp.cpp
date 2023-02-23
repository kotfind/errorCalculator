#include "MathExp.h"

#include <sstream>
#include <stack>
#include <cassert>
#include <stdexcept>
#include <algorithm>

using namespace std;

MathExp::MathExp(const string& s) {
    parse(s);
}

void MathExp::parse(const string& s) {
    RPN.clear();

    stringstream ss(s);
    Token token;
    stack<Token> st; // opearions stack
    while (ss >> token, !token.isNil()) {
        // number
        if (token.isNum()) {
            RPN.push_back(token);
            continue;
        }

        // var
        if (token.isVar()) {
            RPN.push_back(token);
            vars.push_back(token.getVar());
            continue;
        }

        // func
        if (token.isFunc()) {
            st.push(token);
            continue;
        }

        // operator
        if (token.isOperator()) {
            while (!st.empty() &&
                    st.top().isOperator() &&
                    (
                        st.top().getPrecedence() > token.getPrecedence() ||
                        (
                            st.top().getPrecedence() == token.getPrecedence() &&
                            token.leftAssociative()
                        )
                     )
                ) {
                RPN.push_back(st.top());
                st.pop();
            }
            st.push(token);
            continue;
        }

        // left bracket
        if (token.getType() == Token::LEFT) {
            st.push(token);
            continue;
        }

        // right bracket
        assert(token.getType() == Token::RIGHT);
        while (st.top().getType() != Token::LEFT) {
            RPN.push_back(st.top());
            st.pop();
        }
        st.pop();

        if (st.top().isFunc()) {
            RPN.push_back(st.top());
            st.pop();
        }
    }
    while (!st.empty()) {
        RPN.push_back(st.top());
        st.pop();
    }


    // Make variables list unique
    vars.resize(unique(vars.begin(), vars.end()) - vars.begin());
}

ostream& operator<<(std::ostream& out, const MathExp& exp) {
    for (const Token& t : exp.RPN) {
        out << t << ' ';
    }
    return out;
}

void MathExp::checkVariables(const map<string, double>& vs) const {
    for (const string& var : vars) {
        if (vs.count(var) == 0) {
            throw invalid_argument("No variable '" + var + "' in varValues");
        }
    }
    // TODO: check for redundant keys in varValues?
}

double MathExp::calc(const map<string, double>& vs, bool checkVars) const {
    if (checkVars) checkVariables(vs);

    // Calculate
    stack<double> st;
    for (const Token& token : RPN) {
        if (token.isNum()) {
            st.push(token.getNum());
        } else if (token.isVar()) {
            st.push(vs.at(token.getVar()));
        } else if (token.isOperator()) {
            double y = st.top();
            st.pop();
            double x = st.top();
            st.pop();
            st.push(token.apply(x, y));
        } else if (token.isFunc()) {
            double x = st.top();
            st.pop();
            st.push(token.apply(x));
        } else {
            assert(false);
        }
    }
    assert(st.size() == 1);
    return st.top();
}

map<string, double> MathExp::gradient(const map<string, double>& vs_, double eps) const {
    checkVariables(vs_);

    map<string, double> ans;
    auto vs = vs_;
    for (const auto& v : vars) {
        vs[v] += eps;
        ans[v] = calc(vs, false);
        vs[v] -= eps;
        ans[v] -= calc(vs, false);
        ans[v] /= eps;
    }

    return ans;
}

void MathExp::apply(const std::string &funcName) {
    Token t;
    t.setType(Token::FUNC);
    t.setFuncName(funcName);
    RPN.push_back(t);
}
