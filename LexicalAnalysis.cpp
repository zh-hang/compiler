#include "LexicalAnalysis.h"

int currentChar; // 当前标记
int lineNum = 1; // 源码行号
int charNum = 0; //字符总数

//

struct node *next(struct node *p, char *fp, int *fpos)
{
    int type, i, j;
    char value[255] = {0};
    while (1)
    {
        charNum++;
        currentChar = fp[*fpos];
        (*fpos)++;

        if (currentChar == '\'')
        {
            i = 0;
            value[i] = currentChar;
            while (fp[*fpos] != '\'')
                value[++i] = fp[(*fpos)++];
            value[++i] = fp[(*fpos)++];
            value[++i] = '\0';
            type = Char;
            break;
        }
        else if (currentChar == '"')
        {
            i = 0;
            value[i] = currentChar;
            while (fp[*fpos] != '"')
                value[++i] = fp[(*fpos)++];
            value[++i] = fp[(*fpos)++];
            value[++i] = '\0';
            type = Str;
            break;
        }

        if (currentChar == '\n')
        { //处理换行符,当前的行号加一
            lineNum++;
            continue;
        }
        else if (currentChar == ' ') //忽略空格
        {
            while (fp[(*fpos)] == ' ')
                (*fpos)++;
            continue;
        }
        else if (currentChar == 9)
        {
            continue;
        }
        else if ((currentChar >= 'a' && currentChar <= 'z') || (currentChar >= 'A' && currentChar <= 'Z') || (currentChar == '_'))
        { //关键字或者变量名
            i = 0;
            value[i] = currentChar;
            while (fp[*fpos] >= 'a' && fp[*fpos] <= 'z' || fp[*fpos] >= 'A' && fp[*fpos] <= 'Z' || fp[*fpos] >= '0' && fp[*fpos] <= '9' || fp[*fpos] == '_')
                value[++i] = fp[(*fpos)++];
            value[++i] = '\0';
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
            value[i] = currentChar;
            bool isNum = true;
            while (fp[*fpos] >= '0' && fp[*fpos] <= '9')
            {
                value[++i] = fp[(*fpos)++];
                if (isNum && fp[*fpos] == '.')
                    isNum = false;
                else if (!isNum && fp[*fpos] == '.')
                    printf("line:%d Error: Illeagle character!", lineNum);
            }
            value[++i] = '\0';
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
            break;
        case '{':
            type = Separator;
            value[0] = '{';
            break;
        case '}':
            type = Separator;
            value[0] = '}';
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
        case '?':
            value[0] = '?';
            type = Separator;
            break;
        case '~':
            value[0] = '~';
            type = Tilde;
            break;
        case '.':
            value[0] = '.';
            type = Dot;
            break;
        case '\\':
            type = Esc;
            value[0] = '\\';
            value[1] = *fp;
            (*fpos)++;
            break;
        case '#': //预处理和宏定义
            i = 0;
            value[i] = currentChar;
            if (fp[*fpos] == 'i')
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
                    i = 0;
                    while (fp[*fpos] != ' ')
                        value[++i] = fp[(*fpos)++];
                    type = Keyword;
                    break;
                }
            }
            else if (fp[*fpos] == 'd')
            {
                char de[] = "define";
                int pos = *fpos;
                for (i = 0; i < 6; i++)
                {
                    if (fp[pos + i] != de[i])
                        break;
                }
                if (i == 6)
                {
                    i = 0;
                    while (fp[*fpos] != ' ')
                        value[++i] = fp[(*fpos)++];
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
                        printf("line:%d Error: Except */.\n", lineNum);
                    else if (fp[*fpos] == '*')
                    {
                        (*fpos)++;
                        if (fp[*fpos] == '/')
                        {
                            (*fpos)++;
                            break;
                        }
                    }
                }
            }
            continue;
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
        case '\0':
            break;
        default:
            printf("line:%d Error: Illeagle character!\n", lineNum);
            type = Other;
            value[0] = currentChar;
            break;
        }
        break;
    }
    for (i = 0; i < 255; i++)
    {
        p->value[i] = value[i];
    }
    p->type = type;
    node *q = (node *)malloc(sizeof(node));
    q->next = NULL;
    p->next = q;
    return p;
}

