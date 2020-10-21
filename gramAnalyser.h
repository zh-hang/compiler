#ifndef GRAMANALYSER_H
#define GRAMANALYSER_H

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

//获取下一个字符，p结点，fp文件字符串，fpos指针位置
struct node *next(struct node *p, char*fp,int *fpos);

void gramAnalyser(struct node *p, char *fp, FILE *outputFile);
#endif