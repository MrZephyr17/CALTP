#include <vector>
#include <algorithm>
#include "stringMatching.h"

using namespace std;

int editDistance(string pattern, string text)
{
    int pSize = pattern.size();
    int tSize = text.size();
    int oldValue, newValue;
    vector<int> D(tSize + 1);

    for (int j = 0; j <= tSize; j++)
        D.at(j) = j;

    for (int i = 1; i <= pSize; i++)
    {
        oldValue = D.at(0);
        D.at(0) = i;

        for (int j = 1; j <= tSize; j++)
        {
            if (pattern.at(i - 1) == text.at(j - 1))
                newValue = oldValue;
            else
                newValue = 1 + min({oldValue, D.at(j), D.at(j - 1)});

            oldValue = D.at(j);
            D.at(j) = newValue;
        }
    }

    return D.at(tSize);
}

vector<int> ComputePrefixFunction(string pattern)
{
    int m = pattern.size();
    vector<int> pi(m);
    int k = 0;

    for (int q = 1; q < m; q++)
    {
        while (k > 0 && pattern.at(k) != pattern.at(q))
            k = pi.at(k - 1);

        if (pattern.at(k) == pattern.at(q))
            k++;

        pi.at(q) = k;
    }

    return pi;
}

bool KMPMatcher(string text, string pattern)
{
    int m = pattern.size();
    int n = text.size();
    vector<int> pi = ComputePrefixFunction(pattern);
    int q = 0;

    for (int i = 0; i < n; i++)
    {
        while (q > 0 && pattern.at(q) != text.at(i))
            q = pi.at(q - 1);

        if (pattern.at(q) == text.at(i))
            q++;

        if (q == m)
            return true;
    }

    return false;
}