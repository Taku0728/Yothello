#pragma once
#include "stdafx.h" 
using  std::cout;
using  std::cin;
using  std::wcout;
using  std::vector;
using  std::random_device;
using  std::endl;
using  std::abs;

using std::string;
using std::vector;
using std::random_device;
using std::mt19937;
using std::normal_distribution;

class Functor
{
public:
	Functor();
	virtual ~Functor();
	virtual double operator()(double x, double y);

private:
	double data;

};

class Func1: public Functor
{
public:
	Func1();
	~Func1();
	double operator()(double x, double y);

private:
	double data;

};

class Func2 : public Functor
{
public:
	Func2();
	~Func2();
	double operator()(double x, double y);

private:
	double data;

};

class Rotate_board
{
public:
	Rotate_board();
	~Rotate_board();
	void operator()(vector<double> &x);

private:
};

class Reflect_board
{
public:
	Reflect_board();
	~Reflect_board();
	void operator()(vector<double> &x);
private:
	
};

class Translate
{
public:
	Translate();
	~Translate();
	int operator()(const char *p);

private:

};

class Reverse
{
public:
	Reverse();
	~Reverse();
	void operator()(vector<int> &v);

private:

};

class Reverse2
{
public:
	Reverse2();
	~Reverse2();
	void operator()(vector<int> &v);

private:

};

inline double winvalue(int n) {
	switch (n) {
	case 0:
		return 0.5;
		break;
	case 2:
		return 0.0;
		break;
	}
}

class Fibc
{
public:
	Fibc();
	~Fibc();
	int operator()(int n);

private:

};

int fib(int n);

string get_pcname();





//class File_Access
//{
//public:
//	File_Access();
//	~File_Access();
//	void readFileFormat(FILE* fp, int* row, int* col);
//	void chop_crlf(char* buff);
//	void readFileData(FILE* fp, int** data);
//	int** allocArray(int row, int col);
//
//private:
//};