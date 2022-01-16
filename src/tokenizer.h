#pragma once
#include<vector>
#include<string>

enum TokenType
{
	WHITESPACE,
	IDENTIFIER,
	INTEGER_LITERAL,
    DOUBLE_LITERAL,
    STRING_LITERAL,
    OPERATOR,
    STRING_ESCAPE_SEQUENCE,
    POTENTIAL_DOUBLE,
    POTENTIAL_COMMENT,
    COMMENT
};

static const char* TokenTypeString[] =
{
    "WHITESPACE",
    "IDENTIFIER",
    "INTEGER_LITERAL",
    "DOUBLE_LITERAL",
    "STRING_LITERAL",
    "OPERATOR",
    "STRING_ESCAPE_SEQUENCE",
    "POTENTIAL_DOUBLE",
    "POTENTIAL_COMMENT",
    "COMMENT"
};

class Token
{
	/// TODO remove m_ prefix here
public:
    enum TokenType	m_type {WHITESPACE};
    std::string		m_text;
    size_t			m_lineNumber{ 0 };

    void debugPrint() const;
};

class Tokenizer
{
public:
    std::vector<Token> parse(const std::string& inProgram);

private:
    void endToken(Token & token, std::vector<Token> & tokens);

};

