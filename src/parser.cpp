#include "parser.h"

#include <iostream>

using namespace std;

Parser::Parser()
{
    m_types["void"]     = Type("void", VOID);
    m_types["int"]      = Type("signed int", INT32);
    m_types["unsigned"]  = Type("unsigned int", UINT32);
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

std::optional<std::vector<Statement>> Parser::parseFunctionBody()
{
    if ( !expectOperator("{").has_value() ) {
    	return nullopt;
    }
    
    std::vector<Statement> statements;
    
    auto statement = parseOneStatement();
    if ( statement.has_value() ) {
    	statements.push_back(statement.value());
    }
    
//    if ( !expectOperator("}").has_value() ) {
//    	throw std::runtime_error("Unbalanced '{.'");
//    }
    
    return statements;
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
                function.name = possibleName->m_text;

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
                    
                    function.parameters.push_back(param);
                    
                    if (expectOperator(")").has_value()) {
						break;
                    }
                    
                    if (expectOperator(",").has_value()) {
                    	throw runtime_error("Expected ',' to separate parameters of function " + possibleName->m_text);
                    }
                }
				auto statements = parseFunctionBody();
                
                if ( !statements.has_value() ) {
                	m_currentToken = parseStart;
                	return false;
                }
                
                if ( statements.has_value() ) {
					function.statements = statements.value();
				}
    
				m_functions[function.name] = function;
				
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
	std::cout << "finction size " << m_functions.size()  << " ";
	for (auto func : m_functions)
	{
		func.second.debugPrint();
	}
}
std::optional<Statement> Parser::parseOneStatement() {
	auto startToken 	= m_currentToken;
	auto possibleType 	= expectType();
	
	if ( !possibleType.has_value() ) {
		m_currentToken = startToken;
		return nullopt;
	}
	
	auto possibleVariableName = expectIdentifier();
	if ( !possibleType.has_value() ) {
		m_currentToken = startToken;
		return nullopt;
	}
	
	Statement statement;
	
	statement.kind = Statement::StatementKind::VARIABLE_DECLARATION;
	statement.name = possibleVariableName->m_text;
	statement.type = possibleType.value();
	
	
	return statement;
}
