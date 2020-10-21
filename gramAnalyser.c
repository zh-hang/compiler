#include "gramAnalyser.h"

int currentChar; // 当前标记
int lineNum = 0; // 源码行号
int charNum = 0; //字符总数

char *keywords[32] = {
    "auto", "break", "case", "char", "const", "continue", "defualt", "do",
    "double", "else", "enum", "extern", "float", "for", "goto", "if",
    "int", "long", "register", "return", "short", "signed", "sizeof", "static",
    "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"};
//能够支持的标记符
char *tokens[28] = {
    "Num", "Id", "Keyword", "Assign", "Lor", "Or", "Xor", "And",
    "Lan", "Eq", "Ne", "Lt", "Gt", "Le", "Ge", "Shl", "Shr", "Add",
    "Sub", "Mul", "Div", "Mod", "Inc", "Dec", "Separator", "Tilde", "Esc", "Other"};
enum
{
    Num = 0,
    Id,
    Keyword,
    Assign,
    Lor,
    Or,
    Xor,
    Lan,
    And,
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
    Other
};

//用于词法分析，获取下一个标记，它将自动忽略空白字符。
struct node *next(struct node *p, char *fp)
{
    int num = 0, type, i, j;
    char value[12] = {0};

    while (1)
    { //跳过不识别的字符以及处理空白字符
        charNum++;
        currentChar = *fp;
        fp++;

        if (currentChar == '\n')
        { //处理换行符,当前的行号加一
            lineNum++;
            continue;
        }
        else if (currentChar == ' ')
        {
            if (*fp == ' ')
            {
                fp++;
            }
        }
        else if (currentChar == '#')
        { //处理宏定义
            // skip macro, because we will not support it
            while (*fp != 0 && *fp != '\n')
            {
                fp++;
            }
        }
        else if ((currentChar >= 'a' && currentChar <= 'z') || (currentChar >= 'A' && currentChar <= 'Z') || (currentChar == '_'))
        {
            // 解析标识符
            i = 0;
            value[i] = currentChar;
            while ((*fp >= 'a' && *fp <= 'z') || (*fp >= 'A' && *fp <= 'Z') || (*fp >= '0' && *fp <= '9') || (*fp == '_'))
            {
                value[i + 1] = *fp;
                fp++;
                i++;
            }
            value[i + 1] = '\0';
            //查询已有的标识符
            for (j = 0; j < 32; j++)
            {
                if (strcmp(value, keywords[j]) == 0)
                {
                    type = Keyword;
                    break;
                }
            }
            if (type != Keyword)
            {
                type = Id;
            }
            break;
        }
        //数字
        //在ASCII码中，字符a对应的十六进制值是 61, A是41，故通过 (token & 15) 可以得到个位数的值。
        else if (currentChar >= '0' && currentChar <= '9')
        {
            int _num;
            _num = currentChar - '0';
            value[0] = currentChar;
            i = 1;
            if (_num > 0)
            {
                for (i; *fp >= '0' && *fp <= '9'; i++)
                {
                    value[i] = *fp;
                    _num = _num * 10 + *fp++ - '0';
                }
            }
            else
            {
                _num = '0';
            }
            type = Num;
            value[i] = '\0';
            break;
        }
        //注释
        else if (currentChar == '/')
        {
            if (*fp == '/')
            {
                // skip comments
                while (*fp != 0 && *fp != '\n')
                {
                    fp++;
                }
            }
            else if (*fp == '*')
            {
                while (*fp != 0)
                {
                    fp++;
                    if (*fp == '*')
                    {
                        fp++;
                        if (*fp == '/')
                            break;
                    }
                }
                fp++;
            }
            else
            {
                // divide operator
                type = Div;
                value[0] = currentChar;
                break;
            }
        }
        //其他
        else if (currentChar == '+')
        {
            // parse '+' and '++'
            if (*fp == '+')
            {
                fp++;
                type = Inc;
                value[0] = value[1] = '+';
                break;
            }
            else
            {
                type = Add;
                value[0] = '+';
                break;
            }
        }
        else if (currentChar == '-')
        {
            // parse '-' and '--'
            if (*fp == '-')
            {
                fp++;
                type = Dec;
                value[0] = value[1] = '-';
                break;
            }
            else
            {
                type = Sub;
                value[0] = '-';
                break;
            }
        }
        else if (currentChar == '*')
        {
            // key=43;
            type = Mul;
            value[0] = '*';
            break;
        }
        else if (currentChar == '=')
        {
            // parse '==' and '='
            if (*fp == '=')
            {
                fp++;
                type = Eq;
                value[0] = value[1] = '=';
                break;
            }
            else
            {
                //  key=47;
                type = Assign;
                value[0] = '=';
                break;
            }
        }
        else if (currentChar == '!')
        {
            // parse '!='
            if (*fp == '=')
            {
                fp++;
                //  key=48;
                type = Ne;
                value[0] = '!';
                value[1] = '=';
                break;
            }
        }
        else if (currentChar == '<')
        {
            // parse '<=', '<<' or '<'
            if (*fp == '=')
            {
                fp++;
                //  key=49;
                type = Le;
                value[0] = '<';
                value[1] = '=';
                break;
            }
            else if (*fp == '<')
            {
                fp++;
                // key=50;
                type = Shl;
                value[0] = value[1] = '<';
                break;
            }
            else
            {
                // key=51;
                type = Lt;
                value[0] = '<';
                break;
            }
        }
        else if (currentChar == '>')
        {
            // parse '>=', '>>' or '>'
            if (*fp == '=')
            {
                fp++;
                //  key=52;
                type = Ge;
                value[0] = '>';
                value[1] = '=';
                break;
            }
            else if (*fp == '>')
            {
                fp++;
                //  key=53;
                type = Shr;
                value[0] = value[1] = '>';
                break;
            }
            else
            {
                //   key=54;
                type = Gt;
                value[0] = '>';
                break;
            }
        }
        else if (currentChar == '|')
        {
            // parse '|' or '||'
            if (*fp == '|')
            {
                fp++;
                //  key=55;
                type = Lor;
                value[0] = value[1] = '|';
                break;
            }
            else
            {
                //  key=56;
                type = Or;
                value[0] = '|';
                break;
            }
        }
        else if (currentChar == '&')
        {
            // parse '&' and '&&'
            if (*fp == '&')
            {
                fp++;
                //  key=58;
                type = Lan;
                value[0] = value[1] = '&';
                break;
            }
            else
            {
                // key=59;
                type = And;
                value[0] = '&';
                break;
            }
        }
        else if (currentChar == '^')
        {
            //key=60;
            type = Xor;
            value[0] = '^';
            break;
        }
        else if (currentChar == '%')
        {
            // key=61;
            type = Mod;
            value[0] = '%';
            break;
        }
        else if (currentChar == '[')
        {
            // key=62;
            type = Separator;
            value[0] = '[';
            break;
        }
        else if (currentChar == '?')
        {
            //  key=63;
            type = Other;
            value[0] = '?';
            break;
        }
        else if (currentChar == '~')
        {
            type = Tilde;
            value[0] = '~';
            break;
        }
        else if (currentChar == ';' || currentChar == '{' || currentChar == '}' || currentChar == '(' || currentChar == ')' || currentChar == ']' || currentChar == ',' || currentChar == '"' || currentChar == '\'')
        {
            type = Separator;
            value[0] = currentChar;
            break;
        }
        else if (currentChar == '\\')
        {
            type = Esc;
            value[0] = '\\';
            value[1] = *fp;
            fp++;
            break;
        }
        else if (currentChar == 9)
        {
            continue;
        }

        else if (currentChar == 0)
            break;
        else
        {
            printf("Error occurred in line%d!\n", lineNum);
            //printf("%c\n",*src);
            //printf("%c\n",*src--);
            type = Other;
            value[0] = currentChar;
            break;
        }
    }
    for (i = 0; i < 12; i++)
    {
        p->value[i] = value[i];
    }
    p->type = type;
    node *q = (node *)malloc(sizeof(node));
    q->next = NULL;
    p->next = q;
    return p;
}

//语法分析的入口，分析整个 C 语言程序。
void gramAnalyser(struct node *p, char *fp, FILE *outputFile)
{
    printf("The output produced by Lexcial analyzer: \n");
    p = next(p, fp);
    // 获取下一个标记
    while (currentChar > 0)
    {
        printf("< %s , %s >\n", tokens[p->type], p->value);
        fprintf(outputFile, "< %s , %s >\n", tokens[p->type], p->value);
        p = p->next;
        next(p, fp);
    }
    printf("The sum of character is %d\n", charNum);
    fprintf(outputFile, "The sum of character is %d\n", charNum);
    printf("Sum of the line: %d\n", lineNum);
    fprintf(outputFile, "Sum of the line: %d\n", lineNum);
    printf("\n\ndata has been written into output.txt\n\n");
}
