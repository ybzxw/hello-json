#include "hello_json.h"
#include "hello_value.h"
#include "hello_exception.h"

Hellojson::Hellojson(): v(new hello_json::Value){}

Hellojson::~Hellojson() {}

void Hellojson::parser(const std::string &content)
{
    v->parse(content);
}

void Hellojson::parser(const std::string &content, std::string &status)
{
    try{
        parser(content);
        status = "parse ok";
    } catch (const hello_json::Exception &msg){
        status = msg.what();
    } catch(...){

    }
}

void Hellojson::set_boolean(bool b)
{
    if(b) 
        v->set_type(hello_json::TYPE_TRUE);
    else 
        v->set_type(hello_json::TYPE_FALSE);
}


void Hellojson::set_null()
{
    v->set_type(hello_json::TYPE_NULL);
}


int Hellojson::get_type() const
{
    return v->get_type();
}


double Hellojson::get_number() const
{
    return v->get_number();
}

void Hellojson::set_number(const double n)
{
    v->set_number(n);
}

void Hellojson::set_string(const std::string &s)
{
    v->set_string(s);
}

std::string Hellojson::get_string() const
{
    return v->get_string();
}