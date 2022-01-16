#pragma once
#include <string>
#include <vector>

enum BUILTIN_TYPE
{
    VOID,
    INT8,
    UINT8,
    INT32,
    UINT32,
    DOUBLE,
    SCTRUCT,
};


/// TODO remove m_ prefix
struct Type
{
    Type(const std::string & name = "", BUILTIN_TYPE type = VOID)  : m_name(name) , m_type(type) {}
    std::string m_name;
    BUILTIN_TYPE m_type;
    std::vector<Type> m_field; // for struct only
};


