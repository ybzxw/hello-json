#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <math.h>
#include "hello_parser.h"
#include "hello_value.h"
#include "hello_exception.h"
#include <iostream>


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
        case 'n':  parser_literal("null", TYPE_NULL); return;
        case 't':  parser_literal("true", TYPE_TRUE); return;
        case 'f':  parser_literal("false", TYPE_FALSE); return;
        case '\"': parser_string(); return;
        case '[':  parser_array(); return;
        case '\0': throw(Exception("parse expect value"));
        default: parser_number(); return;
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
        throw(Exception("parse number too big"));
    val_.set_number(ret);
    cur_ = p;
}

void Parser::parser_string()
{
    std::string s;
    parser_string_raw(s);
    val_.set_string(s);
}
/**
 * 解析四位16进制数字
 */
void Parser::parser_hex4(const char* &p, unsigned &u)
{
    u = 0;
    for (int i = 0; i < 4; ++i){
        char ch = *p++;
        u <<= 4;
        if(isdigit(ch))
            u |= ch - '0';
        else if(ch >= 'A' && ch <= 'F')
            u |= ch - ('A' - 10);
        else if(ch >= 'a' && ch <= 'f')
            u |= ch - ('a' - 10);
        else
            throw(Exception("parse invalid unicode hex"));
    }

}

void Parser::parser_utf8(std::string & tmp, unsigned &u)
{
    if (u <= 0x7F)
        tmp += static_cast<char> (u & 0xFF);
    else if (u <= 0x7FF) {
        tmp += static_cast<char> (0xC0 | ((u >> 6) & 0xFF));
        tmp += static_cast<char> (0x80 | ( u	   & 0x3F));
    }
    else if (u <= 0xFFFF) {
        tmp += static_cast<char> (0xE0 | ((u >> 12) & 0xFF));
        tmp += static_cast<char> (0x80 | ((u >>  6) & 0x3F));
        tmp += static_cast<char> (0x80 | ( u        & 0x3F));
    }
    else {
        assert(u <= 0x10FFFF);
        tmp += static_cast<char> (0xF0 | ((u >> 18) & 0xFF));
        tmp += static_cast<char> (0x80 | ((u >> 12) & 0x3F));
        tmp += static_cast<char> (0x80 | ((u >>  6) & 0x3F));
        tmp += static_cast<char> (0x80 | ( u        & 0x3F));
    }
}

void Parser::parser_string_raw(std::string & tmp)
{
    expect(cur_, '\"');
    const char *p = cur_;
    while(*p != '\"')
    {
        if (*p == '\0')
            throw(Exception("parse miss quotation mark"));
        if (*p == '\\' && ++p){
            switch (*p++){
                case '\"': tmp += '\"'; break;
                case '\\': tmp += '\\'; break;
                case '/':  tmp += '/';  break;
                case 'b':  tmp += '\b';  break;
                case 'f':  tmp += '\f';  break;
                case 'n':  tmp += '\n';  break;
                case 'r':  tmp += '\r';  break;
                case 't':  tmp += '\t';  break;
                // 解析unicode
                case 'u' :
                    unsigned u;
                    parser_hex4(p, u);
                    if (u >= 0xD800 && u <= 0xDBFF) {
                        if (*p++ != '\\')
                            throw(Exception("parse invalid unicode surrogate"));
                        if (*p++ != 'u')
                            throw(Exception("parse invalid unicode surrogate"));
                        unsigned u2;
                        parser_hex4(p, u2);
                        if (u2 < 0xDC00 || u2 > 0xDFFF)
                            throw(Exception("parse invalid unicode surrogate"));
                        u = (((u - 0xD800) << 10) | (u2 - 0xDC00)) + 0x10000;
                    }
                    parser_utf8(tmp, u);
                    break;
                default: throw (Exception("parse invalid string escape"));
            }
        }
        else if ((unsigned char) *p < 0x20) {
            throw (Exception("parse invalid string char"));
        }
        else{
            tmp += *p++;
        }
    }
    cur_ = ++p;
}

/**
 *  解析数组
 */
void Parser::parser_array()
{
    expect(cur_, '[');
    parser_whitespace();
    std::vector<Value> tmp;
    if(*cur_ == ']'){
        ++cur_;
        // free两次会报错？为啥？
        // val_.set_type(TYPE_ARRAY);
        val_.set_array(tmp);
        return;
    }
    for(;;){
        try{
            parser_value();
        } catch (Exception){
            val_.set_type(TYPE_NULL);
            throw;
        }
        tmp.push_back(val_);
        parser_whitespace();
        if(*cur_ == ','){
            ++cur_;
            parser_whitespace();
        }
        else if(*cur_ == ']'){
            ++cur_;
            val_.set_array(tmp);
            return;
        }
        else{
            val_.set_type(TYPE_NULL);
            throw(Exception("parse miss comma or square bracket"));
        }
    }
}

}