//词法分析的入口，分析整个 C 语言程序。
void gramAnalyser(struct node *p, char *fp, FILE *outputFile)
{
    printf("The output produced by Lexcial analyzer: \n");
    int fpos = 0;
    next(p, fp, &fpos);
    // 获取下一个标记
    while (currentChar > 0)
    {
        printf("< %s , %s >\n", tokens[p->type], p->value);
        fprintf(outputFile, "< %s , %s >\n", tokens[p->type], p->value);
        p = p->next;
        next(p, fp, &fpos);
    }
    printf("The sum of character is %d\n", charNum);
    fprintf(outputFile, "The sum of character is %d\n", charNum);
    printf("Sum of the line: %d\n", lineNum);
    fprintf(outputFile, "Sum of the line: %d\n", lineNum);
    printf("\n\ndata has been written into output.txt\n\n");
}

void fileText::judgeToken()
{
    while (1)
    {
        currToken->value = text[pos];
        pos++;
        if (currToken->value[0] == '\n')
        {
            lineNum++;
            continue;
        }
        else if (currToken->value[0] == ' ')
        {
            while (text[pos] == ' ')
                pos++;
            continue;
        }
        else if (currToken->value[0] == 9)
            continue;
        else if (currToken->value[0] == '\'')
        {
            while (text[pos] != '\'')
                currToken->value += text[pos++];
            currToken->value = Char;
            break;
        }
        else if (currToken->value[0] == '"')
        {
            while (text[pos] != '"')
                currToken->value += text[pos++];
            currToken->type = Str;
            break;
        }
        else if ((currToken->value[0] >= 'a' && currToken->value[0] <= 'z') || (currToken->value[0] >= 'A' && currToken->value[0] <= 'Z') || (currToken->value[0] == '_'))
        {
            ifKeywordorId();
            break;
        }
        else if (currToken->value[0] >= '0' && currToken->value[0] <= '9')
        {
            ifDigital();
            break;
        }
        else if (currToken->value[0] == '#')
        {
            ifSharp();
            break;
        }
        else if (currToken->value[0] == '/')
        {
            ifComment();
            continue;
        }
        switch (currToken->value[0])
        {
        case '[':
        case ']':
        case '{':
        case '}':
        case '(':
        case ')':
        case ';':
        case ',':
        case '?':
            currToken->type = Separator;
        case '~':
            currToken->type = Tilde;
            break;
        case '.':
            currToken->type = Dot;
            break;
        case '*':
            currToken->type = Mul;
            break;
        case '^':
            currToken->type = Xor;
            break;
        case '%':
            currToken->type = Mod;
            break;
        case '\\':
            currToken->type = Esc;
            break;
        case '<':
            if (text[pos] == '=')
            {
                currToken->value += text[pos++];
                currToken->type = Le;
            }
            else if (text[pos] == '<')
            {
                currToken->value += text[pos++];
                currToken->type = Shl;
            }
            else
                currToken->type = Lt;
            break;
        case '>':
            if (text[pos] == '=')
            {
                currToken->value += text[pos++];
                currToken->type = Ge;
            }
            else if (text[pos] == '>')
            {
                currToken->value += text[pos++];
                currToken->type = Shr;
            }
            else
                currToken->type = Gt;
            break;
        case '=':
            if (text[pos] == '=')
            {
                currToken->value += text[pos++];
                currToken->type = Eq;
                break;
            }
            currToken->type = Assign;
            break;
        case '!':
            if (text[pos] == '=')
            {
                currToken->value += text[pos++];
                currToken->type = Ne;
                break;
            }
            currToken->type = Not;
            break;
        case '|':
            if (text[pos] == '|')
            {
                currToken->value += text[pos++];
                currToken->type = Lor;
                break;
            }
            currToken->type = Or;
            break;
        case '&':
            if (text[pos] == '&')
            {
                currToken->value += text[pos++];
                currToken->type = Lan;
                break;
            }
            currToken->type = And;
            break;
        case '+':
            if (text[pos] == '+')
            {
                currToken->value += text[pos++];
                currToken->type = Inc;
                break;
            }
            currToken->type = Add;
            break;
        case '-':
            if (text[pos] == '-')
            {
                currToken->value += text[pos++];
                currToken->type = Dec;
                break;
            }
            currToken->type = Sub;
            break;
        case '\0':
            break;
        default:
            cout << "line:" << lineNum << " Error: Illeagle character!" << endl;
            currToken->type = Other;
            break;
        }
    }
}

void fileText::tokenAnalyser()
{
    while (text[pos] > 0)
    {
        judgeToken();
        tokens->push_back(*currToken);
        cout << *currToken << endl;
        charNum++;
    }
}