#ifndef HELLO_EXCEPTION_H
#define HELLO_EXCEPTION_H

#include <string>
#include <stdexcept>

namespace hello_json{

class Exception final : public std::logic_error{
public:
    Exception(const std::string &err_msg) : logic_error(err_msg) {}
};

}

#endif