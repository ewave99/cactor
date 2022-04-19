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

void printIntStack(struct Context *);
int ipop(struct Context *);
void ipush(struct Context *, int);

void idup(struct Context *);
void iswap(struct Context *);
void idrop(struct Context *);

void itos(struct Context *);
void itoc(struct Context *);
void ioutput(struct Context *);

void iadd(struct Context *);
void isubtract(struct Context *);
void imultiply(struct Context *);
void idivide(struct Context *);
void imod(struct Context *);
void ieq(struct Context *);
void ine(struct Context *);
void igt(struct Context *);
void ige(struct Context *);
void ilt(struct Context *);
void ile(struct Context *);

void printStringStack(struct Context *);
const char * spop(struct Context *);
void spush(struct Context *, const char *);

void sswap(struct Context *);
void sdup(struct Context *);
void sdrop(struct Context *);
void soutput(struct Context *);
void concat(struct Context *);
void headtail(struct Context *);
void ctoi(struct Context *);
void len(struct Context *);
