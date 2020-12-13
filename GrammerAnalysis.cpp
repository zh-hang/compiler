#include "GrammerAnalysis.h"

inline string Rule::getRuleFirstChar(string R)
{
    return R[1] == 'q' ? R.substr(0, 2) : R.substr(0, 1);
}

inline string Rule::getFirst(string word)
{
    switch (word[0])
    {
    case 'E':
        return word[1] == 'q' ? this->firstEq : this->firstE;
    case 'T':
        return word[1] == 'q' ? this->firstTq : this->firstT;
    case 'F':
        return this->firstF;
    default:
        break;
    }
    return "";
}

string Rule::getFollow(string word)
{
    switch (word[0])
    {
    case 'E':
        return word[1] == 'q' ? this->followEq : this->followE;
    case 'T':
        return word[1] == 'q' ? this->followTq : this->followT;
    case 'F':
        return this->followF;
    default:
        break;
    }
    return "";
}

void Rule::constructFirst()
{
    Term term;
    NonTerm nonTerm;
    int pos;
    for (int i = 0; i < this->rule.size(); i++)
    {
        pos = (rule[i][1] == 'q' ? 2 : 1);
        if (term.isContain(this->rule[i][pos]))
        {
            switch (nonTerm.getNonTermPos(rule[i].substr(0, pos)))
            {
            case 0:
                this->firstE += term.getTerm(this->rule[i][pos]);
                this->firstT += term.getTerm(this->rule[i][pos]);
                this->firstF += term.getTerm(this->rule[i][pos]);
                break;
            case 1:
                this->firstEq += term.getTerm(this->rule[i][pos]);
                break;
            case 2:
                this->firstT += term.getTerm(this->rule[i][pos]);
                this->firstE += term.getTerm(this->rule[i][pos]);
                this->firstF += term.getTerm(this->rule[i][pos]);
                ;
                break;
            case 3:
                this->firstTq += term.getTerm(this->rule[i][pos]);
                break;
            case 4:
                this->firstF += term.getTerm(this->rule[i][pos]);
                this->firstE += term.getTerm(this->rule[i][pos]);
                this->firstT += term.getTerm(this->rule[i][pos]);
                break;
            default:
                break;
            }
        }
    }
    cout << "FIRST:\n";
    cout << this->firstE << '\n';
    cout << this->firstEq << '\n';
    cout << this->firstT << '\n';
    cout << this->firstTq << '\n';
    cout << this->firstF << '\n';
}

void Rule::constructFollow()
{
    this->followE.push_back('$');
    this->followT += this->firstEq;
    this->followF += this->firstTq;
    this->followE.push_back(')');
    ridEpsilon(this->followE);
    ridEpsilon(this->followT);
    ridEpsilon(this->followF);
    addFollow(this->followEq, this->followE);
    addFollow(this->followT, this->followE);
    addFollow(this->followT, this->followEq);
    addFollow(this->followTq, this->followT);
    addFollow(this->followF, this->followT);
    addFollow(this->followF, this->followTq);
    cout << "FOLLOW:\n";
    cout << this->followE << '\n';
    cout << this->followEq << '\n';
    cout << this->followT << '\n';
    cout << this->followTq << '\n';
    cout << this->followF << '\n';
}

vector<string> Rule::separate(string str)
{
    vector<string> res;
    for (int i = 0; i < str.length(); i++)
    {
        switch (str[i])
        {
        case 'E':
        case 'T':
        case 'F':
        {
            if (i + 1 < str.length() && str[i + 1] == 'q')
            {
                res.push_back(toString(str[i]) + "q");
                i++;
            }
            else
                res.push_back(toString(str[i]));
        }

        break;
        case '*':
        case '/':
        case '+':
        case '-':
            res.push_back(toString(str[i]));
            break;
        case 'n':
        {
            i += 2;
            res.push_back("num");
        }
        break;
        case 'e':
        {
            while (str[i++] != 'n')
                ;
            res.push_back("epsilon");
        }
        break;
        case '(':
            res.push_back("(");
            break;
        case ')':
            res.push_back(")");
            break;
        case '$':
            res.push_back("$");
            break;
        default:
            break;
        }
    }
    return res;
}

void GrammerAnalysis::addTable(string res, string A, string R)
{
    if (R == "")
        return;
    vector<string> rf = this->rule.separate(res);
    int x = this->nonTerm->getNonTermPos(A);
    for (int i = 0; i < rf.size(); i++)
    {
        int y = this->term->getTermPos(rf[i]);
        this->PredictionTable[this->nonTerm->getNonTermPos(A)][this->term->getTermPos(rf[i])] = R;
    }
}

void GrammerAnalysis::buildTable()
{
    for (int i = 0; i < this->rule.rule.size(); i++)
    {
        string A = this->rule.getRuleFirstChar(this->rule.rule[i]);
        string currFirst = this->rule.getFirstbyWord(this->rule.separate(this->rule.rule[i])[1]);
        addTable(currFirst, A, this->rule.rule[i]);
        for (int j = 0; j < currFirst.length(); j++)
            if (currFirst[j] == 'e')
            {
                addTable(this->rule.getFollow(this->rule.rule[i]), A, this->rule.rule[i]);
                break;
            }
    }
    cout << "Table:\n";
    string *p = *this->PredictionTable;
    int c = 0;
    int counter = 0;
    for (int i = 0; i < TERM_NUM; i++)
    {
        cout.width(12);
        cout.setf(ios::left);
        cout << this->term->term[i];
    }
    cout << '\n';
    while (counter < 45)
    {
        cout.width(12);
        cout.setf(ios::left);
        cout.left;
        cout << *(p++);
        c = (c + 1) % TERM_NUM;
        ++counter;
        if (c == 0)
            cout << '\n';
    }
}

void GrammerAnalysis::analysis(vector<string> omiga)
{   
    this->stack.push_back("$");
    this->stack.push_back("E");
    omiga.push_back("$");
    int i = 0;
    string X;
    string a;
    Term term;
    NonTerm nonTerm;
    exception e;
    do
    {
        cout << "\nstack: ";
        for (int n = 0; n < this->stack.size(); n++)
            cout << stack[n] << ' ';
        X = this->stack.back();
        a = omiga[i];
        cout << "\tinput:" << a;
        if (this->term->isContain(X))
        {
            if (X == a)
            {
                this->stack.pop_back();
                i++;
                continue;
            }
            throw e;
        }
        if (this->PredictionTable[nonTerm.getNonTermPos(X)][term.getTermPos(a)] != "")
        {
            this->stack.pop_back();
            vector<string> res = this->rule.separate(this->PredictionTable[nonTerm.getNonTermPos(X)][term.getTermPos(a)]);
            cout << "\tgeneral: " << res[0] << " -> ";
            for (int m = 1; m < res.size(); m++)
                cout << res[m] << ' ';
            while (res.size() != 1)
            {
                if (res.back() != "epsilon")
                    this->stack.push_back(res.back());
                res.pop_back();
            }
            continue;
        }
        throw e;
    } while (i < omiga.size());
}
