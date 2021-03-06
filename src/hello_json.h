#ifndef HELLO_JSON_H
#define HELLO_JSON_H

#include <string>
#include <memory>
#include <vector>


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


// /**
//  * @brief 定义函数返回值
//  */
// enum {
//     PARSER_OK,                     // 解析成功
//     PARSER_EXPECT_VALUE,
//     PARSER_INVALID_VALUE,
//     PARSER_ROOT_NOT_SINGULAR
// };
class Value;
using object = std::vector<std::pair<std::string, Value>>;   // object类型
using object_size = std::vector<std::pair<std::string, Value>>::size_type;  // object 大小

}


class Hellojson final{
public:
    Hellojson();
    ~Hellojson();
    void parser(const std::string &content);
    void parser(const std::string &content, std::string &status);
    void set_boolean(bool b);
    void set_null();
    void set_string(const std::string &s);
    void set_number(const double n);
    void set_object();

    int get_type() const;
    double get_number() const;
    std::string get_string() const;

    std::vector<hello_json::Value>::size_type get_array_size() const;
    const hello_json::Value& get_array_element(std::vector<hello_json::Value>::size_type index);
    void set_array();

    hello_json::object_size get_object_size() const;
    hello_json::object_size get_object_key_length(hello_json::object_size index) const;
    const std::string& get_object_key(hello_json::object_size index) const;
    const hello_json::Value& get_object_value(hello_json::object_size index) const;


private:
    std::shared_ptr<hello_json::Value> v;
};


#endif