#include "Tokenizer.h"
#include <stdio.h>

int main()
{
    char* path = "C:\\Users\\susha\\Downloads\\square_ruler.stl";
    Tokenizer tokenizer = Tokenizer(path);
    while (tokenizer.NextToken().Type() != Tokenizer::NONE)
    {
        Tokenizer::TOKEN currentToken = tokenizer.CurrentToken();
        switch (currentToken.Type())
        {
        case Tokenizer::KEYWORD_SOLID:
            printf("solid");
            break;
        case Tokenizer::KEYWORD_FACET:
            printf("facet");
            break;
        case Tokenizer::KEYWORD_NORMAL:
            printf("normal");
            break;
        case Tokenizer::KEYWORD_OUTER:
            printf("outer");
            break;
        case Tokenizer::KEYWORD_LOOP:
            printf("loop");
            break;
        case Tokenizer::KEYWORD_VERTEX:
            printf("vertex");
            break;
        case Tokenizer::KEYWORD_ENDLOOP:
            printf("endloop");
            break;
        case Tokenizer::KEYWORD_ENDFACET:
            printf("endfacet");
            break;
        case Tokenizer::KEYWORD_ENDSOLID:
            printf("endsolid");
            break;
        case Tokenizer::FLOAT:
            printf("float %f", currentToken.AsFloat());
            break;
        case Tokenizer::STRING_LITERAL:
            printf("string %s", currentToken.AsString());
            break;
        case Tokenizer::NONE:
            printf("none");
            break;
        default:
            break;
        }
        printf("\n");
    }
}