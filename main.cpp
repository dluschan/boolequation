#include <iostream>
#include <map>
#include <vector>
#include <math.h>
#include <memory>

using namespace std;

class Expr
{
public:
	Expr(){}
	virtual operator bool() const = 0;
	virtual void setValue(bool){}
};
typedef shared_ptr<Expr> PExpr;

class TrueExpr: public Expr
{
public:
	TrueExpr()
	{}
	operator bool() const
	{
		return true;
	}
};

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

class Constant: public Expr
{
public:
	Constant(bool b)
		: value(b)
	{}
	operator bool() const
	{
		return value;
	}

private:
	bool value;
};

class Variable: public Expr
{
public:
	Variable(){}
	void setValue(bool b)
	{
		value = b;
	}

	operator bool() const
	{
		return value;
	}

private:
	bool value;
};

enum Token_value
{
	ERROR, NAME, NUMBER, END = ';',
	OR = '|', AND = '&', NOT = '!',
	EQL = '=', LP = '(', RP = ')'
};

Token_value curr_tok = END;
bool number_value;
string string_value;
map<string, PExpr> variables;

PExpr equal_expr(istream& s);

Token_value get_token(istream& s)
{
	char ch = 0;
	s >> ch;
	switch(ch)
	{
	case ';': case '&': case '|': case '(': case ')': case '=': case '!':
		return curr_tok = Token_value(ch);
	case '0': case '1':
		s.putback(ch);
		s >> number_value;
		return curr_tok = NUMBER;
	default:
		if (isalpha(ch))
		{
			s.putback(ch);
			s >> string_value;
			return curr_tok = NAME;
		}
		else
		{
			return curr_tok = ERROR;
		}
	}
}

PExpr prim(istream& s)
{
	get_token(s);

	switch(curr_tok)
	{
	case NUMBER:
	{
		bool v = number_value;
		get_token(s);
		return make_shared<Constant>(v);
	}
	case LP:
	{
		PExpr e = equal_expr(s);
		if (curr_tok != RP)
			cerr << "ожидалась )" << endl;
		get_token(s);
		return e;
	}
	case NOT:
	{
		PExpr e = prim(s);
		e = make_shared<Negation>(e);
		return e;
	}
	case NAME:
	{
		if (variables.find(string_value) == variables.end())
			variables[string_value] = make_shared<Variable>();
		get_token(s);
		return variables[string_value];
	}
	default:
		return PExpr();
	}
}

PExpr term(istream& s)
{
	PExpr pLeft = prim(s);
	while (true)
	{
		switch(curr_tok)
		{
		case AND:
		{
			PExpr pRight = prim(s);
			pLeft = make_shared<Conjunction>(pLeft, pRight);
			break;
		}
		default:
			return pLeft;
		}
	}
}

PExpr or_expr(istream& s)
{
	PExpr pLeft = term(s);
	while (true)
	{
		switch(curr_tok)
		{
		case OR:
		{
			PExpr pRight = term(s);
			pLeft = make_shared<Disjunction>(pLeft, pRight);
			break;
		}
		default:
			return pLeft;
		}
	}
}

PExpr equal_expr(istream& s)
{
	PExpr pLeft = or_expr(s);
	while (true)
	{
		switch(curr_tok)
		{
		case EQL:
		{
			PExpr pRight = or_expr(s);
			pLeft = make_shared<Equality>(pLeft, pRight);
			break;
		}
		default:
			return pLeft;
		}
	}
}

class Compiler
{
public:
	Compiler(istream& s)
	{
		char ch;
		pGeneralExpr = make_shared<TrueExpr>(TrueExpr());

		while(s >> ch)
		{
			s.putback(ch);
			PExpr pExpr = equal_expr(s);
			if (curr_tok == END)
			{
				if (pExpr)
					pGeneralExpr = make_shared<Conjunction>(pGeneralExpr, pExpr);
			}
			else
			{
				cerr << "неправильная лексема" << endl;
				break;
			}
		}
	}

	void overrun_expression()
	{
		for (uint i = 0; i < pow(2, variables.size()); ++i)
		{
			unsigned k = i;
			for (auto it = variables.rbegin(); it != variables.rend(); ++it)
			{
				it->second->setValue(0x1 & k);
				k >>= 1;
			}
			if (*pGeneralExpr)
			{
				for(auto pVar: variables)
					cout << *pVar.second;
				cout << endl;
			}
		}
	}

private:
	PExpr pGeneralExpr;
};

int main()
{
	Compiler com(cin);
	com.overrun_expression();
	return 0;
}
