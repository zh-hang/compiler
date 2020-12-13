#ifndef GRAMAERANALYSIS_H
#define GRAMAERANALYSIS_H

#include <iostream>
#include <string>
#include <vector>

#define MAX 255
#define TERM_NUM 9
#define NON_TERM_NUM 5

using namespace std;

class Term
{
public:
    string *term;

    Term()
    {
        string data[] = {"+", "-", "*", "/", "num", "(", ")", "epsilon", "$"};
        term = new string[TERM_NUM];
        for (int i = 0; i < TERM_NUM; i++)
        {
            term[i] = data[i];
        }
    }
    bool isContain(string t)
    {
        int i = 0;
        while (i < TERM_NUM)
        {
            if (t == this->term[i++])
                return true;
        }
        return false;
    }
    bool isContain(char c)
    {
        int i = 0;
        while (i < TERM_NUM)
        {
            if (c == this->term[i++][0])
                return true;
        }
        return false;
    }
    int getTermPos(string t)
    {
        for (int i = 0; i < TERM_NUM; i++)
        {
            if (t == this->term[i])
            {
                return i;
            }
        }
        return TERM_NUM + 1;
    }
    string getTerm(char c)
    {
        switch (c)
        {
        case '+':
            return "+";
        case '-':
            return "-";
        case '*':
            return "*";
        case '/':
            return "/";
        case '(':
            return "(";
        case ')':
            return ")";
        case 'n':
            return "num";
        case 'e':
            return "epsilon";
        case '$':
            return "$";
        default:
            return "";
        }
    }
};

class NonTerm
{
public:
    string *nonTerm;
    NonTerm()
    {
        string data[] = {"E", "Eq", "T", "Tq", "F"};
        this->nonTerm = new string[NON_TERM_NUM];
        for (int i = 0; i < NON_TERM_NUM; i++)
        {
            this->nonTerm[i] = data[i];
        }
    }
    bool isContain(string t)
    {
        int i = 0;
        while (i < NON_TERM_NUM)
        {
            if (t == this->nonTerm[i++])
                return true;
        }
        return false;
    }
    int getNonTermPos(string t)
    {
        for (int i = 0; i < NON_TERM_NUM; i++)
        {
            if (t == this->nonTerm[i])
            {
                return i;
            }
        }
        return NON_TERM_NUM + 1;
    }
};

class Rule
{
public:
    vector<string> rule;
    string firstE;
    string firstEq;
    string firstT;
    string firstTq;
    string firstF;
    string followE;
    string followEq;
    string followT;
    string followTq;
    string followF;

    Rule()
    {
        rule.push_back("ETEq");
        rule.push_back("Eq+TEq");
        rule.push_back("Eq-TEq");
        rule.push_back("Eqepsilon");
        rule.push_back("TFTq");
        rule.push_back("Tq*FTq");
        rule.push_back("Tq/FTq");
        rule.push_back("Tqeplison");
        rule.push_back("F(E)");
        rule.push_back("Fnum");
        constructFirst();
        constructFollow();
    }

    inline string getRuleFirstChar(string R);
    inline string getFirst(string word);
    string getFollow(string word);
    vector<string> separate(string str);
    string getFirstbyWord(string word)
    {
        string res = getFirst(word);
        return res == "" ? word : res;
    }

private:
    void constructFirst();
    void constructFollow();
    void addFollow(string &base, string add)
    {
        vector<string> addV = separate(add);
        bool ifAdd;
        for (int i = 0; i < addV.size(); i++)
        {
            ifAdd = true;
            for (int j = 0; j < base.length(); j++)
            {
                if (addV[i][0] == base[j])
                {
                    ifAdd = false;
                    break;
                }
            }
            base += (ifAdd ? addV[i] : "");
        }
    }
    void ridEpsilon(string &follow)
    {
        for (int i = 0; i < follow.length(); i++)
            if (follow[i] == 'e')
                follow.erase(follow.begin() + i, follow.begin() + i + 7);
    }
    string toString(char C)
    {
        string res(1, C);
        return res;
    }
};

class GrammerAnalysis
{
private:
    string PredictionTable[NON_TERM_NUM][TERM_NUM];
    vector<string> stack;
    vector<string> buffer;
    Term *term;
    NonTerm *nonTerm;
    string res;
    Rule rule;

public:
    GrammerAnalysis()
    {
        init();
    }

    GrammerAnalysis(vector<string> omiga)
    {
        init();
        analysis(omiga);
    }

    void analysis(vector<string> omiga);

    string getRes()
    {
        return this->res;
    }

private:
    void init()
    {
        this->term = new Term();
        this->nonTerm = new NonTerm();
        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                PredictionTable[i][j] = "";
            }
        }
        buildTable();
    }
    void buildTable();
    void addTable(string res, string A, string R);
};

#endif