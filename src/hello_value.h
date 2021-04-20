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

private:
    Type type_;                // 存储JSON类型
};


}

#endif