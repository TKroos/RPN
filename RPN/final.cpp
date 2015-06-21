#include <iostream>
#include <cmath>
#include <string>
using namespace std;
double mod_compute(double n, double m)
{
	while (n-m>=0)
	{
		n = n-m;
	}
	return n;
}
class symbol
{
private:
	char input[40];
public:
	virtual void newsymbol(char *temp)
	{
		strcpy_s(input, 40,temp);
	}
	virtual char *getter()
	{
		if (input == NULL) return "Unknown";
		else return input;
	}
};
class operand : public symbol
{
public:
	virtual void add_oper(){}
	virtual void minus_oper(){}
	virtual void timer_oper(){}
	virtual void divide_oper(){}
	virtual void mod_oper(){}
	virtual void power_oper(){}
};
class operate : public symbol
{

};
class add : public operate
{
public:
	void add_oper(symbol left, symbol right)
	{
		char temp[40];
		sprintf_s(temp, "%f", atof(left.getter()) + atof(right.getter()));
		newsymbol(temp);
	}

};
class min : public operate
{
public:
	void minus_oper(symbol left, symbol right)
	{
		char temp[40];
		sprintf_s(temp, "%f", atof(left.getter()) - atof(right.getter()));
		newsymbol(temp);
	}
};

class times : public operate
{
public:
	void times_oper(symbol left, symbol right)
	{
		char temp[40];
		sprintf_s(temp, "%f", atof(left.getter()) * atof(right.getter()));
		newsymbol(temp);
	}
};

class divide : public operate
{
public:
	void divide_oper(symbol left, symbol right)
	{
		char temp[40];
		sprintf_s(temp, "%f", atof(left.getter()) / atof(right.getter()));
		newsymbol(temp);
	}
};
class mod : public operate
{
public:
	void mod_oper(symbol left, symbol right)
	{
		char temp[40];
		sprintf_s(temp, "%f", mod_compute(atof(left.getter()) , atof(right.getter())));
		newsymbol(temp);
	}
};

class power : public operate
{
public:
	void power_oper(symbol left, symbol right)
	{
		char temp[40];
		sprintf_s(temp, "%f", pow(atof(left.getter()), atof(right.getter())));
		newsymbol(temp);
	}
};


int main()
{
	FILE *stream;
	symbol str1, str2;
	min	str_m;
	add str_a;
	divide str_d;
	mod str_mo;
	times str_t;
	power str_p;
	char a[9999], b[99][9999], end;
	int i = 0, j = 0, k = 0;
	freopen_s(&stream, "in.txt", "r", stdin);
	freopen_s(&stream, "out.txt", "w", stdout);
	int lenth, flag_dot = 0, flag_error = 0, flag_space = 0, flag_sign = 0, flag_num = 0, flag_empty = 0;//flag_empty for content of input, flag_dot for '.', flag_error for error, flag_space for ' ',flag_sign for '+-*/%',flag_num for num
	while (cin.getline(a, 9999))
	{
		flag_dot = 0;
		flag_error = 0;
		flag_space = 0;
		flag_sign = 0;
		flag_num = 0;
		flag_empty = 0;
		k = 0;
		j = 0;
		
		
		lenth = strlen(a);
		for (k = 0; k < 99; k++)
		{
			for (i = 0; i < lenth; i++)
			{
				b[k][i] = '\0';
			}
		}
		k = 0;
		for (i = 0; i < lenth; i++)
		{
			if ((a[i] <= 57 && a[i] >= 48) || a[i] == ' ' || a[i] == '-' || a[i] == '*' || a[i] == '/' || a[i] == '%' || a[i] == '+' || a[i] == '.'||(a[i]=='p'&&a[i+1]=='o'&&a[i+2]=='w'))
			{
				flag_empty = 1;
				if (a[i] == ' ')
				{
					flag_dot = 0;
					j = 0;
					flag_space = 1;
					flag_num = 0;
					flag_sign = 0;
				}

				/*******************read the number**************/
				if ((a[i] <= 57 && a[i] >= 48) || a[i] == '.')
				{
					flag_num = 1;
					if (flag_space == 1||(flag_space==0&&flag_sign==1))
						k++;
					if (a[i] == '.')
						flag_dot++;
					if (flag_dot >= 2 || (!(a[i - 1] <= 57 && a[i - 1] >= 48) &&(a[i]=='.'))|| (!(a[i + 1] <= 57 && a[i + 1] >= 48)&&(a[i]=='.')))
					{
						flag_error = 1;
						break;
					}
					b[k][j] = a[i];
					j++;
					flag_space = 0;
				}
				/*********read the signs and compute**********/
				if (a[i] == '-' || a[i] == '*' || a[i] == '/' || a[i] == '%' || a[i] == '+' || ((a[i] == 'p'&&a[i + 1] == 'o'&&a[i + 2] == 'w')))
				{
					if (flag_sign == 1|| k<1)
					{
						flag_error = 1;
						break;
					}
					flag_sign = 1;
					if (a[i] == '-')
					{
						str1.newsymbol(b[k-1]);
						str2.newsymbol(b[k]);
						str_m.minus_oper(str1, str2);
						strcpy_s(b[k - 1], 40, str_m.getter());
						k = k - 1;
					}
					if (a[i] == '+')
					{
						str1.newsymbol(b[k - 1]);
						str2.newsymbol(b[k]);
						str_a.add_oper(str1, str2);
						strcpy_s(b[k - 1], 40, str_a.getter());
						k = k - 1;
					}
					if (a[i] == '*')
					{
						str1.newsymbol(b[k - 1]);
						str2.newsymbol(b[k]);
						str_t.times_oper(str1, str2);
						strcpy_s(b[k - 1], 40, str_t.getter());
						k = k - 1;
					}
					if (a[i] == '/')
					{
						str1.newsymbol(b[k - 1]);
						str2.newsymbol(b[k]);
						str_d.divide_oper(str1, str2);
						strcpy_s(b[k - 1], 40, str_d.getter());
						k = k - 1;
					}
					if (a[i] == '%')
					{
						if (atof(b[k-1])<=0 || atof(b[k]) <= 0)
						{
							flag_error = 1;
							break;
						}
						str1.newsymbol(b[k - 1]);
						str2.newsymbol(b[k]);
						str_mo.mod_oper(str1, str2);
						strcpy_s(b[k - 1], 40, str_mo.getter());
						k = k - 1;
					}
					if ((a[i] == 'p'))
					{
						
						str1.newsymbol(b[k - 1]);
						str2.newsymbol(b[k]);
						str_p.power_oper(str1, str2);
						strcpy_s(b[k - 1], 40, str_p.getter());
						k = k - 1;
						i = i + 2;
					}
				}
			}
			else
			{
				flag_error = 1;
				break;
			}
		}
		if (flag_error == 1 || flag_empty == 0)
		{
			cout << "SYNTAX ERROR" << endl;
		}
		else
		{
			if (k == 0)
				cout << atof(b[k]) << endl;
			else cout << "SYNTAX ERROR" << endl;
		}
	} 
}