#include <iostream>
#include <math.h>

using namespace std;

int main()
{
    const int n = 9;
    for (unsigned int i = 0; i < pow(2, n); ++i)
    {
        bool x[n];
        unsigned k = i;
        for (unsigned int j = 0; j < n; ++j)
        {
            x[n-j-1] = 0x1 & k;
            k >>= 1;
        }
        int f = 1;
        for (unsigned int j = 0; j < n-2; ++j)
        {
            if ((x[j] != x[j+1]) && (x[j] != x[j+2]))
            {
                f = 0;
                break;
            }
        }
        if (f)
        {
            for (unsigned int j = 0; j < n; ++j)
                cout << x[j];
            cout << endl;
        }
    }
    return 0;
}

