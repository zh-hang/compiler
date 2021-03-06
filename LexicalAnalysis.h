#ifndef LEXICALANALYSIS_H
#define LEXICALANALYSIS_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <vector>

using namespace std;

#define SIZE 10240 //默认大小

//关键字
const char *keywords[34] = {
    "#include", "#define", "auto", "break", "case", "char", "const", "continue", "defualt", "do",
    "double", "else", "enum", "extern", "float", "for", "goto", "if",
    "int", "long", "register", "return", "short", "signed", "sizeof", "static",
    "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"};

//能够支持的标记符
const char *tokens[33] = {
    "Num", "Id", "Char", "Str", "Keyword", "Assign", "Lor", "Or", "Not", "Xor", "And",
    "Lan", "Eq", "Ne", "Lt", "Gt", "Le", "Ge", "Shl", "Shr", "Add",
    "Sub", "Mul", "Div", "Mod", "Inc", "Dec", "Separator", "Tilde", "Esc", "Sharp", "Dot", "Other"};
enum
{
    Num = 0,
    Id,
    Char,
    Str,
    Keyword,
    Assign,
    Lor,
    Or,
    Not,
    Xor,
    And,
    Lan,
    Eq,
    Ne,
    Lt,
    Gt,
    Le,
    Ge,
    Shl,
    Shr,
    Add,
    Sub,
    Mul,
    Div,
    Mod,
    Inc,
    Dec,
    Separator,
    Tilde,
    Esc,
    Sharp,
    Dot,
    Other
};

class token
{
public:
    string value;
    int type;
    token()
    {
        value = "";
        type = NULL;
    }
    friend ostream & operator<<(ostream &out,token &t){
        out<<"<"<<t.type<<","<<t.value<<">";
        return out;
    }
};

class fileText
{
    string text;
    token *currToken;
    vector<token> tokens[SIZE];
    int pos;
    int lineNum;
    int tokenNum;

public:
    fileText(string t)
    {
        text = t;
        currToken = new token();
        lineNum = 1;
        tokenNum = 0;
        pos=0;
    }
    void tokenAnalyser();
    void judgeToken();
    void ifKeywordorId();
    void ifDigital();
    void ifSharp();
    void ifComment();
};

typedef struct node
{
    int type;
    char value[255];
    struct node *next;
} node;

//获取下一个字符，p结点，fp文件字符串，fpos指针位置
struct node *next(struct node *p, char *fp, int *fpos);

void gramAnalyser(struct node *p, char *fp, FILE *outputFile);

#endif