#include "parser.h"

#include <iostream>

using namespace std;

Parser::Parser()
{
    m_types["void"]     = Type("void", VOID);
    m_types["int"]      = Type("signed int", INT32);
    m_types["usigned"]  = Type("usigned int", UINT32);
    m_types["char"]     = Type("signed char", INT8);
    m_types["uint8_t"]  = Type("uint8_t", UINT8);
    m_types["double"]   = Type("double", DOUBLE);
}

void Parser::parse(vector<Token> &tokens)
{
    m_currentToken  = tokens.begin();
    m_endToken      = tokens.end();

    while ( m_currentToken != m_endToken )
    {
        if (expectFunctionDefinition())
        {
            return;
        }
        else
        {
            cerr << "Unknown identifier " << m_currentToken->m_text << ".";
            ++m_currentToken;
        }

    }
}

void Parser::parseFunctionBody()
{
//    if ( !expectOperator("{").has_value() ) {
//    	t
//    }
}

bool Parser::expectFunctionDefinition()
{
    auto parseStart = m_currentToken;
    auto possibleType = expectType();
    if ( possibleType.has_value() ) // we have a type
    {
        auto possibleName = expectIdentifier();
        if ( possibleName.has_value() ) // we have a name
        {
            auto possibleOperator = expectOperator("("); // we have a function
            if ( possibleOperator.has_value() )
            {
                cout << "We have a function - " << possibleName->m_text;
                FunctionDefinition function;
                function.m_name = possibleName->m_text;

                while (!expectOperator(")").has_value())
                {
                    auto possibleParamType = expectType();

                    if (!possibleParamType.has_value())
                    {
                        throw runtime_error("expected a type at start of arguments of parameters list");
                    }

                    auto possibleVariableName = expectIdentifier();
                    
                    ParametersDefinition param;
                    param.m_type = possibleParamType->m_name;
                    
                    if ( possibleVariableName.has_value() )
                    {
                        param.m_name = possibleVariableName->m_text;
                    }
                    
                    function.m_parameters.push_back(param);
                    
                    if (expectOperator(")").has_value()) {
						break;
                    }
                    
                    if (expectOperator(",").has_value()) {
                    	throw runtime_error("Expected ',' to separate parameters of function " + possibleName->m_text);
                    }
                }
				m_functions[function.m_name] = function;
				parseFunctionBody();
                return true;
            }
            else
            {
                m_currentToken = parseStart;
            }
        }
        else
        {
            m_currentToken = parseStart;
        }
    }
    return false;
}

std::optional<Type> Parser::expectType()
{
    auto possibleType = expectIdentifier();

    if (!possibleType)
    {
        return nullopt;
    }

    auto foundedType = m_types.find(possibleType->m_text);

    if (foundedType == m_types.end())
    {
        --m_currentToken;
        return nullopt;
    }
    else
    {
        return foundedType->second;
    }
}

std::optional<Token> Parser::expectIdentifier(const string & name)
{
    if (m_currentToken == m_endToken  || m_currentToken->m_type != IDENTIFIER) return nullopt;
    if ( !name.empty() && m_currentToken->m_text != name) return  nullopt;
    return *m_currentToken++;
}

std::optional<Token> Parser::expectOperator(const string &name)
{
    if (m_currentToken == m_endToken  || m_currentToken->m_type != OPERATOR) return nullopt;
    if ( !name.empty() && m_currentToken->m_text != name) return  nullopt;
    return *m_currentToken++;
}
void Parser::debugPrint() const noexcept
{
	std::cout << "finction size " << m_functions.size();
	for (auto func : m_functions)
	{
		func.second.debugPrint();
	}
}
