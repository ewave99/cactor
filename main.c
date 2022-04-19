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

    T_BUILTINS_ITOS,
    T_BUILTINS_ITOC,

    T_BUILTINS_STRING_DUP,
    T_BUILTINS_STRING_SWAP,
    T_BUILTINS_STRING_DROP,
    T_BUILTINS_STRING_DROPPRINT,
    T_BUILTINS_STRING_PRINTSTACK,
    T_BUILTINS_STRING_CONCAT,

    T_BUILTINS_STRING_HEADTAIL,
    T_BUILTINS_STRING_LEN,
    T_BUILTINS_CTOI,

    T_BUILTINS_IF,
    T_BUILTINS_ELSE,
    T_BUILTINS_ENDIF,
    T_BUILTINS_WHILE,
    T_BUILTINS_ENDWHILE,
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
char * processRawString(char * raw);

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

        else if (buffer[0] == 's')
        {
            if      (strcmp(buffer + 1, "dup") == 0)    token_type = T_BUILTINS_STRING_DUP;
            else if (strcmp(buffer + 1, "swap") == 0)   token_type = T_BUILTINS_STRING_SWAP;
            else if (strcmp(buffer + 1, "drop") == 0)   token_type = T_BUILTINS_STRING_DROP;
            else if (strcmp(buffer + 1, ".") == 0)      token_type = T_BUILTINS_STRING_DROPPRINT;
            else if (strcmp(buffer + 1, ".s") == 0)     token_type = T_BUILTINS_STRING_PRINTSTACK;
            else token_type = T_UNKNOWN;
        }
        else if (strcmp(buffer, "concat") == 0) token_type = T_BUILTINS_STRING_CONCAT;
        else if (strcmp(buffer, "len") == 0)    token_type = T_BUILTINS_STRING_LEN;

        else if (strcmp(buffer, "dup") == 0)  token_type = T_BUILTINS_INT_DUP;
        else if (strcmp(buffer, "drop") == 0) token_type = T_BUILTINS_INT_DROP;
        else if (strcmp(buffer, "swap") == 0) token_type = T_BUILTINS_INT_SWAP;
        else if (strcmp(buffer, ".") == 0)    token_type = T_BUILTINS_INT_DROPPRINT;
        else if (strcmp(buffer, ".s") == 0)   token_type = T_BUILTINS_INT_PRINTSTACK;
        else if (strcmp(buffer, "+") == 0)    token_type = T_BUILTINS_INT_ADD;
        else if (strcmp(buffer, "-") == 0)    token_type = T_BUILTINS_INT_SUBTRACT;
        else if (strcmp(buffer, "*") == 0)    token_type = T_BUILTINS_INT_MULTIPLY;
        else if (strcmp(buffer, "/") == 0)    token_type = T_BUILTINS_INT_DIVIDE;
        else if (strcmp(buffer, "%") == 0)    token_type = T_BUILTINS_INT_MOD;
        else if (strcmp(buffer, "=") == 0)    token_type = T_BUILTINS_INT_EQ;
        else if (strcmp(buffer, "!=") == 0)   token_type = T_BUILTINS_INT_NE;
        else if (strcmp(buffer, ">") == 0)    token_type = T_BUILTINS_INT_GT;
        else if (strcmp(buffer, ">=") == 0)   token_type = T_BUILTINS_INT_GE;
        else if (strcmp(buffer, "<") == 0)    token_type = T_BUILTINS_INT_LT;
        else if (strcmp(buffer, "<=") == 0)   token_type = T_BUILTINS_INT_LE;
        else if (strcmp(buffer, "itos") == 0) token_type = T_BUILTINS_ITOS;
        else if (strcmp(buffer, "itoc") == 0) token_type = T_BUILTINS_ITOC;

        else if (strcmp(buffer, "ctoi") == 0) token_type = T_BUILTINS_CTOI;
        else if (strcmp(buffer, "headtail") == 0) token_type = T_BUILTINS_STRING_HEADTAIL;

        else if (strcmp(buffer, "if") == 0)       token_type = T_BUILTINS_IF;
        else if (strcmp(buffer, "else") == 0)     token_type = T_BUILTINS_ELSE;
        else if (strcmp(buffer, "endif") == 0)    token_type = T_BUILTINS_ENDIF;
        else if (strcmp(buffer, "while") == 0)    token_type = T_BUILTINS_WHILE;
        else if (strcmp(buffer, "endwhile") == 0) token_type = T_BUILTINS_ENDWHILE;

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

    char * string;

    tokenptr = token_sequence;
    index = 0;
    while (index >= 0 && index < token_count)
    {
        switch (tokenptr->type)
        {
            case T_INTEGER:
                ipush(context, toInteger(tokenptr->raw));
                break;
            case T_STRING:
                string = processRawString(tokenptr->raw);
                spush(context, string);
                free(string);
                break;

            case T_BUILTINS_ITOS:
                itos(context);
                break;
            case T_BUILTINS_ITOC:
                itoc(context);
                break;

            case T_BUILTINS_IF:
                if (!ipop(context))
                {
                    while (tokenptr->type != T_BUILTINS_ELSE && tokenptr->type != T_BUILTINS_ENDIF && index < token_count)
                    {
                        tokenptr ++;
                        index ++;
                    }
                }
                break;
            case T_BUILTINS_ELSE:
                while (tokenptr->type != T_BUILTINS_ENDIF && index < token_count)
                {
                    tokenptr ++;
                    index ++;
                }
                break;
            case T_BUILTINS_ENDIF:
                break;
            case T_BUILTINS_WHILE:
                if (!ipop(context))
                {
                    while (tokenptr->type != T_BUILTINS_ENDWHILE && index < token_count)
                    {
                        tokenptr ++;
                        index ++;
                    }
                }
                break;
            case T_BUILTINS_ENDWHILE:
                if (ipop(context))
                {
                    while (tokenptr->type != T_BUILTINS_WHILE && index >= 0)
                    {
                        tokenptr --;
                        index --;
                    }
                }
                break;

            case T_BUILTINS_CTOI:
                ctoi(context);
                break;
            case T_BUILTINS_STRING_HEADTAIL:
                headtail(context);
                break;
            case T_BUILTINS_STRING_DROPPRINT:
                soutput(context);
                break;
            case T_BUILTINS_STRING_SWAP:
                sswap(context);
                break;
            case T_BUILTINS_STRING_DUP:
                sdup(context);
                break;
            case T_BUILTINS_STRING_CONCAT:
                concat(context);
                break;
            case T_BUILTINS_STRING_LEN:
                len(context);
                break;
            case T_BUILTINS_STRING_PRINTSTACK:
                printStringStack(context);
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

char * processRawString(char * raw)
{
    unsigned int rawlength = strlen(raw);
    char * result = (char *) malloc(sizeof (char) * (rawlength + 1));

    unsigned int rawindex = 1; //ignore the ' at the start
    unsigned int resultindex = 0;

    while (rawindex < rawlength)
    {
        if (raw[rawindex] == '_')
        {
            result[resultindex] = ' ';
            resultindex ++;
        }
        else if (raw[rawindex] == '\\') /* '\' */
        {
            if (rawindex < rawlength - 1)
            {
                if (raw[rawindex + 1] == 'n')
                {
                    result[resultindex] = '\n';
                }
                else
                {
                    result[resultindex] = raw[rawindex + 1];
                }
                rawindex ++;
                resultindex ++;
            }
        }
        else
        {
            result[resultindex] = raw[rawindex];
            resultindex ++;
        }
        rawindex ++;
    }

    result[resultindex] = 0;

    return result;
}
