#define BUFFER_SIZE 256
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

enum TokenType {
    T_UNKNOWN,
    T_INTEGER,
    T_STRING,
    T_BLOCK_START,
    T_BLOCK_END,
    T_BUILTINS_DUP,
    T_BUILTINS_SWAP,
    T_BUILTINS_DROP,
    T_BUILTINS_DROPPRINT,
    T_BUILTINS_PRINTSTACK,
    T_BUILTINS_ADD,
    T_BUILTINS_SUBTRACT,
    T_BUILTINS_MULTIPLY,
    T_BUILTINS_DIVIDE,
    T_BUILTINS_MOD,
    T_BUILTINS_EQ,
    T_BUILTINS_NE,
    T_BUILTINS_GT,
    T_BUILTINS_GE,
    T_BUILTINS_LT,
    T_BUILTINS_LE,
    T_BUILTINS_IF,
    T_BUILTINS_WHILE,
    T_BUILTINS_DEFINE
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
        if (isInteger(buffer)) token_type = T_INTEGER;
        else if (buffer[0] == '\'') token_type = T_STRING;
        else if (strcmp(buffer, "{") == 0) token_type = T_BLOCK_START;
        else if (strcmp(buffer, "}") == 0) token_type = T_BLOCK_END;
        else if (strcmp(buffer, "dup") == 0) token_type = T_BUILTINS_DUP;
        else if (strcmp(buffer, "swap") == 0) token_type = T_BUILTINS_SWAP;
        else if (strcmp(buffer, "drop") == 0) token_type = T_BUILTINS_DROP;
        else if (strcmp(buffer, ".") == 0) token_type = T_BUILTINS_DROPPRINT;
        else if (strcmp(buffer, ".s") == 0) token_type = T_BUILTINS_PRINTSTACK;
        else if (strcmp(buffer, "+") == 0) token_type = T_BUILTINS_ADD;
        else if (strcmp(buffer, "-") == 0) token_type = T_BUILTINS_SUBTRACT;
        else if (strcmp(buffer, "*") == 0) token_type = T_BUILTINS_MULTIPLY;
        else if (strcmp(buffer, "/") == 0) token_type = T_BUILTINS_DIVIDE;
        else if (strcmp(buffer, "%") == 0) token_type = T_BUILTINS_MOD;
        else if (strcmp(buffer, "=") == 0) token_type = T_BUILTINS_EQ;
        else if (strcmp(buffer, "!=") == 0) token_type = T_BUILTINS_NE;
        else if (strcmp(buffer, ">") == 0) token_type = T_BUILTINS_GT;
        else if (strcmp(buffer, ">=") == 0) token_type = T_BUILTINS_GE;
        else if (strcmp(buffer, "<") == 0) token_type = T_BUILTINS_LT;
        else if (strcmp(buffer, "<=") == 0) token_type = T_BUILTINS_LE;
        else if (strcmp(buffer, "if") == 0) token_type = T_BUILTINS_IF;
        else if (strcmp(buffer, "while") == 0) token_type = T_BUILTINS_WHILE;
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
    printf("%u\n", token_count);
    printf("%lu\n", sizeof (struct Token));

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
    printf("%d %s\n", token->type, token->raw);
}
