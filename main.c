#define BUFFER_SIZE 256
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "builtins.h"

enum TokenType {
    T_UNKNOWN,

    T_INTEGER,
    T_STRING,

    T_BLOCK_START,
    T_BLOCK_END,

    T_BUILTINS_DEFINE,

    T_BUILTINS_INT_DUP,
    T_BUILTINS_INT_SWAP,
    T_BUILTINS_INT_DROP,
    T_BUILTINS_INT_DROPPRINT,
    T_BUILTINS_INT_PRINTSTACK,
    T_BUILTINS_INT_ADD,
    T_BUILTINS_INT_SUBTRACT,
    T_BUILTINS_INT_MULTIPLY,
    T_BUILTINS_INT_DIVIDE,
    T_BUILTINS_INT_MOD,
    T_BUILTINS_INT_EQ,
    T_BUILTINS_INT_NE,
    T_BUILTINS_INT_GT,
    T_BUILTINS_INT_GE,
    T_BUILTINS_INT_LT,
    T_BUILTINS_INT_LE,
    T_BUILTINS_INT_IF,
    T_BUILTINS_INT_IFELSE,
    T_BUILTINS_INT_WHILE,

    T_BUILTINS_STRING_DUP,
    T_BUILTINS_STRING_SWAP,
    T_BUILTINS_STRING_DROP,
    T_BUILTINS_STRING_DROPPRINT,
    T_BUILTINS_STRING_PRINTSTACK,
    T_BUILTINS_STRING_CONCAT,
    T_BUILTINS_STRING_IF,
    T_BUILTINS_STRING_IFELSE,
    T_BUILTINS_STRING_WHILE,
};

struct Token {
    enum TokenType type;
    char * raw;
};

void initialiseToken(struct Token * tokenptr, const enum TokenType type, const char * raw);
int isInteger(const char * buffer);
int toInteger(char * buffer);
void printTokenSequence(struct Token * token_sequence, size_t length);
void printToken(const struct Token * token);

