#include "builtins.h"

void pactor_main(struct Context * context)
{
    spush(context, "Hello!");
    spush(context, "Goodbye!");
    spush(context, "\n");
    concat(context);
    concat(context);
    soutput(context);
}

int main()
{
    struct Context * context = init();

    pactor_main(context);

    freeContext(context);
    return 0;
}
