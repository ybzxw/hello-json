#include "hello_value.h"
#include "hello_parser.h"
#include "assert.h"
#include "new"

namespace hello_json{

Value::~Value()
{
    free();
}

void Value::init(const Value &rhs)
{
    type_ = rhs.type_;
    n_ = 0;
    switch (type_)
    {
        case TYPE_NUMBER: 
            n_ = rhs.n_;
            break;
        case TYPE_STRING:
            new (&s_) std::string(s_);
            break;
    }
}

void Value::free()
{
    using namespace std;
    
    switch (type_)
    {
        case TYPE_STRING: 
            s_.~string();            // 必须手工调用析构函数，不然泄露
            break;      
    }
    type_ = TYPE_NULL;
}

void Value::set_type(Type type)
{
    free();
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

void Value::set_number(const double n)
{
    free();
    type_ = TYPE_NUMBER;
    n_ = n;
}

void Value::parse(const std::string &content)
{
    Parser(*this, content);
}

void Value::set_string(const std::string &s)
{
    if (type_ == TYPE_STRING){
        s_ = s;
    }
    else{
        free();
        type_ = TYPE_STRING;
        // placement new 给指定的内存用指定的构造函数来构造对象
        // 唯一一种必须手工调用析构函数的情况
        new(&s_) std::string(s);
    }
}

}