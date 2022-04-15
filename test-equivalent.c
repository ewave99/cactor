#include "builtins.h"

void fizzbuzz_replace(struct Context * context)
{
    idup(context);
    ipush(context, 3);
    imod(context);
    ipush(context, 0);
    ieq(context);
    iswap(context);
    ipush(context, 5);
    imod(context);
    ipush(context, 0);
    ieq(context);
    iswap(context);
    if (ipop(context))
    {
        spush(context, "Fizz");
    }
    else
    {
        spush(context, "");
    }
    if (ipop(context))
    {
        spush(context, "Buzz");
    }
    else
    {
        spush(context, "");
    }
    idup(context);
    concat(context);
    sdup(context);
    if (!*spop(context))
    {
        sdrop(context);
        idup(context);
        itos(context);
    }
}

void fizzbuzz(struct Context * context)
{
    const int limit = 20;
    ipush(context, 0);
    idup(context);
    ipush(context, limit);
    ilt(context);
    while (ipop(context))
    {
        idup(context);
        fizzbuzz_replace(context);
        soutput(context);
        ipush(context, 1);
        iadd(context);
        ipush(context, limit);
        ilt(context);
    }
}

int main()
{
    struct Context * context = init();
    fizzbuzz(context);
    return 0;
}
