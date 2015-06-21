#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctype.h>
#include <sstream>
#include <math.h>
//#include <cstdlib>
////////////Class declare////////////////
class symbol;
class operand;
class operators;
class add;
class sub;
class mul;
class divide;
class mod;
class power;
/////////////////////////////////////////
using namespace std;
const int MAXLEN = 256;
vector<symbol> line;
vector<vector<symbol>> stack;
vector<double> stackcalculate;
ofstream outfile;
ifstream infile;
class symbol 
{
public:
	string data;
	virtual double execute(int op1, int op2)
	{
		return 0;
	}
};
class operand:public symbol
{
public:
	double op1, op2;
	virtual void empty()
	{
		stackcalculate.clear();
	}
	virtual int getsize()
	{
		return stackcalculate.size();
	}
	virtual double pop()
	{
		double temp;
		temp = stackcalculate.at(stackcalculate.size() - 1); //get the last element
		stackcalculate.pop_back();
		return temp;
	}
	virtual void push(string arg)
	{
		double temp;
		stringstream sstr(arg);
		sstr >> temp;
		stackcalculate.push_back(temp);
	}
	virtual void push(double arg2)
	{
		stackcalculate.push_back(arg2);
	}
};
class operators:public symbol
{
public:
	virtual double execute(int op1,int op2)
	{
		return 0;
	}
};
class add : public operators
{
public:
	double execute(double op1, double op2)
	{
		return op1 + op2;
	}
	virtual string getsysmbol()
	{
		return "+";
	}
};
class sub : public operators
{
public:
	double execute(double op1, double op2)
	{
		return op1 - op2;
	}
	virtual string getsysmbol()
	{
		return "-";
	}
};
class mul : public operators
{
public:
	double execute(double op1, double op2)
	{
		return op1 * op2;
	}
	virtual string getsysmbol()
	{
		return "*";
	}
};
class divide:public operators
{
public:
	double execute(double op1, double op2)
	{
		return op1 / op2;
	}
	virtual string getsysmbol()
	{
		return "/";
	}
};
class mod : public operators
{
public:
	double execute(double op1, double op2)
	{
		return fmod(op1, op2);
	}
	virtual string getsysmbol()
	{
		return "%";
	}
};
class power : public operators
{
public:
	double execute(double op1, double op2)
	{
		int i, j, temp=1;
		i = static_cast<int>(op2);
		for (j = 1; j <= i; j++)
		{
			temp = temp*op1;
		}
		return temp;
	}
	virtual string getsysmbol()
	{
		return "pow";
	}
};


