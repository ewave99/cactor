#define BUFFER_SIZE 256
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

enum TokenType {
    T_INTEGER,
    T_BUILTINS_DUP,
    T_BUILTINS_SWAP,
    T_BUILTINS_DROP,
    T_BUILTINS_ADD,
    T_BUILTINS_SUBTRACT,
    T_BUILTINS_MULTIPLY,
    T_BUILTINS_DIVIDE,
    T_UNKNOWN
};

struct Token {
    enum TokenType type;
    char * raw;
};

void initialiseToken(struct Token * tokenptr, const enum TokenType type, const char * raw);
int isInteger(const char * buffer);
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
    unsigned int temp_counter;

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
        if (isInteger(buffer))
            token_type = T_INTEGER;
        else if (strcmp(buffer, "dup") == 0)
            token_type = T_BUILTINS_DUP;
        else if (strcmp(buffer, "swap") == 0)
            token_type = T_BUILTINS_SWAP;
        else if (strcmp(buffer, "drop") == 0)
            token_type = T_BUILTINS_DROP;
        else if (strcmp(buffer, "add") == 0)
            token_type = T_BUILTINS_ADD;
        else if (strcmp(buffer, "subtract") == 0)
            token_type = T_BUILTINS_SUBTRACT;
        else if (strcmp(buffer, "multiply") == 0)
            token_type = T_BUILTINS_MULTIPLY;
        else if (strcmp(buffer, "divide") == 0)
            token_type = T_BUILTINS_DIVIDE;
        else
            token_type = T_UNKNOWN;
        initialiseToken(tokenptr, token_type, buffer);
        /*---*/

        token_count ++;
        tokenptr ++;
        
        if (token_count >= token_sequence_size)
        {
            token_sequence_size += 64;
            token_sequence = (struct Token *) realloc(token_sequence, sizeof (struct Token) * token_sequence_size);

            tokenptr = token_sequence;
            temp_counter = 0;
            while (temp_counter < token_count)
            {
                tokenptr ++;
                temp_counter ++;
            }
        }
    }

    fclose(fileptr);

    printTokenSequence(token_sequence, token_count);

    return 0;
}

int isInteger(const char * buffer)
{
    const char * buffer_ptr = buffer;

    while (* buffer_ptr != 0)
    {
        if (* buffer_ptr < '0' || * buffer_ptr > '9')
            return 0;
        buffer_ptr ++;
    }

    return 1;
}

void initialiseToken(struct Token * tokenptr, const enum TokenType type, const char * raw)
{
    size_t length_with_null = strlen(raw) + 1;

    tokenptr->type = type;
    tokenptr->raw = (char *) malloc(sizeof (char) * length_with_null);
    strncpy(tokenptr->raw, raw, length_with_null);
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
    switch (token->type)
    {
        case T_INTEGER:
            printf("T_INTEGER ");
            break;
        case T_BUILTINS_DUP:
            printf("T_BUILTINS_DUP ");
            break;
        case T_BUILTINS_SWAP:
            printf("T_BUILTINS_SWAP ");
            break;
        case T_BUILTINS_DROP:
            printf("T_BUILTINS_DROP ");
            break;
        case T_BUILTINS_ADD:
            printf("T_BUILTINS_ADD ");
            break;
        case T_BUILTINS_SUBTRACT:
            printf("T_BUILTINS_SUBTRACT ");
            break;
        case T_BUILTINS_MULTIPLY:
            printf("T_BUILTINS_MULTIPLY ");
            break;
        case T_BUILTINS_DIVIDE:
            printf("T_BUILTINS_DIVIDE ");
            break;
        default:
            printf("T_UNKNOWN ");
            break;
    }
    printf("%s\n", token->raw);
}
