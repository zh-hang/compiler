#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>

#define SIZE 10240 //默认大小

typedef struct node
{
    int type;
    char value[12];
    struct node *next;
} node;

struct node *next(struct node *p, char *fp);

void gramAnalyser(struct node *p, char *fp, FILE *outputFile);