operand op; //add addoperator; sub suboperator; mul muloperator; divide divoperator; mod modoperator; power powoperator;
/////////declare///////
void openthefile();
void inputdata();
bool IsDigit2(string str);
int typeofoperators(string chuanru);
///////////////////////
int typeofoperators(string chuanru)
{
	if (chuanru=="+")
	{
		return 1;
	}
	if (chuanru == "-")
	{
		return 2;
	}
	if (chuanru == "*")
	{
		return 3;
	}
	if (chuanru == "/")
	{
		return 4;
	}
	if (chuanru == "%")
	{
		return 5;
	}
	if (chuanru == "pow")
	{
		return 6;
	}
	return 0;//error not such operator
}
bool IsDigit2(string str)
{
	bool temp;
	for (int i = 0; i<str.size(); i++)
	{
		if (((str.at(i)<='9') && (str.at(i) >= '0'))||str.at(i)=='.')
		{
			temp = true;
		}
		else
		{
			temp = false;
			break;
		}
	}
	if (temp==true)
	{
		return true;
	} 
	else
	{
		return false;
	}
}
void inputdata()
{
	string temp; char temp2; symbol waittodo;
	while (infile.peek()!=EOF)
	{
		while (infile.peek() == '\n')
		{
			infile.get();
			if (line.size()!=0)
			{
				stack.push_back(line);
			}
			line.clear();
		}
		while (infile.peek() == ' ')///delete the front white space.
		{
			temp = "";
			infile.get();
		}
		if (infile.peek() == '.' || isdigit(infile.peek()) == 4)
		{
			while (infile.peek() == '.' || isdigit(infile.peek()) == 4)//read operand.
			{
				temp2 = infile.get();
				temp = temp + temp2;
			}
		} 
		else
		{
			while (isdigit(infile.peek()) == 0 && infile.peek() != ' '&&infile.peek() != '\n'&&infile.peek() != EOF)//read operators
			{
				temp2 = infile.get();
				temp = temp + temp2;
				if (temp2 == '+' || temp2 == '-' || temp2 == '*' || temp2 == '/')
				{
					break;
				}
			}
		}
		if (temp!="")
		{
			waittodo.data = temp;
			line.push_back(waittodo);
		}
		temp = "";
	}
	if (line.size()!=0)
	{
		stack.push_back(line);
	}
	line.clear();
}
void openthefile()
{
	string tempvarpath;
	stack.clear(); line.clear();
	cin >> tempvarpath;
	infile.open(tempvarpath);
	while (infile.is_open()==0)
	{
		infile.close();
		cout << "Can't not open the file. Input again."<< endl;
		cin >> tempvarpath;
		infile.open(tempvarpath);
	}
	cout << "Open the file success." << endl;
	inputdata();
	infile.close();
}
void getresult()
{
	symbol *temppoint;
	system("cls");
	bool occurerror=false;
	for (int i=0; i <stack.size();i++)
	{
		occurerror = false;
		for (int j = 0; j<stack[i].size(); j++)
		{
			if (IsDigit2(stack[i][j].data)==true)//is digit
			{
				op.push(stack[i][j].data);
			} 
			else
			{
				temppoint = new symbol;
				int opertype = typeofoperators(stack[i][j].data);//if it is 0 it will have error
				if (opertype==1)//+
				{
					if (op.getsize()>=2)
					{
						op.op2 = op.pop();
						op.op1 = op.pop();
						op.push(dynamic_cast<add*>(temppoint)->execute(op.op1, op.op2));
					} 
					else
					{
						occurerror = true;
						break;
					}
				}
				if (opertype == 2)//-
				{
					if (op.getsize() >= 2)
					{
						op.op2 = op.pop();
						op.op1 = op.pop();
						op.push(dynamic_cast<sub*>(temppoint)->execute(op.op1, op.op2));
					} 
					else
					{
						occurerror = true;
						break;
					}
				}
				if (opertype == 3)//*
				{
					if (op.getsize() >= 2)
					{
						op.op2 = op.pop();
						op.op1 = op.pop();
						op.push(dynamic_cast<mul*>(temppoint)->execute(op.op1, op.op2));
					} 
					else
					{
						occurerror = true;
						break;
					}
				}
				if (opertype == 4)//div
				{
					if (op.getsize() >= 2)
					{
						op.op2 = op.pop();
						op.op1 = op.pop();
						op.push(dynamic_cast<divide*>(temppoint)->execute(op.op1, op.op2));
					} 
					else
					{
						occurerror = true;
						break;
					}
				}
				if (opertype == 5)//mod
				{
					if (op.getsize() >= 2)
					{
						op.op2 = op.pop();
						op.op1 = op.pop();
						op.push(dynamic_cast<mod*>(temppoint)->execute(op.op1, op.op2));
					} 
					else
					{
						occurerror = true;
						break;
					}
				}
				if (opertype == 6)//pow
				{
					if (op.getsize() >= 2)
					{
						op.op2 = op.pop();
						op.op1 = op.pop();
						op.push(dynamic_cast<power*>(temppoint)->execute(op.op1, op.op2));
					} 
					else
					{
						occurerror = true;
						break;
					}
				}
				if (opertype == 0)//error
				{
					occurerror = true;
					break;
				}
				delete temppoint;
			}
		}
		///get the result of one line
		if (occurerror==false&&op.getsize()==1)
		{
			cout << op.pop() << endl;
		} 
		else
		{
			op.empty();
			cout << "SYNTAX ERROR" << endl;
		}
		
	}

}
void main()
{
	cout << "Please check the extra credit part of this program." << endl;
	cout << "Please input your file name." << endl;
	openthefile();
	getresult();
	system("pause");
}