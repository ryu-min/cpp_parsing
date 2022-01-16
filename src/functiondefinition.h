#pragma once

#include <string>
#include <vector>
#include <iostream>

#include "parser.h"
#include "type.h"
#include "statement.h"

struct ParametersDefinition
{
    std::string m_name; // Empty string means no name given
    Type m_type;
    
    void debugPrint() const {
    	std::cout << m_type.m_name << " " << m_name << std::endl;
    }
};

struct FunctionDefinition
{
    std::string name;
    std::vector<ParametersDefinition> parameters;
    std::vector<Statement> statements;
    
    void debugPrint() const
    {
        std::cout << name << "(\n";
        
        for (const ParametersDefinition & param : parameters) {
        	param.debugPrint();
        }
        
        std::cout << ") " << std::endl;
    }
};

