#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct IntStack
{
    size_t length;
    size_t max_length;
    int * start;
    int * end;
};

struct StringStack
{
    size_t length;
    size_t max_length;
    char ** start;
    char ** end;
};

struct Context
{
    struct IntStack * istack;
    struct StringStack * sstack;
};

int ipop(struct Context *);
void ipush(struct Context *, int);

void spush(struct Context *, const char *);
char * spop(struct Context *);

struct Context * init()
{
    struct Context * context;

    context = (struct Context *) malloc(sizeof (struct Context));

    context->istack = (struct IntStack *) malloc(sizeof (struct IntStack));
    context->istack->max_length = 64;
    context->istack->start = (int *) malloc(sizeof (int) * context->istack->max_length);
    context->istack->end = context->istack->start;
    context->istack->length = 0;

    context->sstack = (struct StringStack *) malloc(sizeof (struct StringStack));
    context->sstack->max_length = 64;
    context->sstack->start = (char **) malloc(sizeof (char *) * context->sstack->max_length);
    context->sstack->end = context->sstack->start;
    context->sstack->length = 0;

    return context;
}
void freeContext(struct Context * context)
{
    free(context->istack->start);
    free(context->istack);

    for (int i = 0; i < context->sstack->length; i ++)
        free(context->sstack->start[i]);
    free(context->sstack->start);
    free(context->sstack);
}
void idup(struct Context * context)
{
    int val = ipop(context);
    ipush(context, val);
    ipush(context, val);
}
void imod(struct Context * context)
{
    int b = ipop(context);
    int a = ipop(context);
    
    ipush(context, a % b);
}
void ieq(struct Context * context)
{
    int b = ipop(context);
    int a = ipop(context);

    ipush(context, (a == b) ? 1 : 0);
}
void iswap(struct Context * context)
{
    int b = ipop(context);
    int a = ipop(context);

    ipush(context, b);
    ipush(context, a);
}
void concat(struct Context * context)
{
    char buffer[256];
    char * b = spop(context);
    char * a = spop(context);
    strncpy(buffer, a, 255);
    strncat(buffer, b, 255-strlen(a));
    free(a);
    free(b);
    spush(context, buffer);
}
void sdup(struct Context * context)
{
    char * val = spop(context);
    spush(context, val);
    spush(context, val);
    free(val);
}
void sdrop(struct Context * context)
{
    free(spop(context));
}
void itos(struct Context * context)
{
    int ival = ipop(context);
    char buffer[256];
    sprintf(buffer, "%d", ival);
    spush(context, buffer);
}
void ilt(struct Context * context)
{
    int b = ipop(context);
    int a = ipop(context);

    ipush(context, (a < b) ? 1 : 0);
}
void soutput(struct Context * context)
{
    char * val = spop(context);
    printf("%s", val);
    free(val);
}
void iadd(struct Context * context)
{
    int b = ipop(context);
    int a = ipop(context);
    
    ipush(context, a + b);
}
