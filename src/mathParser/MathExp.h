#pragma once

#include "Token.h"

#include <string>
#include <vector>
#include <ostream>
#include <map>

class MathExp;
std::ostream& operator<<(std::ostream&, const MathExp&);

class MathExp {
    public:
        MathExp(const std::string&);

        double calc(const std::map<std::string, double>& vs, bool checkVars = true) const;
        std::map<std::string, double> gradient(const std::map<std::string, double>& vs, double eps = 1e-9) const;

        std::vector<std::string> getVars() const { return vars; }

        void apply(const std::string& funcName);

    private:
        void checkVariables(const std::map<std::string, double>& vars) const;

        void parse(const std::string&);

        std::vector<Token> RPN; // reverse polish notation

        std::vector<std::string> vars;

    friend std::ostream& operator<<(std::ostream&, const MathExp&);
};
