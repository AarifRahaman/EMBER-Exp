#include "em_string.hpp"

#include <iostream>
#include <string>

const char* ember::strcat(const char* str_a, const char* str_b) {
    std::string res(str_a);
    res.append(str_b);

    return res.c_str();
}

const std::string ember::hiercat(const std::string base, const char* suffix) {
    std::string res = base;
    res.append(ember::hierseparator);
    res.append(suffix);

    return res;
}
