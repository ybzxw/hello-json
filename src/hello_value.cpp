#include "hello_value.h"
#include "hello_parser.h"
#include "assert.h"

namespace hello_json{

void Value::set_type(Type type)
{
    type_ = type;
}

int Value::get_type() const
{
    return type_;
} 

double Value::get_number() const
{
    assert(type_ == TYPE_NUMBER);
    return n_;
}

void Value::set_number(double n)
{
    type_ = TYPE_NUMBER;
    n_ = n;
}

void Value::parse(const std::string &content)
{
    Parser(*this, content);
}

}