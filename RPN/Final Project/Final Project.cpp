#include<fstream>
#include<vector>
#include<string.h>
#include<iostream>
#include "math.h"
using namespace std;
const char template_operator[6][4] = { "+", "-", "*", "/", "%", "pow" };
class Symbol { 
public:
	virtual void setsymbol(char *temp){
		strcpy(oper, temp);
	}
	virtual char *getter(){
		if (oper == NULL) return "Unknown";
		else return oper;
	}
private:
	char oper[10];
};
class Operand : public Symbol {
public:
	double operands;
};
class Operators : public Symbol { 
public:
	virtual void perform(){}
};
class Add : public Operators { 
public:
	void perform(Symbol left, Symbol right){
		char temp[20];
		sprintf(temp, "%f", atof(left.getter())+atof(right.getter()));
		setsymbol(temp);
	}
};
class Subtract : public Operators {
public:
	void perform(Symbol left, Symbol right){
		char temp[20];
		sprintf(temp, "%f", atof(left.getter()) - atof(right.getter()));
		setsymbol(temp);
	}
};
class Times : public Operators {
public:
	void perform(Symbol left, Symbol right){
		char temp[20];
		sprintf(temp, "%f", atof(left.getter()) * atof(right.getter()));
		setsymbol(temp);
	}
};
class Divide : public Operators {
public:
	void perform(Symbol left, Symbol right){
		char temp[20];
		sprintf(temp, "%f", atof(left.getter()) / atof(right.getter()));
		setsymbol(temp);
	}
};
class Mod : public Operators {
public:
	void perform(Symbol left, Symbol right){
		char temp[20];
		sprintf(temp, "%f", fmod(atof(left.getter()), atof(right.getter())));
		setsymbol(temp);
	}
};
class Power : public Operators {
public:
	void perform(Symbol left, Symbol right){
		char temp[20];
		sprintf(temp, "%f", pow(atof(left.getter()), atof(right.getter())));
		setsymbol(temp);
	}
};
void openfile(ifstream &fp){
	char filename[50];
	do{
		cout << "Please input the location and name of the input text file" << endl;
		cin >> filename;
		fp.open(filename, ios::in);
		if (!fp)
			cout << "Wrong location or name! Please input again!" << endl;
	} while (!fp);
}
void filter_space(char *temp, char str[100][20]){
	int i,j,k = 0;
	i = 0; j = 0;
	while (temp[k] != '\0'){
		j = 0;
		if (temp[k] != ' ') {
			do{
				str[i][j] = temp[k];
				j++;
				k++;
			} while (temp[k] != ' ' && temp[k] != '\0');
			str[i][j] = '\0';
			i++;
		}
		else {
			do{
				k++;
			} while (temp[k] == ' ');
		}
	}
	strcpy(str[i], "end");
}
bool isoperand(char c){
	if (46 <= static_cast<int>(c) && static_cast<int>(c) <= 57 && static_cast<int>(c) != 47)
		return true;
	else return false;
}
bool isoperator(char c){
	if (static_cast<int>(c) == 37 || static_cast<int>(c) == 42 || static_cast<int>(c) == 43 || static_cast<int>(c) == 45 || static_cast<int>(c) == 47)
		return true;
	else return false;
}
void allocate(char str[100][20], char obj[100][20]){
	int i = 0, j = 0, m = 0, n = 0;
	while (strcmp(str[i], "end") != 0){
		n = 0;
		j = 0;
		while (str[i][j] != '\0') {
			n = 0;
			if (isoperand(str[i][j]) == true && static_cast<int>(str[i][j]) != 46){
				do{
					obj[m][n] = str[i][j];
					j++;
					n++;
				} while (isoperand(str[i][j]) == true);
			}
			else if (static_cast<int>(str[i][j]) == 112){
				obj[m][n] = str[i][j];
				j++;
				n++;
				if (static_cast<int>(str[i][j]) == 111){
					obj[m][n] = str[i][j];
					j++;
					n++;
					if (static_cast<int>(str[i][j]) == 119){
						obj[m][n] = str[i][j];
						j++;
						n++;
					}
				}
			}
			else if (isoperator(str[i][j]) == true){
				obj[m][n] = str[i][j];
				n++;
				j++;
			}
			else {
				obj[m][n] = str[i][j];
				n++;
				j++;
			}
			if (obj[m][n - 1] == '.') obj[m][n-1] = '\0';
			else obj[m][n] = '\0';
			m++;
		}
		i++;
	}
	strcpy(obj[m], "end");
}
void display(char *temp){
	int i = 0;
	cout << "~Input: ";
	while (temp[i] != '\0'){
		cout << temp[i];
		i++;
	}
	cout << endl;
}
bool is_operators(char *obj){
	for (int i = 0; i <= 5; i++)
		if (strcmp(obj, template_operator[i]) == 0)
			return true;
	return false;
}
bool syntax_check(char obj[100][20]){
	int i = 0, flag = 0;
	for (i = 0; strcmp(obj[i], "end") != 0; i++){
		if (is_operators(obj[i]) || strcmp(obj[i], "pow") == 0){
			flag--;
		}
		else if (isoperand(obj[i][0])) flag++;
		if (flag < 1) return false;
	}
	if (flag == 1) return true;
	else return false;
}
bool right(char *temp){
	return (isoperator(*temp) || isoperand(*temp) || *temp == 'p');
}
void syntax_error(){
	cout << "Result: SYNTAX ERROR" << endl;
}
void store_in_symbolvector(vector<Symbol> &ptr, char obj[100][20], const int length){
	Symbol *p = new Symbol;
	for (int i = 0; i < length; i++){
		p->setsymbol(obj[i]);
		ptr.push_back(*p);
	}
}
bool process(vector<Symbol> &ptr, int length){
	Symbol *p;
	for (int i = 0; i < length; i++){
		if (!right(ptr[i].getter())) return false;
		else if (strcmp(ptr[i].getter(), "+") == 0){
			p = new Add;
			dynamic_cast<Add*>(p)->perform(ptr[i - 2], ptr[i - 1]);
			ptr[i].setsymbol(p->getter());
			delete p;
			if (i >= 3) for (int j = i - 1; j >= 2; j--) ptr[j].setsymbol(ptr[j - 2].getter());
		}
		else if (strcmp(ptr[i].getter(), "-") == 0){
			p = new Subtract;
			dynamic_cast<Subtract*>(p)->perform(ptr[i - 2], ptr[i - 1]);
			ptr[i].setsymbol(p->getter());
			delete p;
			if (i >= 3) for (int j = i - 1; j >= 2; j--) ptr[j].setsymbol(ptr[j - 2].getter());
		}
		else if (strcmp(ptr[i].getter(), "*") == 0){
			p = new Times;
			dynamic_cast<Times*>(p)->perform(ptr[i - 2], ptr[i - 1]);
			ptr[i].setsymbol(p->getter());
			delete p;
			if (i >= 3) for (int j = i - 1; j >= 2; j--) ptr[j].setsymbol(ptr[j - 2].getter());
		}
		else if (strcmp(ptr[i].getter(), "/") == 0){
			p = new Divide;
			dynamic_cast<Divide*>(p)->perform(ptr[i - 2], ptr[i - 1]);
			ptr[i].setsymbol(p->getter());
			delete p;
			if (i >= 3) for (int j = i - 1; j >= 2; j--) ptr[j].setsymbol(ptr[j - 2].getter());
		}
		else if (strcmp(ptr[i].getter(), "%") == 0){
			p = new Mod;
			dynamic_cast<Mod*>(p)->perform(ptr[i - 2], ptr[i - 1]);
			ptr[i].setsymbol(p->getter());
			delete p;
			if (i >= 3) for (int j = i - 1; j >= 2; j--) ptr[j].setsymbol(ptr[j - 2].getter());
		}
		else if (strcmp(ptr[i].getter(), "pow") == 0){
			p = new Power;
			dynamic_cast<Power*>(p)->perform(ptr[i - 2], ptr[i - 1]);
			ptr[i].setsymbol(p->getter());
			delete p;
			if (i >= 3) for (int j = i - 1; j >= 2; j--) ptr[j].setsymbol(ptr[j - 2].getter());
		}
	}
	return true;
}
void main(){
	char temp[100];
	char str[100][20];
	char obj[100][20];
	int i = 0, j, k, length;
	float a;
	ifstream fp;
	vector<Symbol> ptr;
	cout << endl;
	cout << "                 Welcome To Reverse Polish Notation Calculator!" << endl;
	cout << "****************************************************************************************************************************************************************";
	openfile(fp);
	system("cls");
	cout << "                      Reverse Polish Notation Calculator" << endl;
	cout << "****************************************************************************************************************************************************************";
	do{
		ptr.clear();
		length = 0;
		fp.getline(temp, 100);
		filter_space(temp, str);
		display(temp);
		allocate(str, obj);
		while (strcmp(obj[length], "end") != 0) length++;
		Add pp;
		store_in_symbolvector(ptr, obj, length);
		if (syntax_check(obj) && process(ptr, length)) {
			double output = atof(ptr[length - 1].getter());
			cout << "Result: " << output << endl;
		}
		else syntax_error();
		cout << endl;
	} while (!fp.eof());
	system("pause");
}