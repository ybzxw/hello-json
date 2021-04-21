#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <math.h>
#include "hello_parser.h"
#include "hello_value.h"
#include "hello_exception.h"



namespace hello_json{


inline void expect(const char * &c, char ch)
{
    assert(*c == ch);
    ++c;
}

/**
 * @brief 在初始化时进行解析
 */
Parser::Parser(Value &val, const std::string &content) 
    : val_(val), cur_(content.c_str())
{
    val_.set_type(TYPE_NULL);
    parser_whitespace();
    parser_value();
    parser_whitespace();
    if (*cur_ != '\0'){
        val_.set_type(TYPE_NULL);
        throw(Exception("parse root not singular"));
    }
}

/**
 * @brief 解析接口
 */ 
void Parser::parser_value()
{
    switch (*cur_){
        case 'n': parser_literal("null", TYPE_NULL); return;
        case 't': parser_literal("true", TYPE_TRUE); return;
        case 'f': parser_literal("false", TYPE_FALSE); return;
        case '\0': throw(Exception("parse expect value"));
        default: parser_number();
    }
}

/**
 * @brief 解析空白
 */ 
void Parser::parser_whitespace()
{
    while(*cur_ == ' '|| *cur_ == '\t' || *cur_ == '\n' || *cur_ == '\r')
        ++cur_;
}

/**
 * @brief 解析字符
 */
void Parser::parser_literal(const char *literal, Type type)
{
    expect(cur_, *literal);
    size_t i = 0;
    for(i = 0; literal[i+1] != '\0'; ++i){
        if(cur_[i] != literal[i+1])
            throw (Exception("parse invalid value"));
    }
    cur_ += i;
    val_.set_type(type);
}

/**
 * @brief 解析数字
 */
void Parser::parser_number()
{
    // 判断数字是否正确
    const char *p = cur_;
    if(*p == '-')
        ++p;
    if(*p == '0')
        ++p;
    else{
        if(!isdigit(*p)) throw (Exception("parse invalid value"));
        while(isdigit(*++p));
    }
    if (*p == '.'){
        if(!isdigit(*++p)) throw (Exception("parse invalid value"));
        while(isdigit(*++p));
    }
    if(*p == 'e' || *p == 'E'){
        ++p;
        if(*p == '+' || *p == '-') 
            ++p;
        if(!isdigit(*p)) throw (Exception("parse invalid value")); 
        while(isdigit(*++p));
    }
    // 解析
    errno = 0;
    double ret = strtod(cur_, NULL);
    if (errno == ERANGE && (ret == HUGE_VAL) || (ret == -HUGE_VAL))
        throw(Exception("parse invalid value"));
    val_.set_number(ret);
    cur_ = p;
}

}