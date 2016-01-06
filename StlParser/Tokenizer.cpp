#include "Tokenizer.h"
#include <math.h>
#include <assert.h>
#include <string>

Tokenizer::Tokenizer(char* pszFilePath)
{
    fopen_s(&_pFile, pszFilePath, "r");
}

Tokenizer::~Tokenizer()
{
    fclose(_pFile);
}

Tokenizer::TOKEN Tokenizer::CurrentToken()
{
    return _currentToken;
}

Tokenizer::TOKEN Tokenizer::NextToken()
{
    if (_pFile == nullptr)
    {
        // Return empty token file could not be opened.s
        return TOKEN();
    }
    if (_pHead == nullptr || *_pHead == NULL)
    {
        _pHead = fgets(_buffer, sizeof(_buffer), _pFile);
        if (_pHead == nullptr || *_pHead == NULL)
        {
            // End of file was reached.
            return TOKEN();
        }
    }

    // Skip whitespace.
    while ((*_pHead == ' ' || *_pHead == '\t' || *_pHead == '\n') && *_pHead != NULL) _pHead++;

    // Find end of token.
    char* pStart = _pHead;
    int length = 0;
    while (*_pHead != ' ' && *_pHead != '\t' && *_pHead != NULL && *_pHead != '\n') { _pHead++; length++; }

    // Find token type.
    TOKEN_TYPE tokenType = NONE;
    if (length > 0)
    {
        for (TOKEN token : _tokenTable)
        {
            if (_strnicmp(pStart, token.AsString(), length) == 0)
            {
                tokenType = token.Type();
                break;
            }
        }
    }
    if (tokenType == NONE)
    {
        // Sniff for float.
        tokenType = FLOAT;
        char* _pSniff = pStart;
        for (int i = 0; i < length; i++)
        {
            char sniff = *_pSniff;
            if (!(sniff >= '0' && sniff <= '9' || sniff == '-' || sniff == '.' || sniff == 'E' || sniff == 'e' || sniff == '+'))
            {
                // not a float;
                tokenType = STRING_LITERAL;
                break;
            }
            _pSniff++;
        }
    }
        
    if (tokenType == STRING_LITERAL)
    {
        // Move to the next line
        _pHead = nullptr;
    }
    else
    {
        // Skip whitespace.
        while ((*_pHead == ' ' || *_pHead == '\t' || *_pHead == '\n') && *_pHead != NULL) _pHead++;
    }

    _currentToken = TOKEN(tokenType, pStart);
    return _currentToken;
}

#pragma region TOKEN

Tokenizer::TOKEN::TOKEN()
{
    _type = NONE;
    _pszToken = nullptr;
}

Tokenizer::TOKEN::TOKEN(Tokenizer::TOKEN_TYPE type, char* pszToken)
    :_type(type), _pszToken(pszToken)
{    
}

Tokenizer::TOKEN_TYPE Tokenizer::TOKEN::Type()
{
    return _type;
}

float Tokenizer::TOKEN::AsFloat()
{
    assert(_type == FLOAT);
    return atof(_pszToken);
}

char* Tokenizer::TOKEN::AsString()
{
    return _pszToken;
}

#pragma endregion