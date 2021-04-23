#ifndef HELLO_PARSER_H
#define HELLO_PARSER_H


#include "hello_value.h"


namespace hello_json{

/**
 * @brief 解析器类
 * 
 */
class Parser final{

public:
    Parser(Value &val, const std::string &content);
    
private:
    void parser_whitespace();
    void parser_literal(const char *literal, Type type);
    void parser_value();
    void parser_number();
    void parser_string();
    void parser_string_raw(std::string & tmp);



    Value &val_;                 //存放解析类型
    const char *cur_;            //存放待解析的json
};



}

#endif