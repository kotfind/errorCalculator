#pragma once

#include <functional>
#include <string>
#include <unordered_map>
#include <math.h>

#define F(f) {#f, [](double x) { return f(x); }}

const std::unordered_map<std::string, std::function<double(double)>> funcs = {
    F(sin),
    F(cos),
    F(log),
};

#undef GEN
