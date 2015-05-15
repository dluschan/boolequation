#include <iostream>
#include <math.h>

using namespace std;

int main()
{
    const int n = 9;
    for (unsigned int i = 0; i < pow(2, n); ++i)
    {
        bool a[n];
        unsigned k = i;
        for (unsigned int j = 0; j < n; ++j)
        {
            a[n-j-1] = 0x1 & k;
            k >>= 1;
        }
        for (unsigned int j = 0; j < n; ++j)
        {
            cout << a[j];
        }
        cout << endl;
    }
    return 0;
}

