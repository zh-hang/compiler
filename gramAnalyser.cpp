#include "gramAnalyser.h"

int currentChar; // 当前标记
int lineNum = 0; // 源码行号
int charNum = 0; //字符总数

char *keywords[34] = {
    "#include", "#define", "auto", "break", "case", "char", "const", "continue", "defualt", "do",
    "double", "else", "enum", "extern", "float", "for", "goto", "if",
    "int", "long", "register", "return", "short", "signed", "sizeof", "static",
    "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"};
//能够支持的标记符
char *tokens[31] = {
    "Num", "Id", "Str", "Keyword", "Assign", "Lor", "Or", "Not", "Xor", "And",
    "Lan", "Eq", "Ne", "Lt", "Gt", "Le", "Ge", "Shl", "Shr", "Add",
    "Sub", "Mul", "Div", "Mod", "Inc", "Dec", "Separator", "Tilde", "Esc", "Sharp", "Other"};
enum
{
    Num = 0,
    Id,
    Str,
    Keyword,
    Assign,
    Lor,
    Or,
    Not,
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
    Sharp,
    Other
};

struct node *next(struct node *p, char *fp, int *fpos)
{
    int type, i, j;
    char value[12] = {0};
    while (1)
    {
        charNum++;
        currentChar = fp[*fpos];
        (*fpos)++;

        if (currentChar == '\n' || 9)
        { //处理换行符,当前的行号加一
            lineNum++;
            continue;
        }
        else if ((currentChar >= 'a' && currentChar <= 'z') || (currentChar >= 'A' && currentChar <= 'Z') || (currentChar == '_'))
        { //关键字或者变量名
            i = 0;
            value[i] = currentChar;
            while (fp[*fpos] >= 'a' && fp[*fpos] <= 'z' || fp[*fpos] >= 'A' && fp[*fpos] <= 'Z' || fp[*fpos] >= '0' && fp[*fpos] <= '9' || fp[*fpos] == '_')
                value[++i] = fp[(*fpos)++];
            value[i] = '\0';
            for (j = 0; j < 34; j++)
            {
                if (strcmp(value, keywords[j]) == 0)
                {
                    type = Keyword;
                    break;
                }
            }
            if (type != Keyword)
                type = Id;
            break;
        }
        else if (currentChar >= '0' && currentChar <= '9')
        { //数字
            i = 0;
            value[i++] = currentChar;
            bool isNum = true;
            while (fp[*fpos] >= '0' && fp[*fpos] <= '9')
            {
                value[++i] = fp[(*fpos)++];
                if (isNum && fp[*fpos] == '.')
                    isNum = false;
                else if (!isNum && fp[*fpos] == '.')
                    printf("line:%d Error: illegible etymology", lineNum);
            }
            value[i] = '\0';
            type = Num;
            break;
        }

        switch (currentChar)
        {
        case 9:
            break;
        case '*':
            type = Mul;
            value[0] = '*';
            break;
        case '^':
            type = Xor;
            value[0] = '^';
            break;
        case '%':
            type = Mod;
            value[0] = '%';
            break;
        case '[':
            type = Separator;
            value[0] = '[';
            break;
        case ']':
            type = Separator;
            value[0] = ']';
            break;
        case ';':
            type = Separator;
            value[0] = ';';
        case '{':
            type = Separator;
            value[0] = '{';
            break;
        case '}':
            type = Separator;
            value[0];
            break;
        case '(':
            value[0] = '(';
            type = Separator;
            break;
        case ')':
            value[0] = ')';
            type = Separator;
            break;
        case ',':
            value[0] = ',';
            type = Separator;
            break;
        case '.':
            value[0] = '.';
            type = Separator;
            break;
        case '?':
            value[0] = '?';
            type = Separator;
            break;
        case '~':
            value[0] = '~';
            type = Tilde;
        case '\\':
            type = Esc;
            value[0] = '\\';
            value[1] = *fp;
            (*fpos)++;
            break;
        case ' ': //忽略空格
            while (fp[*fpos] != ' ')
                (*fpos)++;
            break;
        case '#': //预处理和宏定义
            i = 0;
            value[i] = currentChar;
            if (fp[*fpos] == "i")
            {
                char in[] = "include";
                int pos = *fpos;
                for (i = 0; i < 7; i++)
                {
                    if (fp[pos + i] != in[i])
                        break;
                }
                if (i == 7)
                {
                    while (fp[(*fpos)++] != e)
                    {
                        i++;
                        value[i] = fp[*fpos];
                    }
                    type = Keyword;
                    break;
                }
            }
            else if (fp[*fpos] == "d")
            {
                char de[] = "include";
                int pos = *fpos;
                for (i = 0; i < 6; i++)
                {
                    if (fp[pos + i] != in[i])
                        break;
                }
                if (i == 6)
                {
                    while (fp[(*fpos)++] != e)
                    {
                        i++;
                        value[i] = fp[*fpos];
                    }
                    type = Keyword;
                    break;
                }
            }
            type = Sharp;
            break;
        case '/': //判断是否是注释
            if (fp[*fpos] == '/')
                while (fp[(*fpos)++] != '\0' && fp[(*fpos)++] != '\n')
                    ;
            else if (fp[*fpos] == '*')
            {
                while (fp[(*fpos)++] != '\0')
                {
                    if (fp[*fpos] == '\0')
                        printf("line:%d Error: Except */\n", lineNum);
                    else if (fp[*fpos] == '*')
                    {
                        if (fp[(*fpos) + 1] == '/')
                            break;
                        else
                            (*fpos)++;
                    }
                }
            }
            break;
        case '<':
            if (fp[*fpos] == '=')
            {
                (*fpos)++;
                type = Le;
                value[0] = '<';
                value[1] = '=';
            }
            else if (fp[*fpos] == '<')
            {
                (*fpos)++;
                type = Shl;
                value[0] = value[1] = '<';
            }
            else
            {
                type = Lt;
                value[0] = '<';
            }
            break;
        case '>':
            if (fp[*fpos] == '=')
            {
                (*fpos)++;
                type = Le;
                value[0] = '>';
                value[1] = '=';
            }
            else if (fp[*fpos] == '>')
            {
                (*fpos)++;
                type = Shl;
                value[0] = value[1] = '>';
            }
            else
            {
                type = Lt;
                value[0] = '>';
            }
            break;
        case '=':
            if (fp[*fpos] == '=')
            {
                (*fpos)++;
                type = Eq;
                value[0] = value[1] = '=';
            }
            else
            {
                type = Assign;
                value[0] = '=';
            }
            break;
        case '!':
            if (fp[*fpos] == '=')
            {
                (*fpos)++;
                type = Ne;
                value[0] = '!';
                value[1] = '=';
            }
            else
            {
                type = Not;
                value[0] = '!';
            }
            break;
        case '|':
            if (fp[*fpos] == '|')
            {
                (*fpos)++;
                type = Lor;
                value[0] = value[1] = '|';
            }
            else
            {
                type = Or;
                value[0] = '|';
            }
            break;
        case '&':
            if (fp[*fpos] == '&')
            {
                (*fpos)++;
                type = Lan;
                value[0] = value[1] = '&';
            }
            else
            {
                type = And;
                value[0] = '&';
            }
        case '+':
            if (fp[*fpos] == '+')
            {
                (*fpos)++;
                type = Inc;
                value[0] = value[1] = '+';
            }
            else
            {
                type = Add;
                value[0] = '+';
            }
        case '-':
            if (fp[*fpos] == '-')
            {
                (*fpos)++;
                type = Dec;
                value[0] = value[1] = '-';
            }
            else
            {
                type = Sub;
                value[0] = '-';
            }
        default:
            printf("Error occurred in line%d!\n", lineNum);
            //printf("%c\n",*src);
            //printf("%c\n",*src--);
            type = Other;
            value[0] = currentChar;
            break;
        }
        break;
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
void gramAnalyser(struct node *p, char fp, FILE *outputFile)
{
    printf("The output produced by Lexcial analyzer: \n");
    int fpos = 0;
    next(p, &fp, &fpos);
    // 获取下一个标记
    while (currentChar > 0)
    {
        printf("< %s , %s >\n", tokens[p->type], p->value);
        fprintf(outputFile, "< %s , %s >\n", tokens[p->type], p->value);
        p = p->next;
        next(p, &fp, &fpos);
    }
    printf("The sum of character is %d\n", charNum);
    fprintf(outputFile, "The sum of character is %d\n", charNum);
    printf("Sum of the line: %d\n", lineNum);
    fprintf(outputFile, "Sum of the line: %d\n", lineNum);
    printf("\n\ndata has been written into output.txt\n\n");
}
