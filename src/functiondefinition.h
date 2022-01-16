#pragma once

#include <string>
#include <vector>
#include <iostream>

#include "parser.h"
#include "type.h"

struct ParametersDefinition
{
    std::string m_name; // Empty string means no name given
    Type m_type;
    
    void debugPrint() {
    	std::cout << m_type.m_name << " " << m_name << std::endl;
    }
};

struct FunctionDefinition
{
    std::string m_name;
    std::vector<ParametersDefinition> m_parameters;
    
    void debugPrint()
    {
        std::cout << m_name << "(\n";
        
        for (ParametersDefinition param : m_parameters) {
        	param.debugPrint();
        }
        
        std::cout << ") " << std::endl;
    }
};

