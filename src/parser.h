#pragma once

#include <vector>
#include <optional>
#include <map>

#include "tokenizer.h"
#include "type.h"
#include "functiondefinition.h"

class Parser
{
public:
    Parser();
    void parse(std::vector<Token> & tokens);
    
    void debugPrint() const noexcept;

private:
    void parseFunctionBody();

    bool expectFunctionDefinition();
    std::optional<Type>  expectType();
    std::optional<Token> expectIdentifier(const std::string & name = std::string());
    std::optional<Token> expectOperator(const std::string & name = std::string());

private:
    std::vector<Token>::iterator m_currentToken;
    std::vector<Token>::iterator m_endToken;
    std::map<std::string, Type>  m_types;
    std::map<std::string, FunctionDefinition> m_functions;
};

