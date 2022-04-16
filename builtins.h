struct IntStack
{
    unsigned int length;
    int * start;
    int * end;
};

struct StringStack
{
    unsigned int length;
    char ** start;
    char ** end;
};

struct Context
{
    struct IntStack * istack;
    struct StringStack * sstack;
};

struct Context * init();
void freeContext(struct Context *);
void idup(struct Context *);
void ipush(struct Context *, int);
void imod(struct Context *);
void ieq(struct Context *);
void iswap(struct Context *);
int ipop(struct Context *);
void spush(struct Context *, const char *);
void concat(struct Context *);
void sdup(struct Context *);
const char * spop(struct Context *);
void sdrop(struct Context *);
void itos(struct Context *);
void ilt(struct Context *);
void soutput(struct Context *);
void iadd(struct Context *);
