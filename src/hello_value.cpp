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
            new (&s_) std::string(rhs.s_);
            break;
        case TYPE_ARRAY:
            new (&arr_) std::vector<Value>(rhs.arr_);
    }
}

Value& Value::operator=(const Value &rhs)
{
    free();
    init(rhs);
}

    
void Value::free()
{
    using namespace std;
    
    switch (type_)
    {
        case TYPE_STRING: 
            s_.~string();             // 必须手工调用析构函数，不然泄露
            break;
        case TYPE_ARRAY:
            arr_.~vector<Value>();
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

std::string Value::get_string() const
{
    assert(type_ == TYPE_STRING);
    return s_;
}

std::vector<Value>::size_type Value::get_array_size() const
{
    assert(type_ == TYPE_ARRAY);
    return arr_.size();
}

const Value& Value::get_array_element(std::vector<Value>::size_type index)
{
    assert(type_ == TYPE_ARRAY);
    return arr_[index];
}

void Value::set_array(const std::vector<Value> &arr)
{
    if(type_ == TYPE_ARRAY){
        arr_ = arr;
    }
    else{
        free();
        type_ = TYPE_ARRAY;
        // Value
        new(&arr_) std::vector<Value>(arr); 
    }
}

object_size Value::get_object_size() const
{
    assert(type_ == TYPE_OBJECT);
    return obj_.size();
}

object_size Value::get_object_key_length(object_size index) const
{
    assert(type_ == TYPE_OBJECT);
    return obj_[index].first.size();
}

const std::string& Value::get_object_key(object_size index) const
{
    assert(type_ == TYPE_OBJECT);
    return obj_[index].first;
}

const Value& Value::get_object_value(object_size index) const
{
    assert(type_ == TYPE_OBJECT);
    return obj_[index].second;
}

void Value::set_object(const object &obj)
{
    if(type_ == TYPE_OBJECT){
        obj_= obj;
    }
    else{
        free();
        type_ = TYPE_OBJECT;
        new(&obj_) object(obj);
    }
}

bool operator==(const Value &lhs, const Value &rhs)
{
    if (lhs.type_ != rhs.type_)
        return false;
    switch (lhs.type_) {
        case TYPE_STRING: return lhs.s_ == rhs.s_;
        case TYPE_NUMBER: return lhs.n_ == rhs.n_;
        case TYPE_ARRAY:  return lhs.arr_ == rhs.arr_;
        // case TYPE_OBJECT:
        //     if (lhs.get_object_size() != rhs.get_object_size())
        //         return false;
        //     for (object_size i = 0; i < lhs.get_object_size(); ++i) {
        //         auto index = rhs.find_object_index(lhs.get_object_key(i));
        //         if(index < 0 || lhs.get_object_value(i) != rhs.get_object_value(index)) return false;
        //     }
        //     return true;
    }
    return true;
}
bool operator!=(const Value &lhs, const Value &rhs)
{
    return !(lhs == rhs);
}


}