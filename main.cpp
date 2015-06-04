#include <iostream>
#include <math.h>

using namespace std;

const int n = 9;

bool fun(bool *x)
{
	for (unsigned int j = 0; j < n-2; ++j)
		if ((x[j] != x[j+1]) && (x[j] != x[j+2]))
			return false;
	return true;
}

void overrun()
{
	for (unsigned int i = 0; i < pow(2, n); ++i)
	{
		bool x[n];
		unsigned k = i;
		for (unsigned int j = 0; j < n; ++j)
		{
			x[n-j-1] = 0x1 & k;
			k >>= 1;
		}
		if (fun(x))
		{
			for (unsigned int j = 0; j < n; ++j)
				cout << x[j];
			cout << endl;
		}
	}
}

int main(int argc, char* argv[])
{
	switch(argc)
	{
	case 1:
		cout << "запуск без параметров. выбран режим работы по умолчанию." << endl;
		overrun();
		return 0;
	case 2:
		if (!strcmp(argv[1],"-d"))
		{
			cout << "режим работы по умолчанию" << endl;
			overrun();
			return 0;
		}
		if (!strcmp(argv[1],"-i"))
		{
			cout << "интерактивный режим еще не поддерживается" << endl;
			return(10);
		}
		cout << "неизвестный параметр" << endl;
		return(1);
	default:
		cout << "слишком много параметров" << endl;
		return(2);
	}
}