int main(int argc, char ** argv)
{
    FILE * fileptr;

    char current_char;
    char buffer[BUFFER_SIZE];
    char * buffer_ptr;

    size_t token_sequence_size;
    struct Token * token_sequence;
    unsigned int token_count;
    struct Token * tokenptr;
    unsigned int index;

    enum TokenType token_type;

    if (argc != 2)
    {
        printf("Incorrect number of arguments.\n");
        return 1;
    }

    token_sequence_size = 64;
    token_sequence = (struct Token *) malloc(sizeof(struct Token) * token_sequence_size);
    token_count = 0;
    tokenptr = token_sequence;

    fileptr = fopen(argv[1], "r");
    if (fileptr == NULL)
    {
        printf("Error opening file: %s\n", argv[1]);
        return 1;
    }

    buffer_ptr = buffer;
    current_char = getc(fileptr);

    while (current_char != EOF)
    {
        while (current_char != EOF && isspace(current_char) == 0)
        {
            * buffer_ptr = current_char;

            buffer_ptr ++;
            current_char = getc(fileptr);
        }

        * buffer_ptr = 0;

        buffer_ptr = buffer;
        while (current_char != EOF && isspace(current_char) != 0)
            current_char = getc(fileptr);

        /* process token */
        if (isInteger(buffer)) token_type = T_INTEGER;
        else if (buffer[0] == '\'') token_type = T_STRING;

        else if (buffer[0] == 'i')
        {
            if      (strcmp(buffer + 1, "dup") == 0)    token_type = T_BUILTINS_INT_DUP;
            else if (strcmp(buffer + 1, "drop") == 0)   token_type = T_BUILTINS_INT_DROP;
            else if (strcmp(buffer + 1, "swap") == 0)   token_type = T_BUILTINS_INT_SWAP;
            else if (strcmp(buffer + 1, ".") == 0)      token_type = T_BUILTINS_INT_DROPPRINT;
            else if (strcmp(buffer + 1, ".s") == 0)     token_type = T_BUILTINS_INT_PRINTSTACK;
            else if (strcmp(buffer + 1, "+") == 0)      token_type = T_BUILTINS_INT_ADD;
            else if (strcmp(buffer + 1, "-") == 0)      token_type = T_BUILTINS_INT_SUBTRACT;
            else if (strcmp(buffer + 1, "*") == 0)      token_type = T_BUILTINS_INT_MULTIPLY;
            else if (strcmp(buffer + 1, "/") == 0)      token_type = T_BUILTINS_INT_DIVIDE;
            else if (strcmp(buffer + 1, "%") == 0)      token_type = T_BUILTINS_INT_MOD;
            else if (strcmp(buffer + 1, "=") == 0)      token_type = T_BUILTINS_INT_EQ;
            else if (strcmp(buffer + 1, "!=") == 0)     token_type = T_BUILTINS_INT_NE;
            else if (strcmp(buffer + 1, ">") == 0)      token_type = T_BUILTINS_INT_GT;
            else if (strcmp(buffer + 1, ">=") == 0)     token_type = T_BUILTINS_INT_GE;
            else if (strcmp(buffer + 1, "<") == 0)      token_type = T_BUILTINS_INT_LT;
            else if (strcmp(buffer + 1, "<=") == 0)     token_type = T_BUILTINS_INT_LE;
            else if (strcmp(buffer + 1, "if") == 0)     token_type = T_BUILTINS_INT_IF;
            else if (strcmp(buffer + 1, "ifelse") == 0) token_type = T_BUILTINS_INT_IFELSE;
            else if (strcmp(buffer + 1, "while") == 0)  token_type = T_BUILTINS_INT_WHILE;
            else token_type = T_UNKNOWN;
        }

        else if (buffer[0] == 's')
        {
            if      (strcmp(buffer + 1, "dup") == 0)    token_type = T_BUILTINS_STRING_DUP;
            else if (strcmp(buffer + 1, "swap") == 0)   token_type = T_BUILTINS_STRING_SWAP;
            else if (strcmp(buffer + 1, "drop") == 0)   token_type = T_BUILTINS_STRING_DROP;
            else if (strcmp(buffer + 1, ".") == 0)      token_type = T_BUILTINS_STRING_DROPPRINT;
            else if (strcmp(buffer + 1, ".s") == 0)     token_type = T_BUILTINS_STRING_PRINTSTACK;
            else if (strcmp(buffer + 1, "ifelse") == 0) token_type = T_BUILTINS_STRING_IFELSE;
            else if (strcmp(buffer + 1, "while") == 0)  token_type = T_BUILTINS_STRING_WHILE;
            else token_type = T_UNKNOWN;
        }

        else if (strcmp(buffer, "{") == 0) token_type = T_BLOCK_START;
        else if (strcmp(buffer, "}") == 0) token_type = T_BLOCK_END;
        else if (strcmp(buffer, "define") == 0) token_type = T_BUILTINS_DEFINE;

        else token_type = T_UNKNOWN;

        initialiseToken(tokenptr, token_type, buffer);
        /*---*/

        token_count ++;

        tokenptr ++;
        
        if (token_count >= token_sequence_size)
        {
            token_sequence_size += 64;
            token_sequence = (struct Token *) realloc(token_sequence, sizeof (struct Token) * token_sequence_size);

            tokenptr = token_sequence;
            index = 0;
            while (index < token_count)
            {
                tokenptr ++;
                index ++;
            }
        }
    }

    fclose(fileptr);

    struct Context * context = init();

    tokenptr = token_sequence;
    index = 0;
    while (index < token_count)
    {
        switch (tokenptr->type)
        {
            case T_INTEGER:
                ipush(context, toInteger(tokenptr->raw));
                break;

            case T_BUILTINS_INT_DUP:
                idup(context);
                break;
            case T_BUILTINS_INT_SWAP:
                iswap(context);
                break;
            case T_BUILTINS_INT_DROP:
                idrop(context);
                break;
            case T_BUILTINS_INT_DROPPRINT:
                ioutput(context);
                break;
            case T_BUILTINS_INT_PRINTSTACK:
                printIntStack(context);
                break;
            case T_BUILTINS_INT_ADD:
                iadd(context);
                break;
            case T_BUILTINS_INT_SUBTRACT:
                isubtract(context);
                break;
            case T_BUILTINS_INT_MULTIPLY:
                imultiply(context);
                break;
            case T_BUILTINS_INT_DIVIDE:
                idivide(context);
                break;
            case T_BUILTINS_INT_MOD:
                imod(context);
                break;
            case T_BUILTINS_INT_EQ:
                ieq(context);
                break;
            case T_BUILTINS_INT_NE:
                ine(context);
                break;
            case T_BUILTINS_INT_GT:
                igt(context);
                break;
            case T_BUILTINS_INT_GE:
                ige(context);
                break;
            case T_BUILTINS_INT_LT:
                ilt(context);
                break;
            case T_BUILTINS_INT_LE:
                ile(context);
                break;
            default:
                break;
        }

        tokenptr ++;
        index ++;
    }

    freeContext(context);

    return 0;
}

int isInteger(const char * buffer)
{
    const char * buffer_ptr = buffer;
    
    if (* buffer_ptr == '-')
        buffer_ptr ++;
    if (* buffer_ptr == 0)
        return 0;

    while (* buffer_ptr != 0)
    {
        if (* buffer_ptr < '0' || * buffer_ptr > '9')
            return 0;
        buffer_ptr ++;
    }

    return 1;
}
int toInteger(char * buffer)
{
    char * buffer_ptr;
    int result;
    int sign;

    buffer_ptr = buffer;
    result = 0;

    sign = 1;
    if (* buffer_ptr == '-')
    {
        sign = -1;
        buffer_ptr ++;
    }

    while (* buffer_ptr != 0)
    {
        result *= 10;
        result += (int) (* buffer_ptr) - 48;

        buffer_ptr ++;
    }

    result *= sign;

    return result;
}

void initialiseToken(struct Token * tokenptr, const enum TokenType type, const char * raw)
{
    size_t length_with_null = strlen(raw) + 1;

    if (type == T_UNKNOWN || type == T_STRING || type == T_INTEGER)
    {
        tokenptr->raw = (char *) malloc(sizeof (char) * length_with_null);
        strncpy(tokenptr->raw, raw, length_with_null);
    }
    else
    {
        tokenptr->raw = NULL;
    }
    tokenptr->type = type;
}

void printTokenSequence(struct Token * token_sequence, size_t length)
{
    struct Token * tokenptr = token_sequence;
    int counter = 0;

    while (counter < length)
    {
        printToken(tokenptr);

        tokenptr ++;
        counter ++;
    }
}

void printToken(const struct Token * token)
{
    printf("%d", token->type);
    if (token->raw)
    {
        printf(" %s", token->raw);
    }
    printf("\n");
}
