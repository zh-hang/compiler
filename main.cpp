#include "GrammerAnalysis.h"

int main()
{
    string testStr[] = {"(", "num", "+", "num", ")", "*", "(", "(", "num", "-", "num", ")", "/", "(", "num", "+", "num", "*", "num", ")", ")"};
    vector<string> testVector;
    for (int i = 0; i < 21; i++)
        testVector.push_back(testStr[i]);
    try
    {
        GrammerAnalysis *test = new GrammerAnalysis(testVector);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}