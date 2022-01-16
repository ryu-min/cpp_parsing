#include "tokenizer.h"

#include<stdexcept>
#include<iostream>


std::vector<Token> Tokenizer::parse(const std::string& inProgram)
{
    std::vector<Token> tokens;

    Token currentToken;
    currentToken.m_lineNumber = 1;

    for (char currCh : inProgram)
    {

        if (currentToken.m_type == STRING_ESCAPE_SEQUENCE)
        {
            switch (currCh)
            {
            case'n':
                currentToken.m_text.append(1, '\n');
                break;
            case'r':
                currentToken.m_text.append(1, '\r');
                break;
            case't':
                currentToken.m_text.append(1, '\t');
                break;
            case '\\':
                currentToken.m_text.append(1, '\\');
                break;
            default:
                throw std::runtime_error(std::string("unknown escape sequence: \\") +
                                         std::string(1, currCh) + std::string(" in string on line") +
                                         std::to_string(currentToken.m_lineNumber));

                break;
            }
            currentToken.m_type = STRING_LITERAL;
            continue;
        }
        else if (currentToken.m_type == POTENTIAL_COMMENT && currCh != '/')
        {
            currentToken.m_type = OPERATOR;
            endToken(currentToken, tokens);
            continue;
        }

        switch (currCh)
        {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':

            if (currentToken.m_type == WHITESPACE)
            {
                currentToken.m_type = INTEGER_LITERAL;
                currentToken.m_text.append(1, currCh);
            }
            else if (currentToken.m_type == POTENTIAL_DOUBLE)
            {
                currentToken.m_type = DOUBLE_LITERAL;
                currentToken.m_text.append(1, currCh);
            }
            else
            {
                currentToken.m_text.append(1, currCh);
            }
            break;

        case '.':
            if (currentToken.m_type == WHITESPACE)
            {
                currentToken.m_type = POTENTIAL_DOUBLE;
                currentToken.m_text.append(1, currCh);
            }
            else if (currentToken.m_type == INTEGER_LITERAL)
            {
                currentToken.m_type = DOUBLE_LITERAL;
                currentToken.m_text.append(1, currCh);

            }
            else if (currentToken.m_type == STRING_LITERAL)
            {
                currentToken.m_text.append(1, currCh);
            }
            else
            {
                endToken(currentToken, tokens);
                currentToken.m_type = OPERATOR;
                currentToken.m_text.append(1, currCh);
                endToken(currentToken, tokens);
            }
            break;

        case '{':
        case '}':
        case '(':
        case ')':
        case '=':
        case '-':
        case ';':
            if (currentToken.m_type != STRING_LITERAL)
            {
                endToken(currentToken, tokens);
                currentToken.m_type = OPERATOR;
                currentToken.m_text.append(1, currCh);
                endToken(currentToken, tokens);
            }
            else
            {
                currentToken.m_text.append(1, currCh);
            }
            break;

        case  ' ':
        case '\t':
            if (currentToken.m_type == STRING_LITERAL ||
                    currentToken.m_type == COMMENT)
            {
                currentToken.m_text.append(1, currCh);
            }
            else
            {
                endToken(currentToken, tokens);
            }
            break;
        case '\r':
        case '\n':
            endToken(currentToken, tokens);
            currentToken.m_lineNumber++;
            break;

        case'"':
            if (currentToken.m_type != STRING_LITERAL)
            {
                endToken(currentToken, tokens);
                currentToken.m_type = STRING_LITERAL;
            }
            else if (currentToken.m_type == STRING_LITERAL)
            {
                endToken(currentToken, tokens);
            }
            break;

        case'\\':
            if (currentToken.m_type != STRING_LITERAL)
            {
                currentToken.m_type = STRING_ESCAPE_SEQUENCE;
            }
            else
            {
                endToken(currentToken, tokens);
                currentToken.m_type = OPERATOR;
                currentToken.m_text.append(1, currCh);
                endToken(currentToken, tokens);
            }
            break;

        case'/':
            if (currentToken.m_type == STRING_LITERAL)
            {
                currentToken.m_text.append(1, currCh);
            }
            else if (currentToken.m_type == POTENTIAL_COMMENT)
            {
                currentToken.m_type = COMMENT;
                currentToken.m_text.erase();
            }
            else
            {
                endToken(currentToken, tokens);
                currentToken.m_type = POTENTIAL_COMMENT;
                currentToken.m_text.append(1, currCh);
            }

            break;
        default:

            if (currentToken.m_type == WHITESPACE ||
                    currentToken.m_type == INTEGER_LITERAL ||
                    currentToken.m_type == DOUBLE_LITERAL)
            {
                endToken(currentToken, tokens);
                currentToken.m_type = IDENTIFIER;
                currentToken.m_text.append(1, currCh);
            }
            else
            {
                currentToken.m_text.append(1, currCh);
            }
            break;
        }
    }

    endToken(currentToken, tokens);
    return tokens;
}

void Tokenizer::endToken(Token& token, std::vector<Token>& tokens)
{
    if (token.m_type == COMMENT)
    {
        std::cout << "ignoring comment" << token.m_text;
    }
    else if (token.m_type != WHITESPACE)
    {
        tokens.push_back(token);
    }
    else if (token.m_type == POTENTIAL_DOUBLE)
    {
        if (token.m_text.compare(".") == 0)
        {
            token.m_type = OPERATOR;
        }
        else
        {
            token.m_type = DOUBLE_LITERAL;
        }
    }

    token.m_type = WHITESPACE;
    token.m_text.erase(); // 1.04
}


void Token::debugPrint() const
{
    std::cout << "Token (" << TokenTypeString[m_type] << ", " << "\'"
              << m_text << "\'" << m_lineNumber << ")" << std::endl;
}


