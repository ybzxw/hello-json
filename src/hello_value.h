#ifndef HELLO_VALUE_H
#define HELLO_VALUE_H

#include"hello_json.h"

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
    void set_number(double n);
    void parse(const std::string &content);

    
private:
    Type type_ = TYPE_NULL;                // 存储JSON类型
    double n_;                              // 存储数字类型
};

;
}

#endif