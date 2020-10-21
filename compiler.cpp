#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include "gramAnalyser.h"
// #define SIZE 10240 //默认大小
int main()
{
    int i;
    FILE *inputFile, *outputFile;
    char *fileName = (char *)malloc(sizeof(char) * 50);
    struct node *head;
    char *fp;
    int poolSize;
    head = (node *)malloc(sizeof(node));
    head->next = NULL;
    printf("***************************\n");
    printf("*****Lexcial analyzer******\n");
    printf("***************************\n");
    printf("\n");
    printf("Input the filename with postfix: ");
    scanf("%s", fileName);
    char path[60] = "simple/";
    i = 0;
    while (*fileName != '\0')
        path[i++ + 7] = *fileName++;
    i = 0;
    if ((inputFile = fopen(path, "r")) == NULL)
    {
        printf("could not open(%s)\n", path);
        return -1;
    }
    if ((outputFile = fopen("simple/output.txt", "w")) == NULL)
    {
        printf("could not open(%s)\n", "simple/output.txt");
        return -1;
    }
    if (!(fp = (char *)malloc(SIZE)))
    {
        printf("could not malloc(%d) for source area\n", poolSize);
        fprintf(outputFile, "could not malloc(%d) for source area\n", poolSize);
        return -1;
    }
    // 读取源文件
    if ((i = fread(fp, 1, SIZE, inputFile)) <= 0)
    {
        printf("read() returned %d\n", i);
        fprintf(outputFile, "read() returned %d\n", i);
        return -1;
    }
    fp[i] = 0; // 添加EOF字符
    fclose(inputFile);
    gramAnalyser(head, fp, outputFile);
    system("pause");
    fclose(outputFile);
    return 0;
}