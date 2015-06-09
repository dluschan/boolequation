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
	virtual operator bool() const = 0;
};
typedef shared_ptr<Expr> PExpr;

class Equality: public Expr
{
public:
	Equality(PExpr pLeft, PExpr pRight)
		: left (pLeft )
		, right(pRight)
	{}
	operator bool() const
	{
		return *left == *right;
	}

private:
	PExpr left;
	PExpr right;
};

class Disjunction: public Expr
{
public:
	Disjunction(PExpr pLeft, PExpr pRight)
		: left (pLeft )
		, right(pRight)
	{}
	operator bool() const
	{
		return *left || *right;
	}

private:
	PExpr left;
	PExpr right;
};

class Conjunction: public Expr
{
public:
	Conjunction(PExpr pLeft, PExpr pRight)
		: left (pLeft )
		, right(pRight)
	{}
	operator bool() const
	{
		return *left && *right;
	}

private:
	PExpr left;
	PExpr right;
};

class Negation: public Expr
{
public:
	Negation(PExpr pExpr)
		: expr(pExpr)
	{}
	operator bool() const
	{
		return !(*expr);
	}

private:
	PExpr expr;
};

class ValueToken: public Expr
{
public:
	ValueToken(bool b)
		: value(b)
	{}
	operator bool() const
	{
		return value;
	}

private:
	bool value;
};

enum Token_value
{
	NAME, NUMBER, END = ';',
	OR = '|', AND = '&', NOT = '!',
	EQL = '=', LP = '(', RP = ')'
};

Token_value curr_tok = END;
bool number_value;
string string_value;
map<string, bool> table;
int no_of_errors;

PExpr equal_expr(bool);

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
	case '&':
	case '|':
	case '(':
	case ')':
	case '=':
	case '!':
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
		else
		{
			error("неправильная лексема");
			return curr_tok = END;
		}
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
		bool v = number_value;
		get_token();
		return make_shared<ValueToken>(v);
	}
	case LP:
	{
		PExpr e = equal_expr(true);
		if (curr_tok != RP)
			error("ожидалась )");
		get_token();
		return e;
	}
	case NOT:
	{
		PExpr e = equal_expr(true);
		e = make_shared<Negation>(e);
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
		case AND:
		{
			PExpr pRight = prim(true);
			pLeft = make_shared<Conjunction>(pLeft, pRight);
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
	while (true)
	{
		switch(curr_tok)
		{
		case OR:
		{
			PExpr pRight = term(true);
			pLeft = make_shared<Disjunction>(pLeft, pRight);
			break;
		}
		default:
			return pLeft;
		}
	}
}

PExpr equal_expr(bool get)
{
	PExpr pLeft = expr(get);
	while (true)
	{
		switch(curr_tok)
		{
		case EQL:
		{
			PExpr pRight = expr(true);
			pLeft = make_shared<Equality>(pLeft, pRight);
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
		cout << *equal_expr(false) << endl;
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
			cout << "интерактивный режим работы" << endl;
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
