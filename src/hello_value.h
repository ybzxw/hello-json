#ifndef HELLO_VALUE_H
#define HELLO_VALUE_H

#include "hello_json.h"

namespace hello_json{

/**
 * @brief 定义JSON值类，存储解析后的JSON值
 * 
 */

class Value{
public:
    void set_type(Type type);
    int get_type() const;
    double get_number() const;
    std::string get_string() const;
    void set_number(const double n);
    void parse(const std::string &content);
    void set_string(const std::string &s);

    std::vector<Value>::size_type get_array_size() const;
    const Value& get_array_element(std::vector<Value>::size_type index);
    void set_array(const std::vector<Value> &arr);
    Value() { n_ = 0; }
    Value(const Value &rhs) { init(rhs); }
    Value& operator=(const Value &rhs);
    ~Value(); 

private:
    void init(const Value &rhs);
    void free();
    Type type_ = TYPE_NULL;                // 存储JSON类型
    union{
        double n_;                              // 存储数字类型
        std::string s_;                         // 存储字符串类型
        std::vector<Value> arr_;                // 存储数组类型
    };
    friend bool operator==(const Value &lhs, const Value &rhs);
};

bool operator==(const Value &lhs, const Value &rhs);
bool operator!=(const Value &lhs, const Value &rhs);

}

#endif