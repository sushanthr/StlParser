#pragma once
#include <stdio.h>

class Tokenizer
{
public:
    Tokenizer(char* pszFilePath);
    ~Tokenizer();

    enum TOKEN_TYPE
    {
        KEYWORD_SOLID,
        KEYWORD_FACET,
        KEYWORD_NORMAL,
        KEYWORD_OUTER,
        KEYWORD_LOOP,
        KEYWORD_VERTEX,
        KEYWORD_ENDLOOP,
        KEYWORD_ENDFACET,
        KEYWORD_ENDSOLID,
        FLOAT,
        STRING_LITERAL,
        NONE
    };

    struct TOKEN
    {
    private:
        TOKEN_TYPE _type;
        char* _pszToken;
    public:
        TOKEN();
        TOKEN(TOKEN_TYPE type, char* pszToken);
        TOKEN_TYPE Type();
        float AsFloat();
        char* AsString();
    };

    TOKEN CurrentToken();
    TOKEN NextToken();

private:
    TOKEN _tokenTable[9] = {
        { KEYWORD_SOLID, "solid"},
        { KEYWORD_FACET, "facet" },
        { KEYWORD_NORMAL, "normal" },
        { KEYWORD_OUTER, "outer" },
        { KEYWORD_LOOP, "loop" },
        { KEYWORD_VERTEX, "vertex" },
        { KEYWORD_ENDLOOP, "endloop" },
        { KEYWORD_ENDFACET, "endfacet" },
        { KEYWORD_ENDSOLID, "endsolid" }
    };

    TOKEN _currentToken;
    FILE* _pFile;
    char _buffer[1024];
    char* _pHead;
};

