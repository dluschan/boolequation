#include <iostream>
#include <map>
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

class Expr
{
public:
	Expr(){}
	virtual operator double() const = 0;
};

typedef shared_ptr<Expr> PExpr;

class PlusToken: public Expr
{
public:
	PlusToken(PExpr pLeft, PExpr pRight)
		: left (pLeft )
		, right(pRight)
	{}
	operator double() const
	{
		return *left + *right;
	}

private:
	PExpr left;
	PExpr right;
};

class MulToken: public Expr
{
public:
	MulToken(PExpr pLeft, PExpr pRight)
		: left (pLeft )
		, right(pRight)
	{}
	operator double() const
	{
		return *left * *right;
	}

private:
	PExpr left;
	PExpr right;
};

class ValueToken: public Expr
{
public:
	ValueToken(double v)
		: value(v)
	{}
	operator double() const
	{
		return value;
	}

private:
	double value;
};

enum Token_value
{
	NAME, NUMBER, END,
	PLUS = '+', MINUS = '-', MUL = '*', DIV = '/',
	PRINT = ';', ASSIGN = '=', LP = '(', RP = ')'
};

Token_value curr_tok = PRINT;
double number_value;
string string_value;
map<string, double> table;
int no_of_errors;

PExpr expr(bool);

int error(const string& s)
{
	++no_of_errors;
	cerr << "ошибка: " << s << endl;
	return 1;
}

Token_value get_token()
{
	char ch = 0;
	cin >> ch;
	switch(ch)
	{
	case 0:
		return curr_tok = END;
	case ';':
	case '*':
	case '/':
	case '+':
	case '-':
	case '(':
	case ')':
	case '=':
		return curr_tok = Token_value(ch);
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
	case '.':
		cin.putback(ch);
		cin >> number_value;
		return curr_tok = NUMBER;
	default:
		if (isalpha(ch))
		{
			cin.putback(ch);
			cin >> string_value;
			return curr_tok = NAME;
		}
		error("неправильная лексема");
		return curr_tok = PRINT;
	}
}

PExpr prim(bool get)
{
	if (get)
		get_token();

	switch(curr_tok)
	{
	case NUMBER:
	{
		double v = number_value;
		get_token();
		return make_shared<ValueToken>(v);
	}
	case LP:
	{
		PExpr e = expr(true);
		if (curr_tok != RP)
			error("ожидалась )");
		get_token();
		return e;
	}
	default:
		return PExpr();
	}
}

PExpr term(bool get)
{
	PExpr pLeft = prim(get);
	while (true)
	{
		switch(curr_tok)
		{
		case MUL:
		{
			PExpr pRight = prim(true);
			pLeft = std::make_shared<MulToken>(pLeft, pRight);
			break;
		}
		default:
			return pLeft;
		}
	}
}

PExpr expr(bool get)
{
	PExpr pLeft = term(get);
	while(true)
	{
		switch(curr_tok)
		{
		case PLUS:
		{
			PExpr pRight = term(true);
			pLeft = make_shared<PlusToken>(pLeft, pRight);
			break;
		}
		default:
			return pLeft;
		}
	}
}

void compile()
{
	freopen("input.txt", "r", stdin);
	while(cin)
	{
		get_token();
		if (curr_tok == END)
			break;
		if (curr_tok == PRINT)
			continue;
		cout << *expr(false) << endl;
	}
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
			cout << "интерактивный режим еще не поддерживается, пока запускается просто калькулятор" << endl;
			compile();
			return(10);
		}
		cout << "неизвестный параметр" << endl;
		return(1);
	default:
		cout << "слишком много параметров" << endl;
		return(2);
	}
}
