#ifndef HELLO_JSON_H
#define HELLO_JSON_H

#include <string>
#include <memory>


/**
 * @brief 定义JSON的六种数据类型
 */
namespace hello_json {

enum Type {
    TYPE_NULL,                          // 值为空
    TYPE_FALSE,                         // 值为false
    TYPE_TRUE,                          // 值为true
    TYPE_NUMBER,                        // 值为数字
    TYPE_STRING,                        // 值为字符串
    TYPE_ARRAY,                         // 值为数组
    TYPE_OBJECT                         // 值为对象
}; 

class Value;


// /**
//  * @brief 定义函数返回值
//  */
// enum {
//     PARSER_OK,                     // 解析成功
//     PARSER_EXPECT_VALUE,
//     PARSER_INVALID_VALUE,
//     PARSER_ROOT_NOT_SINGULAR
// };

}

class Hellojson final{
public:
    Hellojson();
    ~Hellojson();
    void parser(const std::string &content);
    void parser(const std::string &content, std::string &status);
    void set_boolean(bool b);
    void set_null();
    int get_type() const;
    double get_number() const;
private:
    std::shared_ptr<hello_json::Value> v;
};


#endif