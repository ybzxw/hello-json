#ifndef HELLO_JSON_H
#define HELLO_JSON_H


/**
 * @brief 定义JSON的六种数据类型
 */
namespace hello_json {
    
enum hello_type { 
    NULL,                          // 值为空
    FALSE,                         // 值为false
    TRUE,                          // 值为true
    NUMBER,                        // 值为数字
    STRING,                        // 值为字符串
    ARRAY,                         // 值为数组
    OBJECT                         // 值为对象
}; 


/**
 * @brief 定义函数返回值
 */
enum {
    HELLO_PARSER_OK,               // 解析成功
    HELLO_PARSER_EXPECT_VALUE,
    HELLO_PARSER_INVALID_VALUE,
    HELLO_PARSER_ROOT_NOT_SINGULAR
};




/**
 * @brief 定义JSON值
 * 
 */

class hello_value{
public:

private:
    hello_type type;                // 存储JSON类型

};

}

#endif